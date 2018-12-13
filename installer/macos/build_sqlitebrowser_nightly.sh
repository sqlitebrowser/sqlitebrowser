#!/bin/bash

# Builds and uploads DB Browser for SQLite nightlies,
# after updating the Homebrew dependencies

QTVER="5.12.0"
BRANCH="v3.11.x"
BREW="/usr/local/bin/brew"
BUILD_TYPE="release"
DATE=`date "+%Y%m%d"`
LOG="$HOME/db4s_nightlies/nightly.log-$DATE"
LRELEASE="$HOME/Qt/${QTVER}/clang_64/bin/lrelease"
LUPDATE="$HOME/Qt/${QTVER}/clang_64/bin/lupdate"
MACDEPLOYQT="$HOME/Qt/${QTVER}/clang_64/bin/macdeployqt"
PATH="$PATH:/usr/local/bin:/usr/sbin"
QMAKE="$HOME/Qt/${QTVER}/clang_64/bin/qmake"

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

# Ensure the temporary SQLCipher v3.4.2 tap is in place
echo Ensure the temporary SQLCipher v3.4.2 tap is in place >>$LOG 2>&1
brew tap justinclift/sqlcipher3 >>$LOG 2>&1

# Update Homebrew
echo Update Homebrew >>$LOG 2>&1
$BREW update >>$LOG 2>&1

### Build standard version

# Remove any existing Homebrew installed packages
echo Remove any existing Homebrew installed packages >>$LOG 2>&1
$BREW remove `$BREW list` --force >>$LOG 2>&1

# Install SQLite3 
echo Install SQLite3 >>$LOG 2>&1
$BREW install sqlite --with-functions --with-json1 --with-fts --with-fts5 >>$LOG 2>&1
$BREW link sqlite3 --force >>$LOG 2>&1

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
if [ "${BUILD_TYPE}" = "debug" ]; then
  $QMAKE sqlitebrowser.pro -r -spec macx-clang CONFIG+=debug CONFIG+=x86_64 >>$LOG 2>&1
else
  $QMAKE sqlitebrowser.pro -r -spec macx-clang CONFIG+=x86_64 >>$LOG 2>&1
fi
make -j3 >>$LOG 2>&1
make -j3 >>$LOG 2>&1 # Seems to need a 2nd time now, due to language files needing initialisation or something

# Include the depencencies in the .app bundle
$MACDEPLOYQT src/DB\ Browser\ for\ SQLite.app -verbose=2 >>$LOG 2>&1

# Add the math extensions to the .dmg
echo Add the math extensions to the .dmg >>$LOG 2>&1
mkdir src/DB\ Browser\ for\ SQLite.app/Contents/Extensions >>$LOG 2>&1
gcc -I/usr/local/opt/sqlite/include -L/usr/local/opt/sqlite/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1

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
codesign --sign "${DEV_ID}" --verbose --deep --keychain "/Library/Keychains/System.keychain" src/DB\ Browser\ for\ SQLite.app >>$LOG 2>&1

# Make a .dmg file from the .app
mv src/DB\ Browser\ for\ SQLite.app $HOME/appdmg/ >>$LOG 2>&1
cd $HOME/appdmg >>$LOG 2>&1
$BREW install npm >>$LOG 2>&1
npm -g update >>$LOG 2>&1
npm -g upgrade >>$LOG 2>&1
npm -g uninstall appdmg >>$LOG 2>&1
npm -g install appdmg >>$LOG 2>&1
appdmg --quiet nightly.json DB\ Browser\ for\ SQLite_${DATE}.dmg >>$LOG 2>&1
mv DB\ Browser\ for\ SQLite_${DATE}.dmg $HOME/db4s_nightlies/ >>$LOG 2>&1
$BREW unlink sqlite3 >>$LOG 2>&1
rm -rf $HOME/appdmg/DB\ Browser\ for\ SQLite.app >>$LOG 2>&1

### Build SQLCipher version
# Remove any existing Homebrew installed packages
echo Remove any existing Homebrew installed packages >>$LOG 2>&1
$BREW remove `$BREW list` --force >>$LOG 2>&1

# Install sqlcipher
echo Install sqlcipher >>$LOG 2>&1
$BREW install sqlcipher3 --with-fts >>$LOG 2>&1

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
if [ "${BUILD_TYPE}" = "debug" ]; then
  $QMAKE sqlitebrowser.pro -r -spec macx-clang CONFIG+=debug CONFIG+=x86_64 CONFIG+=sqlcipher >>$LOG 2>&1
else
  $QMAKE sqlitebrowser.pro -r -spec macx-clang CONFIG+=x86_64 CONFIG+=sqlcipher >>$LOG 2>&1
fi
make -j3 >>$LOG 2>&1
make -j3 >>$LOG 2>&1 # Seems to need a 2nd time now, due to language files needing initialisation or something

# Include the depencencies in the .app bundle
$MACDEPLOYQT src/DB\ Browser\ for\ SQLite.app -verbose=2 >>$LOG 2>&1

# Add the math extensions to the .dmg
echo Add the math extensions to the .dmg >>$LOG 2>&1
mkdir src/DB\ Browser\ for\ SQLite.app/Contents/Extensions >>$LOG 2>&1
gcc -I/usr/local/opt/sqlite/include -L/usr/local/opt/sqlite/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o src/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib >>$LOG 2>&1

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
codesign --sign "${DEV_ID}" --verbose --deep --keychain "/Library/Keychains/System.keychain" src/DB\ Browser\ for\ SQLite.app >>$LOG 2>&1

# Make a .dmg file from the .app
mv src/DB\ Browser\ for\ SQLite.app $HOME/appdmg/ >>$LOG 2>&1
cd $HOME/appdmg >>$LOG 2>&1
$BREW install npm >>$LOG 2>&1
npm -g update >>$LOG 2>&1
npm -g upgrade >>$LOG 2>&1
npm -g uninstall appdmg >>$LOG 2>&1
npm -g install appdmg >>$LOG 2>&1
appdmg --quiet nightly.json DB\ Browser\ for\ SQLite-sqlcipher_${DATE}.dmg >>$LOG 2>&1
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
rsync -aP $HOME/db4s_nightlies/DB\ Browser\ for\ SQLite*${DATE}.dmg $HOME/db4s_nightlies/nightly.log-${DATE} ${UPLOAD_SERVER}:/nightlies/osx/ >>$LOG 2>&1

# Add the new builds to the "latest" directory
ssh ${UPLOAD_SERVER} "cd /nightlies/latest; rm -f *dmg" >>$LOG 2>&1
ssh ${UPLOAD_SERVER} "cd /nightlies/latest; cp /nightlies/osx/DB\ Browser\ for\ SQLite_${DATE}.dmg /nightlies/latest/DB.Browser.for.SQLite.dmg" >>$LOG 2>&1
ssh ${UPLOAD_SERVER} "cd /nightlies/latest; cp /nightlies/osx/DB\ Browser\ for\ SQLite-sqlcipher_${DATE}.dmg /nightlies/latest/DB.Browser.for.SQLite-sqlcipher.dmg" >>$LOG 2>&1

echo Done! >>$LOG 2>&1
