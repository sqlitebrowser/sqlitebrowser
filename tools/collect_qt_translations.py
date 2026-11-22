#!/usr/bin/env python3
"""Collects the Qt translation catalogs that the app loads, for the macOS app bundle and the Windows installers.

For the language of the app, Application.cpp loads one Qt catalog, qt_<locale>.qm, which translates the texts that Qt
draws itself, e.g. the standard buttons of QDialogButtonBox and QMessageBox and the context menu of text fields.
Since Qt 5.3, qt_<language>.qm is only a list of module catalogs (qtbase_<language>.qm, ...), and QTranslator fails to
load it, without a warning, if one of them is missing. Which catalogs exist, and which modules they list, differs
between the Qt versions of the platforms, so lists of file names that are maintained by hand go out of date.

So this script takes the app languages from config/translations.cmake, finds the Qt catalog of each one in the
translations directory of the Qt that the app is deployed with, the same way as QTranslator does, and follows its
dependencies. A language for which Qt has no catalog is skipped. So is, with a warning, a language whose
catalog depends on a file that this Qt does not have, because QTranslator cannot load that catalog either. E.g. the
qt_pt_BR.qm of Qt 5.15.3, which Ubuntu 22.04 ships, lists 'qtbase_' instead of 'qtbase_pt_BR'. Since the script copies
every dependency that it finds, a missing one can only be missing from Qt itself, so it does not fail the build.

Outputs:
  --copy-to DIR  Copies the files into DIR, e.g. Contents/translations of the macOS app bundle. May be repeated.
  --wxs FILE     Writes the WiX fragment with the ComponentGroup "TranslationsGroup" of the Windows installers.

Only the standard library is used, so that it runs with the Python of any runner.
"""

from __future__ import annotations

import argparse
import re
import shutil
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from xml.sax.saxutils import quoteattr

REPO_ROOT = Path(__file__).resolve().parent.parent

# The .qm format: the magic number, followed by sections of a 1-byte tag, a 4-byte big-endian length and the data.
QM_MAGIC = bytes([0x3C, 0xB8, 0x64, 0x18, 0xCA, 0xEF, 0x9C, 0x95, 0xCD, 0x21, 0x1C, 0xBF, 0x60, 0xA1, 0xBD, 0xDD])
QM_TAG_MESSAGES = 0x69
QM_TAG_DEPENDENCIES = 0x96

# The app stores QLocale(<code>).name() as its language, see PreferencesDialog::fillLanguageBox(), so a code without
# a region gets the default region of the language. That only matters where Qt has catalogs per region, e.g. "zh"
# becomes "zh_CN" and loads qt_zh_CN.qm. For the other languages, QTranslator ends up at qt_<language>.qm anyway.
DEFAULT_REGIONS = {"pt": "BR", "zh": "CN"}


class CollectError(Exception):
    pass


class MissingDependency(CollectError):
    pass


@dataclass
class Catalog:
    path: Path
    dependencies: List[str]
    messages: bytes


@dataclass
class Language:
    code: str
    locale: str
    catalog: Optional[str] = None
    files: List[str] = field(default_factory=list)
    problem: Optional[MissingDependency] = None


def read_catalog(path: Path) -> Catalog:
    data = path.read_bytes()
    if not data.startswith(QM_MAGIC):
        raise CollectError(f"'{path}' is not a Qt translation file.")
    dependencies: List[str] = []
    messages = b""
    pos = len(QM_MAGIC)
    while pos + 5 <= len(data):
        tag = data[pos]
        (length,) = struct.unpack(">I", data[pos + 1 : pos + 5])
        block = data[pos + 5 : pos + 5 + length]
        pos += 5 + length
        if tag == QM_TAG_MESSAGES:
            messages = block
        elif tag == QM_TAG_DEPENDENCIES:
            # QDataStream strings: a 4-byte big-endian length in bytes, followed by UTF-16BE.
            i = 0
            while i + 4 <= len(block):
                (size,) = struct.unpack(">I", block[i : i + 4])
                dependencies.append(block[i + 4 : i + 4 + size].decode("utf-16-be"))
                i += 4 + size
    return Catalog(path, dependencies, messages)


def find_catalog(directory: Path, name: str) -> Optional[Path]:
    """Finds a catalog like QTranslator::load(name, directory): it strips "_<suffix>" until a file exists."""
    while True:
        for candidate in (directory / f"{name}.qm", directory / name):
            if candidate.is_file():
                return candidate
        cut = max(name.rfind("_"), name.rfind("."))
        if cut <= 0:
            return None
        name = name[:cut]


def resolve(directory: Path, name: str, owner: str, files: Dict[str, Catalog]) -> None:
    """Adds the catalog and, recursively, its dependencies to files. A missing dependency fails the whole load."""
    path = find_catalog(directory, name)
    if path is None:
        raise MissingDependency(f"'{owner}' depends on '{name}', which is not in '{directory}'.")
    if path.name in files:
        return
    catalog = read_catalog(path)
    files[path.name] = catalog
    for dependency in catalog.dependencies:
        resolve(directory, dependency, path.name, files)


