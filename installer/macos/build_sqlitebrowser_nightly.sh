#!/bin/bash

# Builds and uploads DB Browser for SQLite nightlies,
# after updating the Homebrew dependencies

QTVER="5.15.2"
BRANCH="master"
BREW="/usr/local/bin/brew"
BUILD_TYPE="release"
DATE=`date "+%Y%m%d"`
LOG="$HOME/db4s_nightlies/nightly-$DATE.log"
LRELEASE="$HOME/Qt/${QTVER}/clang_64/bin/lrelease"
LUPDATE="$HOME/Qt/${QTVER}/clang_64/bin/lupdate"
MACDEPLOYQT="$HOME/Qt/${QTVER}/clang_64/bin/macdeployqt"
PATH="$PATH:/usr/local/bin:/usr/sbin"
CMAKE="/usr/local/bin/cmake"

# Add the sensitive values we don't want to store in this script file
source ~/.db4s_secure

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
sudo chown -Rh jc:staff /usr/local >>$LOG 2>&1

# Update Homebrew
echo Update Homebrew >>$LOG 2>&1
$BREW update >>$LOG 2>&1

### Build standard version

# Remove any existing Homebrew installed packages
echo Remove any existing Homebrew installed packages >>$LOG 2>&1
$BREW remove `$BREW list --formula` --force >>$LOG 2>&1

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

# Update the translation files
echo Updating the translations >>$LOG 2>&1
$LUPDATE src/src.pro >>$LOG 2>&1
$LRELEASE src/src.pro >>$LOG 2>&1

# Build and package standard sqlitebrowser nightly
echo Build and package standard sqlitebrowser nightly >>$LOG 2>&1
mkdir build >>$LOG 2>&1
cd build >>$LOG 2>&1
if [ "${BUILD_TYPE}" = "debug" ]; then
  $CMAKE -DCMAKE_BUILD_TYPE=Debug .. >>$LOG 2>&1
else
  $CMAKE .. >>$LOG 2>&1
fi
make -j3 >>$LOG 2>&1
cd .. >>$LOG 2>&1
cp -R build/sqlitebrowser.app src/DB\ Browser\ for\ SQLite.app >>$LOG 2>&1

# Include the depencencies in the .app bundle
$MACDEPLOYQT src/DB\ Browser\ for\ SQLite.app -verbose=2 -sign-for-notarization="${DEV_ID}">>$LOG 2>&1

# Add the extensions to the .dmg
echo Add the extensions to the .dmg >>$LOG 2>&1
mkdir src/DB\ Browser\ for\ SQLite.app/Contents/Extensions >>$LOG 2>&1
gcc -I/usr/local/opt/sqlitefts5/include -L/usr/local/opt/sqlitefts5/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
gcc -I/usr/local/opt/sqlitefts5/include -L/usr/local/opt/sqlitefts5/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1
# fileio.c extension
curl -L -o src/extensions/fileio.c 'https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk' >>$LOG 2>&1
curl -L -o src/extensions/test_windirent.c 'https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk' >>$LOG 2>&1
curl -L -o src/extensions/test_windirent.h 'https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk' >>$LOG 2>&1
gcc -I/usr/local/opt/sqlitefts5/include -L/usr/local/opt/sqlitefts5/lib -fno-common -dynamiclib src/extensions/fileio.c src/extensions/test_windirent.c -o src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1

# Copy the license files to the .dmg
echo Copying the license files to the .dmg >>$LOG 2>&1
cp LICENSE LICENSE-PLUGINS src/DB\ Browser\ for\ SQLite.app/Contents/Resources/ >>$LOG 2>&1

# Copy the translation files to the .dmg
mkdir -p src/DB\ Browser\ for\ SQLite.app/Contents/translations >>$LOG 2>&1
for i in ar zh_CN zh_TW cs en fr de it ko pl pt ru es uk; do
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qt_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtbase_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtmultimedia_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtquick1_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtscript_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtxmlpatterns_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
done

# Unlock the local security keychain, so signing can be done
security unlock-keychain -p "${KEYCHAIN_PASSWORD}" "${HOME}/Library/Keychains/login.keychain"

# Sign the added libraries
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1

# Make a .dmg file from the .app
mv src/DB\ Browser\ for\ SQLite.app $HOME/appdmg/ >>$LOG 2>&1
cd $HOME/appdmg >>$LOG 2>&1
appdmg --quiet nightly.json DB\ Browser\ for\ SQLite_${DATE}.dmg >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp DB\ Browser\ for\ SQLite_${DATE}.dmg >>$LOG 2>&1
mv DB\ Browser\ for\ SQLite_${DATE}.dmg $HOME/db4s_nightlies/ >>$LOG 2>&1
rm -rf $HOME/appdmg/DB\ Browser\ for\ SQLite.app >>$LOG 2>&1

