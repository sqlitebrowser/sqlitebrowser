#!/bin/bash

# Builds and uploads DB Browser for SQLite nightlies,
# after updating the Homebrew dependencies

BRANCH="master"
BREW="/opt/homebrew/bin/brew"
BUILD_TYPE="release"
CMAKE="/opt/homebrew/bin/cmake"
DATE=`date "+%Y%m%d"`
LOG="$HOME/db4s_nightlies/nightly-$DATE.log"
PATH="$PATH:/opt/homebrew/bin:/usr/sbin"
QT5_DIR="/opt/homebrew/opt/qt5"
MACDEPLOYQT="$QT5_DIR/bin/macdeployqt"

# Add the sensitive values we don't want to store in this script file
source ~/.db4s_secure

# Load NVM, for appdmg to work
export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"

# Update the branch to build, if specified on the command line with "-b [branch name]"
if [ "$1" = "-b" ]; then
  if [ ! -z "$2" ]; then
    BRANCH="$2"
    echo "New branch = ${BRANCH}" >>$LOG 2>&1
  else
    # Warn on missing branch name override
    echo "Missing branch name after the '-b'"
    exit 1
  fi
fi

# Determine if a Release or Debug build is to be built
if [ "$3" = "-t" ]; then
  if [ ! -z "$4" ]; then
    BUILD_TYPE="$4"
    echo "Build type = ${BUILD_TYPE}" >>$LOG 2>&1
  else
    # Warn on missing build type override
    echo "Missing build type after the '-t'"
    exit 2
  fi
fi

# Verify the build type is valid
if [ "${BUILD_TYPE}" != "debug" -a "${BUILD_TYPE}" != "release" ]; then
  echo "Unknown build type"
  exit 3
fi

# Ensure Homebrew is owned by my user
echo Ensure Homebrew is owned by my user >>$LOG 2>&1
sudo chown -Rh $USER: /opt/homebrew >>$LOG 2>&1

# Update Homebrew
echo Update Homebrew >>$LOG 2>&1
$BREW update >>$LOG 2>&1

### Build standard version

# Remove any existing Homebrew installed packages
echo Remove any existing Homebrew installed packages >>$LOG 2>&1
$BREW remove `$BREW list --formula` --force >>$LOG 2>&1

# Install CMake
$BREW install --formula cmake >>$LOG 2>&1

# Install Qt5
$BREW install --formula qt5 >>$LOG 2>&1

# Install SQLite3 
# Note - `brew tap sqlitebrowser/homebrew-sqlite3` needs to have been run at least once (manually) first
echo Install SQLite3 >>$LOG 2>&1
$BREW install sqlitefts5 >>$LOG 2>&1

# Update the sqlitebrowser source
echo Update the sqlitebrowser source >>$LOG 2>&1
cd $HOME/git_repos/sqlitebrowser >>$LOG 2>&1
git reset --hard HEAD >>$LOG 2>&1
git clean -dffx >>$LOG 2>&1
git pull >>$LOG 2>&1
git checkout $BRANCH >>$LOG 2>&1
git reset --hard HEAD >>$LOG 2>&1
git clean -dffx >>$LOG 2>&1
git pull >>$LOG 2>&1

# Build and package standard sqlitebrowser nightly
echo Build and package standard sqlitebrowser nightly >>$LOG 2>&1
mkdir build >>$LOG 2>&1
cd build >>$LOG 2>&1
if [ "${BUILD_TYPE}" = "debug" ]; then
  $CMAKE -DCMAKE_BUILD_TYPE=Debug .. >>$LOG 2>&1
else
  $CMAKE .. >>$LOG 2>&1
fi
make -j9 >>$LOG 2>&1
cd .. >>$LOG 2>&1

# Include the depencencies in the .app bundle
$MACDEPLOYQT build/DB\ Browser\ for\ SQLite.app -verbose=2 -sign-for-notarization="${DEV_ID}">>$LOG 2>&1