def read_app_languages(cmake_file: Path) -> List[str]:
    text = cmake_file.read_text(encoding="utf-8")
    # Comments are removed first, because a ')' in one would end the set() call early and drop the languages after it,
    # and a file name in one would add a language: bracket comments (#[[ ... ]], also over several lines) and line
    # comments. Quoted ("...") and bracket ([[...]], [=[...]=]) arguments are matched as a whole, so that a '#', '"' or
    # ')' in them is kept.
    text = re.sub(
        r'"(?:\\.|[^"\\])*"|\[(=*)\[.*?\]\1\]|#\[(=*)\[.*?\]\2\]|#[^\n]*',
        lambda match: match.group(0) if match.group(0)[0] in '"[' else "",
        text,
        flags=re.DOTALL,
    )
    block = re.search(r'set\(\s*SQLB_TSS\b((?:"(?:\\.|[^"\\])*"|\[(=*)\[.*?\]\2\]|[^")])*)\)', text, re.DOTALL)
    codes = re.findall(r"sqlb_([A-Za-z_]+)\.ts", block.group(1)) if block else []
    if not codes:
        raise CollectError(f"No translation files were found in SQLB_TSS of '{cmake_file}'.")
    return codes


def app_locale(code: str) -> str:
    return f"{code}_{DEFAULT_REGIONS[code]}" if code in DEFAULT_REGIONS else code


def collect(qt_translations: Path, codes: List[str]) -> Tuple[List[Language], Dict[str, Catalog]]:
    if not any(qt_translations.glob("qt_*.qm")):
        raise CollectError(f"'{qt_translations}' has no Qt translation catalogs (qt_*.qm).")

    languages: List[Language] = []
    shipped: Dict[str, Catalog] = {}
    for code in codes:
        language = Language(code, app_locale(code))
        languages.append(language)

        path = find_catalog(qt_translations, f"qt_{language.locale}")
        if path is None:
            continue
        language.catalog = path.name

        files: Dict[str, Catalog] = {}
        try:
            resolve(qt_translations, path.stem, f"qt_{language.locale}", files)
        except MissingDependency as error:
            language.problem = error
            continue
        if not any(catalog.messages for catalog in files.values()):
            # E.g. qt_en.qm, which has no translations, because the source texts are English.
            continue
        language.files = sorted(files)
        shipped.update(files)
    if not shipped:
        raise CollectError(f"None of the app languages has a Qt catalog in '{qt_translations}'.")
    return languages, shipped


def write_wxs(path: Path, languages: List[Language], source_dir: str) -> None:
    # The Component and File Ids default to the file names, as in the hand-written file that this replaces, so that
    # an upgrade sees the same components.
    lines = [
        '<?xml version="1.0" encoding="utf-8"?>',
        "<!-- Generated by tools/collect_qt_translations.py. Do not edit, and do not commit. -->",
        '<Wix xmlns="http://schemas.microsoft.com/wix/2006/wi">',
        "",
        "    <?include variables.wxi ?>",
        "",
        '    <Fragment Id="TranslationsFragment">',
        "",
        "        <DirectoryRef Id=\"INSTALLDIR\">",
        '            <Directory Id="translations" Name="translations">',
    ]
    written = set()
    for language in languages:
        files = [name for name in language.files if name not in written]
        if not files:
            continue
        lines.append(f"                <!-- {language.code}: {language.catalog} -->")
        for name in files:
            source = quoteattr(f"{source_dir}\\{name}")
            lines.append(f"                <Component><File Source={source} /></Component>")
            written.add(name)
    lines += [
        "            </Directory>",
        "        </DirectoryRef>",
        "",
        '        <ComponentGroup Id="TranslationsGroup">',
    ]
    lines += [f"            <ComponentRef Id={quoteattr(name)} />" for name in sorted(written)]
    lines += [
        "        </ComponentGroup>",
        "",
        "    </Fragment>",
        "</Wix>",
    ]
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--qt-translations", required=True, type=Path, help="The translations directory of Qt.")
    parser.add_argument(
        "--languages-from",
        type=Path,
        default=REPO_ROOT / "config" / "translations.cmake",
        help="The CMake file with SQLB_TSS, the translations of the app (default: %(default)s).",
    )
    parser.add_argument("--copy-to", type=Path, action="append", default=[], help="Copies the files to this directory.")
    parser.add_argument("--wxs", type=Path, help="Writes the WiX fragment of the files to this file.")
    parser.add_argument(
        "--wxs-source-dir",
        default=r"$(var.QtPath)\translations",
        help="The directory of the files in the WiX fragment (default: %(default)s).",
    )
    args = parser.parse_args()

    try:
        languages, shipped = collect(args.qt_translations, read_app_languages(args.languages_from))
    except (CollectError, OSError) as error:
        # The ::error:: prefix shows the message as an annotation on GitHub Actions, and is harmless elsewhere.
        print(f"::error::{error}", file=sys.stderr)
        return 1
    for language in languages:
        if language.problem:
            print(
                f"::warning::{language.problem} So Qt cannot load '{language.catalog}' either, "
                f"and the Qt translation of '{language.code}' is not shipped.",
                file=sys.stderr,
            )

    for destination in args.copy_to:
        destination.mkdir(parents=True, exist_ok=True)
        for name, catalog in sorted(shipped.items()):
            shutil.copy2(catalog.path, destination / name)
    if args.wxs:
        write_wxs(args.wxs, languages, args.wxs_source_dir)
    return 0


if __name__ == "__main__":
    sys.exit(main())