### Build SQLCipher version
# Remove any existing Homebrew installed packages
echo Remove any existing Homebrew installed packages >>$LOG 2>&1
$BREW remove `$BREW list --formula` --force >>$LOG 2>&1

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

# Update the translation files
echo Updating the translations >>$LOG 2>&1
$LUPDATE src/src.pro >>$LOG 2>&1
$LRELEASE src/src.pro >>$LOG 2>&1

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
cp -R build/sqlitebrowser.app src/DB\ Browser\ for\ SQLite.app >>$LOG 2>&1

# Unlock the local security keychain, so signing can be done
security unlock-keychain -p "${KEYCHAIN_PASSWORD}" "${HOME}/Library/Keychains/login.keychain"

# Include the depencencies in the .app bundle
$MACDEPLOYQT src/DB\ Browser\ for\ SQLite.app -verbose=2 -sign-for-notarization="${DEV_ID}">>$LOG 2>&1

# Add the extensions to the .dmg
echo Add the extensions to the .dmg >>$LOG 2>&1
mkdir src/DB\ Browser\ for\ SQLite.app/Contents/Extensions >>$LOG 2>&1
gcc -I/usr/local/opt/sqlcipherdb4s/include -L/usr/local/opt/sqlcipherdb4s/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
gcc -I/usr/local/opt/sqlcipherdb4s/include -L/usr/local/opt/sqlcipherdb4s/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1
# fileio.c extension
curl -L -o src/extensions/fileio.c 'https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk' >>$LOG 2>&1
curl -L -o src/extensions/test_windirent.c 'https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk' >>$LOG 2>&1
curl -L -o src/extensions/test_windirent.h 'https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk' >>$LOG 2>&1
gcc -I/usr/local/opt/sqlcipherdb4s/include -L/usr/local/opt/sqlcipherdb4s/lib -fno-common -dynamiclib src/extensions/fileio.c src/extensions/test_windirent.c -o src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1

# Copy the license files to the .dmg
echo Copying the license files to the .dmg >>$LOG 2>&1
cp LICENSE LICENSE-PLUGINS src/DB\ Browser\ for\ SQLite.app/Contents/Resources/ >>$LOG 2>&1

# Copy the translation files to the .dmg
mkdir -p src/DB\ Browser\ for\ SQLite.app/Contents/translations >>$LOG 2>&1
for i in ar zh_CN zh_TW cs en fr de it ko pl pt ru es uk; do
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qt_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtbase_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtmultimedia_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtquick1_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtscript_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
  cp -v $HOME/Qt/${QTVER}/clang_64/translations/qtxmlpatterns_${i}.qm src/DB\ Browser\ for\ SQLite.app/Contents/translations/ >>$LOG 2>&1
done

# Unlock the local security keychain, so signing can be done
security unlock-keychain -p "${KEYCHAIN_PASSWORD}" "${HOME}/Library/Keychains/login.keychain"

# Sign the .app
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --force --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1

# Make a .dmg file from the .app
mv src/DB\ Browser\ for\ SQLite.app $HOME/appdmg/ >>$LOG 2>&1
cd $HOME/appdmg >>$LOG 2>&1
appdmg --quiet nightly.json DB\ Browser\ for\ SQLite-sqlcipher_${DATE}.dmg >>$LOG 2>&1
codesign --sign "${DEV_ID}" --verbose --deep --keychain "/Library/Keychains/System.keychain" --options runtime --timestamp DB\ Browser\ for\ SQLite-sqlcipher_${DATE}.dmg >>$LOG 2>&1
mv DB\ Browser\ for\ SQLite-sqlcipher_${DATE}.dmg $HOME/db4s_nightlies/ >>$LOG 2>&1
rm -rf $HOME/appdmg/DB\ Browser\ for\ SQLite.app >>$LOG 2>&1

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
rsync -aP $HOME/db4s_nightlies/DB\ Browser\ for\ SQLite*${DATE}.dmg ${LOG} ${UPLOAD_SERVER}:/nightlies/osx/ >>$LOG 2>&1
ssh -q ${UPLOAD_SERVER} "cd /nightlies/latest; rm -f *dmg" >>$LOG 2>&1
ssh -q ${UPLOAD_SERVER} "cd /nightlies/latest; cp /nightlies/osx/DB\ Browser\ for\ SQLite_${DATE}.dmg /nightlies/latest/DB.Browser.for.SQLite.dmg" >>$LOG 2>&1
ssh -q ${UPLOAD_SERVER} "cd /nightlies/latest; cp /nightlies/osx/DB\ Browser\ for\ SQLite-sqlcipher_${DATE}.dmg /nightlies/latest/DB.Browser.for.SQLite-sqlcipher.dmg" >>$LOG 2>&1

echo Done! >>$LOG 2>&1