# Add the extensions to the .dmg
echo Add the extensions to the .dmg >>$LOG 2>&1
mkdir build/DB\ Browser\ for\ SQLite.app/Contents/Extensions >>$LOG 2>&1
clang -I/opt/homebrew/opt/sqlitefts5/include -L/opt/homebrew/opt/sqlitefts5/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
install_name_tool -id "@executable_path/../Extensions/formats.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
ln -s formats.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib.dylib >>$LOG 2>&1
clang -I/opt/homebrew/opt/sqlitefts5/include -L/opt/homebrew/opt/sqlitefts5/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1
install_name_tool -id "@executable_path/../Extensions/math.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1
ln -s math.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib.dylib >>$LOG 2>&1
# fileio.c extension
curl -L -o src/extensions/fileio.c 'https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk' >>$LOG 2>&1
curl -L -o src/extensions/test_windirent.c 'https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk' >>$LOG 2>&1
curl -L -o src/extensions/test_windirent.h 'https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk' >>$LOG 2>&1
clang -I/opt/homebrew/opt/sqlitefts5/include -L/opt/homebrew/opt/sqlitefts5/lib -fno-common -dynamiclib src/extensions/fileio.c src/extensions/test_windirent.c -o build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
install_name_tool -id "@executable_path/../Extensions/fileio.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
ln -s fileio.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib.dylib >>$LOG 2>&1

# Copy the license files to the .dmg
echo Copying the license files to the .dmg >>$LOG 2>&1
cp LICENSE LICENSE-PLUGINS build/DB\ Browser\ for\ SQLite.app/Contents/Resources/ >>$LOG 2>&1

# Copy the translation files to the .dmg
mkdir -p build/DB\ Browser\ for\ SQLite.app/Contents/translations >>$LOG 2>&1
for i in ar zh_CN zh_TW cs en fr de it ko pl pt pt_BR ru es uk; do
  cp -v $QT5_DIR/translations/qt_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtbase_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtmultimedia_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtquick1_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtscript_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtxmlpatterns_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
done

# Add the icon file
cp installer/macos/macapp-nightly.icns build/DB\ Browser\ for\ SQLite.app/Contents/Resources/ >>$LOG 2>&1
/usr/libexec/PlistBuddy -c "Set :CFBundleIconFile macapp-nightly.icns" build/DB\ Browser\ for\ SQLite.app/Contents/Info.plist >>$LOG 2>&1

# Rename the nightly build
mv build/DB\ Browser\ for\ SQLite.app build/DB\ Browser\ for\ SQLite\ Nightly.app >>$LOG 2>&1

# Sign the manually added extensions.  Needs to be done prior to the ".app signing" bit below, as that doesn't seem to sign these... which results in notarisation failure later on
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/math.dylib >>$LOG 2>&1

# Sign the app (again).  Needs to be done after the extensions are manually signed (above), else notarisation fails
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app >>$LOG 2>&1

# Make a .dmg file from the .app
mv build/DB\ Browser\ for\ SQLite\ Nightly.app $HOME/appdmg/ >>$LOG 2>&1
cd $HOME/appdmg >>$LOG 2>&1
appdmg --quiet nightly.json DB\ Browser\ for\ SQLite-arm64_${DATE}.dmg >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp DB\ Browser\ for\ SQLite-arm64_${DATE}.dmg >>$LOG 2>&1

# Notarise the .app
security unlock-keychain -p "${KEYCHAIN_PASSWORD}" db4s.keychain
xcrun notarytool submit --apple-id "${APPLE_ID}" --team-id "${TEAM_ID}" --wait --keychain-profile "${PROFILE_NAME}" --keychain /Users/jc/Library/Keychains/db4s.keychain-db DB\ Browser\ for\ SQLite-arm64_${DATE}.dmg >>$LOG 2>&1
xcrun stapler staple DB\ Browser\ for\ SQLite-arm64_${DATE}.dmg >>$LOG 2>&1
mv DB\ Browser\ for\ SQLite-arm64_${DATE}.dmg $HOME/db4s_nightlies/ >>$LOG 2>&1
rm -rf $HOME/appdmg/DB\ Browser\ for\ SQLite\ Nightly.app >>$LOG 2>&1

### Build SQLCipher version
# Remove any existing Homebrew installed packages
echo Remove any existing Homebrew installed packages >>$LOG 2>&1
$BREW remove `$BREW list --formula` --force >>$LOG 2>&1

