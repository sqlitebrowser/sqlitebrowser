# SPDX-FileCopyrightText: (C) 2026 SeongTae Jeong <seongtaejg@gmail.com>
# SPDX-License-Identifier: MIT

import requests
import hashlib
from packaging.version import Version
from pathlib import Path

JSON_URL = "https://raw.githubusercontent.com/slproweb/opensslhashes/refs/heads/master/win32_openssl_hashes.json"
TARGET_MAJOR_PREFIX = "3."


def get_latest_universal_installer_info():
    response = requests.get(JSON_URL, timeout=30)
    response.raise_for_status()

    data = response.json()
    files = data.get("files", {})

    matched = []
    for filename, meta in files.items():
        basever = meta.get("basever", "")
        if (
            meta.get("arch") == "Universal"
            and meta.get("light") is False
            and meta.get("installer") == "exe"
            and basever.startswith(TARGET_MAJOR_PREFIX)
        ):
            matched.append(
                {
                    "filename": filename,
                    "basever": basever,
                    "url": meta["url"],
                    "sha256": meta["sha256"],
                }
            )

    if not matched:
        raise ValueError(
            f"No files matching your criteria were found for {TARGET_MAJOR_PREFIX}x."
        )

    latest = max(matched, key=lambda x: Version(x["basever"]))
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


if __name__ == "__main__":
    latest = get_latest_universal_installer_info()
    download_path = Path(latest["filename"])
    saved_path = download_file(latest["url"], download_path)
    sha256 = verify_sha256(saved_path, latest["sha256"])

    print(latest["filename"])
