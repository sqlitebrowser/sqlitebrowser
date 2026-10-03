# SPDX-FileCopyrightText: (C) 2026 SeongTae Jeong <seongtaejg@gmail.com>
# SPDX-License-Identifier: MIT

import hashlib
import re
from pathlib import Path
from urllib.request import urlopen

DOWNLOAD_PAGE_URL = "https://sqlite.org/download.html"
DOWNLOAD_BASE_URL = "https://sqlite.org/"
PRODUCT_REGEX = re.compile(
    r"PRODUCT,(\d+\.\d+\.\d+),(\d+/sqlite-amalgamation-\d+\.zip),\d+,([0-9a-fA-F]+)"
)


def fetch_download_page():
    with urlopen(DOWNLOAD_PAGE_URL, timeout=30) as response:
        return response.read().decode("utf-8", errors="replace")


def version_key(version):
    return tuple(int(part) for part in version.split("."))


def get_latest_amalgamation_info(download_page):
    matches = []
    for match in PRODUCT_REGEX.finditer(download_page):
        version, relative_url, sha3_256 = match.groups()
        matches.append(
            {
                "version": version,
                "relative_url": relative_url,
                "url": f"{DOWNLOAD_BASE_URL}{relative_url}",
                "filename": Path(relative_url).name,
                "sha3_256": sha3_256,
            }
        )

    if not matches:
        raise ValueError("No SQLite amalgamation download entries were found.")

    return max(matches, key=lambda entry: version_key(entry["version"]))


def download_file(url, destination):
    destination.parent.mkdir(parents=True, exist_ok=True)

    with urlopen(url, timeout=30) as response, destination.open("wb") as file:
        while True:
            chunk = response.read(8192)
            if not chunk:
                break
            file.write(chunk)

    return destination


def verify_sha3_256(file_path, expected_sha3_256):
    hasher = hashlib.sha3_256()

    with file_path.open("rb") as file:
        for chunk in iter(lambda: file.read(8192), b""):
            hasher.update(chunk)

    actual_sha3_256 = hasher.hexdigest()
    if actual_sha3_256.lower() != expected_sha3_256.lower():
        raise ValueError(
            f"SHA3-256 mismatch for {file_path}: expected {expected_sha3_256}, got {actual_sha3_256}"
        )

    return actual_sha3_256


if __name__ == "__main__":
    latest = get_latest_amalgamation_info(fetch_download_page())
    download_path = Path(latest["filename"])
    saved_path = download_file(latest["url"], download_path)
    verify_sha3_256(saved_path, latest["sha3_256"])

    print(saved_path.name)