# Install CMake
$BREW install --formula cmake >>$LOG 2>&1

# Install Qt5
$BREW install --formula qt5 >>$LOG 2>&1

# Install SQLCipher
echo Install SQLCipher >>$LOG 2>&1
$BREW install sqlcipherdb4s >>$LOG 2>&1

# Clean the sqlitebrowser source
echo Clean the sqlitebrowser source >>$LOG 2>&1
cd $HOME/git_repos/sqlitebrowser >>$LOG 2>&1
git reset --hard HEAD >>$LOG 2>&1
git clean -dffx >>$LOG 2>&1
git checkout $BRANCH >>$LOG 2>&1
git reset --hard HEAD >>$LOG 2>&1
git clean -dffx >>$LOG 2>&1

# Build and package sqlitebrowser with SQLCipher support
echo Build and package sqlitebrowser with SQLCipher support >>$LOG 2>&1
mkdir build >>$LOG 2>&1
cd build >>$LOG 2>&1
if [ "${BUILD_TYPE}" = "debug" ]; then
  $CMAKE -DCMAKE_BUILD_TYPE=Debug -Dsqlcipher=1 .. >>$LOG 2>&1
else
  $CMAKE -Dsqlcipher=1 .. >>$LOG 2>&1
fi
make -j3 >>$LOG 2>&1
cd .. >>$LOG 2>&1

# Include the depencencies in the .app bundle
$MACDEPLOYQT build/DB\ Browser\ for\ SQLite.app -verbose=2 -sign-for-notarization="${DEV_ID}">>$LOG 2>&1

# Add the extensions to the .dmg
echo Add the extensions to the .dmg >>$LOG 2>&1
mkdir build/DB\ Browser\ for\ SQLite.app/Contents/Extensions >>$LOG 2>&1
clang -I/opt/homebrew/opt/sqlcipherdb4s/include -L/opt/homebrew/opt/sqlcipherdb4s/lib -fno-common -dynamiclib -lsqlcipher src/extensions/extension-formats.c -o build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
install_name_tool -id "@executable_path/../Extensions/formats.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
install_name_tool -change "/opt/homebrew/opt/sqlcipherdb4s/lib/libsqlcipher.0.dylib" "@executable_path/../Frameworks/libsqlcipher.0.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
ln -s formats.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib.dylib >>$LOG 2>&1
clang -I/opt/homebrew/opt/sqlcipherdb4s/include -L/opt/homebrew/opt/sqlcipherdb4s/lib -fno-common -dynamiclib -lsqlcipher src/extensions/extension-functions.c -o build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1
install_name_tool -id "@executable_path/../Extensions/math.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1
install_name_tool -change "/opt/homebrew/opt/sqlcipherdb4s/lib/libsqlcipher.0.dylib" "@executable_path/../Frameworks/libsqlcipher.0.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1
ln -s math.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib.dylib >>$LOG 2>&1
# fileio.c extension
curl -L -o src/extensions/fileio.c 'https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk' >>$LOG 2>&1
curl -L -o src/extensions/test_windirent.c 'https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk' >>$LOG 2>&1
curl -L -o src/extensions/test_windirent.h 'https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk' >>$LOG 2>&1
clang -I/opt/homebrew/opt/sqlcipherdb4s/include -L/opt/homebrew/opt/sqlcipherdb4s/lib -fno-common -dynamiclib -lsqlcipher src/extensions/fileio.c src/extensions/test_windirent.c -o build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
install_name_tool -id "@executable_path/../Extensions/fileio.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
install_name_tool -change "/opt/homebrew/opt/sqlcipherdb4s/lib/libsqlcipher.0.dylib" "@executable_path/../Frameworks/libsqlcipher.0.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
ln -s fileio.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib.dylib >>$LOG 2>&1

# Copy the license files to the .dmg
echo Copying the license files to the .dmg >>$LOG 2>&1
cp LICENSE LICENSE-PLUGINS build/DB\ Browser\ for\ SQLite.app/Contents/Resources/ >>$LOG 2>&1

