# SPDX-FileCopyrightText: (C) 2026 SeongTae Jeong <seongtaejg@gmail.com>
# SPDX-License-Identifier: MIT

import argparse
import requests
import hashlib
from packaging.version import Version
from pathlib import Path

JSON_URL = "https://raw.githubusercontent.com/slproweb/opensslhashes/refs/heads/master/win32_openssl_hashes.json"


def get_latest_installer_info(version_prefix, arch, bits, light):
    response = requests.get(JSON_URL, timeout=30)
    response.raise_for_status()

    data = response.json()
    files = data.get("files", {})

    matched = []
    for filename, meta in files.items():
        basever = meta.get("basever", "")
        if (
            meta.get("arch") == arch
            and (bits is None or meta.get("bits") == bits)
            and meta.get("light") is light
            and meta.get("installer") == "exe"
            and basever.startswith(version_prefix)
        ):
            matched.append(
                {
                    "filename": filename,
                    "basever": basever,
                    # The letter releases of OpenSSL 1.1.1 (e.g. 1.1.1w) share the same "basever".
                    "subver": meta.get("subver", ""),
                    "url": meta["url"],
                    "sha256": meta["sha256"],
                }
            )

    if not matched:
        raise ValueError(
            f"No files matching your criteria were found for {version_prefix}x."
        )

    latest = max(matched, key=lambda x: (Version(x["basever"]), x["subver"]))
    return latest


def download_file(url, destination):
    destination.parent.mkdir(parents=True, exist_ok=True)

    with requests.get(url, stream=True, timeout=30) as response:
        response.raise_for_status()

        with destination.open("wb") as file:
            for chunk in response.iter_content(chunk_size=8192):
                if chunk:
                    file.write(chunk)

    return destination


def verify_sha256(file_path, expected_sha256):
    hasher = hashlib.sha256()

    with file_path.open("rb") as file:
        for chunk in iter(lambda: file.read(8192), b""):
            hasher.update(chunk)

    actual_sha256 = hasher.hexdigest()
    if actual_sha256.lower() != expected_sha256.lower():
        raise ValueError(
            f"SHA256 mismatch for {file_path}: expected {expected_sha256}, got {actual_sha256}"
        )

    return actual_sha256


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Download the latest Win32/Win64 OpenSSL installer of Shining Light Productions and print its file name."
    )
    # The defaults select the full OpenSSL 3.x installer for all architectures, which the build links with.
    parser.add_argument("--version-prefix", default="3.", help='e.g. "3." or "1.1." (default: "3.")')
    parser.add_argument("--arch", default="Universal", help='e.g. "Universal", "INTEL" or "ARM" (default: "Universal")')
    parser.add_argument("--bits", type=int, choices=[32, 64], help="32 or 64, for the architecture-specific installers")
    parser.add_argument("--light", action="store_true", help="the Light installer, which only contains the binaries")
    return parser.parse_args()


if __name__ == "__main__":
    arguments = parse_arguments()
    latest = get_latest_installer_info(arguments.version_prefix, arguments.arch, arguments.bits, arguments.light)
    download_path = Path(latest["filename"])
    saved_path = download_file(latest["url"], download_path)
    sha256 = verify_sha256(saved_path, latest["sha256"])

    print(latest["filename"])
