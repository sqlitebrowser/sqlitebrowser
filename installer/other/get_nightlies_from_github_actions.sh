# SPDX-FileCopyrightText: (C) 2024 SeongTae Jeong <seongtaejg@gmail.com>
# This script downloads the daily build output from GitHub Release, built by GitHub Actions, and archives it on our nightly server.

#!/usr/bin/env bash

source /root/.gh_token_secure

set -ex

echo "$(TZ=UTC date +"%Y-%m-%d %H:%M:%S %Z"): [START]"
echo "Clear the incoming directory"
DOWNLOAD_DIR="/tmp/incoming/"
rm -rfv $DOWNLOAD_DIR
mkdir -v $DOWNLOAD_DIR

if [ $(ls -l /nightlies/macos-universal | grep -c "$(date +%Y%m%d)") ] &&
   [ $(ls -l /nightlies/win32 /nightlies/win64 | grep -c "$(date +%Y-%m-%d)") -ne 0 ]; then
    echo "Nightly build already exists"
    exit 1
fi

if ! gh auth login --with-token <<< "$GH_TOKEN"; then
    echo "Unable to authenticate with GitHub"
fi
echo "Successfully authenticated with GitHub"

IS_BUILD_SUCCESS=$(gh run list --created $(date '+%Y-%m-%d') --limit 1 --status "success" --workflow "cppcmake-nightly.yml" --repo "sqlitebrowser/sqlitebrowser" | wc -l)
if [ $IS_BUILD_SUCCESS -eq 0 ]; then
    echo "No successful build found"
    exit 1
fi
echo "Found a successful build"

if ! gh release download --dir /tmp/incoming/ -R "sqlitebrowser/sqlitebrowser" nightly; then
    echo "Unable to download the nightly build"
fi
echo "Successfully downloaded the nightly build"


# Check if the downloaded files are as expected
# This case is occuring when the nightly build is skipped
# for AppImage, macOS Binaries
if [ $(ls -l $DOWNLOAD_DIR | grep -c "$(date +%Y%m%d)") -ne 4 ]; then
    echo "Last nightly build is skipped"
    exit 1
fi
# for Windows Binaries
if [ $(ls -l $DOWNLOAD_DIR | grep -c "$(date +%Y-%m-%d)") -ne 4 ]; then
    echo "Last nightly build is skipped"
    exit 1
fi

mv -v $DOWNLOAD_DIR*AppImage /nightlies/appimage/
mv -v $DOWNLOAD_DIR*win32* /nightlies/win32/
mv -v $DOWNLOAD_DIR*win64* /nightlies/win64/
mv -v $DOWNLOAD_DIR*dmg /nightlies/macos-universal/

rm -v /nightlies/latest/*.AppImage
rm -v /nightlies/latest/*.dmg
rm -v /nightlies/latest/*.msi
rm -v /nightlies/latest/*.zip

DATE=$(date +%Y%m%d)
ln -sv /nightlies/appimage/DB.Browser.for.SQLCipher-$DATE.AppImage /nightlies/latest/DB.Browser.for.SQLCipher.AppImage
ln -sv /nightlies/appimage/DB.Browser.for.SQLite-$DATE.AppImage /nightlies/latest/DB.Browser.for.SQLite.AppImage
ln -sv /nightlies/macos-universal/DB.Browser.for.SQLCipher-universal_$DATE.dmg /nightlies/latest/DB.Browser.for.SQLCipher-universal.dmg
ln -sv /nightlies/macos-universal/DB.Browser.for.SQLite-universal_$DATE.dmg /nightlies/latest/DB.Browser.for.SQLite-universal.dmg
DATE=$(date +%Y-%m-%d)
ln -sv /nightlies/win32/DB.Browser.for.SQLite-$DATE-win32.msi /nightlies/latest/DB.Browser.for.SQLite-win32.msi
ln -sv /nightlies/win32/DB.Browser.for.SQLite-$DATE-win32.zip /nightlies/latest/DB.Browser.for.SQLite-win32.zip
ln -sv /nightlies/win64/DB.Browser.for.SQLite-$DATE-win64.msi /nightlies/latest/DB.Browser.for.SQLite-win64.msi
ln -sv /nightlies/win64/DB.Browser.for.SQLite-$DATE-win64.zip /nightlies/latest/DB.Browser.for.SQLite-win64.zip

echo "[STOP]"