# Copy the translation files to the .dmg
mkdir -p build/DB\ Browser\ for\ SQLite.app/Contents/translations >>$LOG 2>&1
for i in ar zh_CN zh_TW cs en fr de it ko pl pt pt_BR ru es uk; do
  cp -v $QT5_DIR/translations/qt_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtbase_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtmultimedia_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtquick1_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtscript_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $QT5_DIR/translations/qtxmlpatterns_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
done

# Add the icon file
cp installer/macos/macapp-nightly.icns build/DB\ Browser\ for\ SQLite.app/Contents/Resources/ >>$LOG 2>&1
/usr/libexec/PlistBuddy -c "Set :CFBundleIconFile macapp-nightly.icns" build/DB\ Browser\ for\ SQLite.app/Contents/Info.plist >>$LOG 2>&1

# Rename the nightly build
mv build/DB\ Browser\ for\ SQLite.app build/DB\ Browser\ for\ SQLite\ Nightly.app >>$LOG 2>&1

# Sign the manually added extensions.  Needs to be done prior to the ".app signing" bit below, as that doesn't seem to sign these... which results in notarisation failure later on
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/math.dylib >>$LOG 2>&1

# Sign the app (again).  Needs to be done after the extensions are manually signed (above), else notarisation fails
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app >>$LOG 2>&1

# Make a .dmg file from the .app
mv build/DB\ Browser\ for\ SQLite\ Nightly.app $HOME/appdmg/ >>$LOG 2>&1
cd $HOME/appdmg >>$LOG 2>&1
appdmg --quiet nightly.json DB\ Browser\ for\ SQLite-sqlcipher-arm64_${DATE}.dmg >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp DB\ Browser\ for\ SQLite-sqlcipher-arm64_${DATE}.dmg >>$LOG 2>&1

# Notarise the .app
security unlock-keychain -p "${KEYCHAIN_PASSWORD}" db4s.keychain
xcrun notarytool submit --apple-id "${APPLE_ID}" --team-id "${TEAM_ID}" --wait --keychain-profile "${PROFILE_NAME}" --keychain /Users/jc/Library/Keychains/db4s.keychain-db DB\ Browser\ for\ SQLite-sqlcipher-arm64_${DATE}.dmg >>$LOG 2>&1
xcrun stapler staple DB\ Browser\ for\ SQLite-sqlcipher-arm64_${DATE}.dmg >>$LOG 2>&1
mv DB\ Browser\ for\ SQLite-sqlcipher-arm64_${DATE}.dmg $HOME/db4s_nightlies/ >>$LOG 2>&1
rm -rf $HOME/appdmg/DB\ Browser\ for\ SQLite\ Nightly.app >>$LOG 2>&1

# If building a non-master branch, remove it now that we're finished
if [ "${BRANCH}" != "master" ]; then
  echo "Removing non-master branch, now we're finihed" >>$LOG 2>&1
  cd $HOME/git_repos/sqlitebrowser >>$LOG 2>&1
  git reset --hard HEAD >>$LOG 2>&1
  git clean -dffx >>$LOG 2>&1
  git checkout master >>$LOG 2>&1
  git branch -D "${BRANCH}" >>$LOG 2>&1
fi

# Upload nightly builds and the build log thus far
echo Upload nightly builds >>$LOG 2>&1
rsync -aP $HOME/db4s_nightlies/DB\ Browser\ for\ SQLite*${DATE}.dmg ${LOG} ${UPLOAD_SERVER}:/nightlies/macos-arm64/ >>$LOG 2>&1
ssh -q ${UPLOAD_SERVER} "cd /nightlies/latest; rm -f *arm64*dmg" >>$LOG 2>&1
ssh -q ${UPLOAD_SERVER} "cd /nightlies/latest; cp /nightlies/macos-arm64/DB\ Browser\ for\ SQLite-arm64_${DATE}.dmg /nightlies/latest/DB.Browser.for.SQLite-arm64.dmg" >>$LOG 2>&1
ssh -q ${UPLOAD_SERVER} "cd /nightlies/latest; cp /nightlies/macos-arm64/DB\ Browser\ for\ SQLite-sqlcipher-arm64_${DATE}.dmg /nightlies/latest/DB.Browser.for.SQLite-sqlcipher-arm64.dmg" >>$LOG 2>&1

echo Done! >>$LOG 2>&1
