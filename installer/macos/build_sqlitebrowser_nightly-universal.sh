#!/usr/bin/env bash

BRANCH="master"
BUILD_TYPE="release"
DATE=`date "+%Y%m%d"`

# Determine if a Release or Debug build is to be built
if [ "$3" = "-t" ]; then
	if [ ! -z "$4" ]; then
		BUILD_TYPE="$4"
		echo "Build type = ${BUILD_TYPE}"
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

# Add the sensitive secret values to the env for notarization
source $HOME/.db4s_secure

# Update the sqlitebrowser source code
echo "> Update the sqlitebrowser source code"
cd $HOME/git_repos/sqlitebrowser
git fetch
git reset --hard "origin/$BRANCH"
git clean -dffx

# Update the name of the created .app
echo "> Update the name of the created .app"
sed -i "" 's/"DB Browser for SQLite"/"DB Browser for SQLite Nightly"/' CMakeLists.txt

# Build and package standard sqlitebrowser nightly
echo "> Build and package standard sqlitebrowser nightly"
mkdir build && cd build
if [ "${BUILD_TYPE}" = "debug" ]; then
	cmake -G Ninja -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DcustomTap=1 -DCMAKE_BUILD_TYPE=Debug ..
else
	cmake -G Ninja -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DcustomTap=1 ..
fi
ninja
cd ../

# Inlcude the dependencies in the .app bundle
echo "> Include the dependencies in the .app bundle"
/opt/homebrew/opt/db4subqt@5/bin/macdeployqt build/DB\ Browser\ for\ SQLite\ Nightly.app -verbose=2 -sign-for-notarization=$DEV_ID

# Add the extensions to the .dmg
echo "> Add the extensions to the .dmg"
mkdir build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions
clang -I /opt/homebrew/opt/db4subsqlitefts@5/include -L /opt/homebrew/opt/db4subsqlitefts@5/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/formats.dylib
install_name_tool -id "@executable_path/../Extensions/formats.dylib" build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/formats.dylib
ln -s formats.dylib build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/formats.dylib.dylib

clang -I /opt/homebrew/opt/db4subsqlitefts@5/include -L /opt/homebrew/opt/db4subsqlitefts@5/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/math.dylib
install_name_tool -id "@executable_path/../Extensions/math.dylib" build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/math.dylib
ln -s math.dylib build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/math.dylib.dylib

curl -L -o src/extensions/fileio.c 'https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk'
curl -L -o src/extensions/test_windirect.c 'https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk'
curl -L -o src/extensions/test_windirect.h 'https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk'
clang -I /opt/homebrew/opt/db4subsqlitefts@5/include -L /opt/homebrew/opt/db4subsqlitefts@5/lib -fno-common -dynamiclib src/extensions/fileio.c src/extensions/test_windirect.c -o build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/fileio.dylib
install_name_tool -id "@executable_path/../Extensions/fileio.dylib" build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/fileio.dylib
ln -s fileio.dylib build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/fileio.dylib.dylib

# Copy the license file to the .dmg
echo "> Copy the license file to the .dmg"
cp LICENSE LICENSE-PLUGINS build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Resources/

# Copy the translations files to the .dmg
echo "> Copy the translations files to the .dmg"
for i in ar cs de en es fr it ko pl pt pt_BR ru uk zh_CN zh_TW; do
    find /opt/homebrew/opt/db4subqt@5/translations -name "qt_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/translations/ \;
    find /opt/homebrew/opt/db4subqt@5/translations -name "qtbase_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/translations/ \;
    find /opt/homebrew/opt/db4subqt@5/translations -name "qtmultimedia_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/translations/ \;
    find /opt/homebrew/opt/db4subqt@5/translations -name "qtscript_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/translations/ \;
    find /opt/homebrew/opt/db4subqt@5/translations -name "qtxmlpatterns_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/translations/ \;
done 

# Add the icon file
echo "> Add the icon file"
cp installer/macos/macapp-nightly.icns build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Resources/
/usr/libexec/PlistBuddy -c "Set :CFBundleIconFile macapp-nightly.icns" build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Info.plist

# Sign the manually added extensions
echo "> Sign the manually added extensions"
codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/fileio.dylib
codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/formats.dylib
codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app/Contents/Extensions/math.dylib
codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp build/DB\ Browser\ for\ SQLite\ Nightly.app

# Move the app bundle to installer folder for DMG creation
echo "> Move the app bundle to installer folder for DMG creation"
mv build/DB\ Browser\ for\ SQLite\ Nightly.app installer/macos
cd installer/macos

# Create DMG
echo "> Create DMG"
appdmg --quiet nightly.json DB\ Browser\ for\ SQLite\ Nightly-universal_$DATE.dmg
codesign --sign "$DEV_ID" --verbose --options=runtime --timestamp DB\ Browser\ for\ SQLite\ Nightly-universal_$DATE.dmg
codesign -vvv --deep --strict --verbose=4 DB\ Browser\ for\ SQLite\ Nightly-universal_$DATE.dmg

# Notarize the DMG
echo "> Notarize the DMG"
xcrun notarytool submit DB\ Browser\ for\ SQLite\ Nightly-universal_$DATE.dmg --apple-id "$APPLE_ID" --password "$APPLE_PW" --team-id "$TEAM_ID" --wait

# Staple the notarization ticket
echo "> Staple the notarization ticket"
xcrun stapler staple DB\ Browser\ for\ SQLite\ Nightly-universal_$DATE.dmg

echo ">> Let's build the SQLCipher version of DB Browser for SQLite"
# Clean the existing source tree
cd $HOME/git_repos/sqlitebrowser
git reset --hard "origin/$BRANCH"
git clean -dffx

# Update the name of the created .app
echo "> Update the name of the created .app"
sed -i "" 's/"DB Browser for SQLite"/"DB Browser for SQLCipher Nightly"/' CMakeLists.txt

# Build and package standard sqlitebrowser nightly
echo "> Build and package standard sqlitebrowser nightly"
mkdir build && cd build
if [ "${BUILD_TYPE}" = "debug" ]; then
	cmake -G Ninja -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DcustomTap=1 -DCMAKE_BUILD_TYPE=Debug -Dsqlcipher=1 ..
else
	cmake -G Ninja -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DcustomTap=1 -Dsqlcipher=1 ..
fi
ninja
cd ../

# Inlcude the dependencies in the .app bundle
echo "> Include the dependencies in the .app bundle"
/opt/homebrew/opt/db4subqt@5/bin/macdeployqt build/DB\ Browser\ for\ SQLCipher\ Nightly.app -verbose=2 -sign-for-notarization=$DEV_ID

# Add the extensions to the .dmg
echo "> Add the extensions to the .dmg"
mkdir build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions
clang -I /opt/homebrew/opt/db4subsqlitefts@5/include -L /opt/homebrew/opt/db4subsqlitefts@5/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/formats.dylib
install_name_tool -id "@executable_path/../Extensions/formats.dylib" build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/formats.dylib
ln -s formats.dylib build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/formats.dylib.dylib

clang -I /opt/homebrew/opt/db4subsqlitefts@5/include -L /opt/homebrew/opt/db4subsqlitefts@5/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/math.dylib
install_name_tool -id "@executable_path/../Extensions/math.dylib" build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/math.dylib
ln -s math.dylib build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/math.dylib.dylib

curl -L -o src/extensions/fileio.c 'https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk'
curl -L -o src/extensions/test_windirect.c 'https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk'
curl -L -o src/extensions/test_windirect.h 'https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk'
clang -I /opt/homebrew/opt/db4subsqlitefts@5/include -L /opt/homebrew/opt/db4subsqlitefts@5/lib -fno-common -dynamiclib src/extensions/fileio.c src/extensions/test_windirect.c -o build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/fileio.dylib
install_name_tool -id "@executable_path/../Extensions/fileio.dylib" build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/fileio.dylib
ln -s fileio.dylib build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/fileio.dylib.dylib

# Copy the license file to the .dmg
echo "> Copy the license file to the .dmg"
cp LICENSE LICENSE-PLUGINS build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Resources/

# Copy the translations files to the .dmg
echo "> Copy the translations files to the .dmg"
for i in ar cs de en es fr it ko pl pt pt_BR ru uk zh_CN zh_TW; do
    find /opt/homebrew/opt/db4subqt@5/translations -name "qt_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/translations/ \;
    find /opt/homebrew/opt/db4subqt@5/translations -name "qtbase_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/translations/ \;
    find /opt/homebrew/opt/db4subqt@5/translations -name "qtmultimedia_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/translations/ \;
    find /opt/homebrew/opt/db4subqt@5/translations -name "qtscript_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/translations/ \;
    find /opt/homebrew/opt/db4subqt@5/translations -name "qtxmlpatterns_${i}.qm" 2> /dev/null -exec cp -v {} build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/translations/ \;
done 

# Add the icon file
echo "> Add the icon file"
cp installer/macos/macapp-nightly.icns build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Resources/
/usr/libexec/PlistBuddy -c "Set :CFBundleIconFile macapp-nightly.icns" build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Info.plist

# Sign the manually added extensions
echo "> Sign the manually added extensions"
codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/fileio.dylib
codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/formats.dylib
codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp build/DB\ Browser\ for\ SQLCipher\ Nightly.app/Contents/Extensions/math.dylib
codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp build/DB\ Browser\ for\ SQLCipher\ Nightly.app

# Move the app bundle to installer folder for DMG creation
echo "> Move the app bundle to installer folder for DMG creation"
mv build/DB\ Browser\ for\ SQLCipher\ Nightly.app installer/macos
cd installer/macos

# Create DMG
echo "> Create DMG"
appdmg --quiet sqlcipher-nightly.json DB\ Browser\ for\ SQLCipher\ Nightly-universal_$DATE.dmg
codesign --sign "$DEV_ID" --verbose --options=runtime --timestamp DB\ Browser\ for\ SQLCipher\ Nightly-universal_$DATE.dmg
codesign -vvv --deep --strict --verbose=4 DB\ Browser\ for\ SQLCipher\ Nightly-universal_$DATE.dmg

# Notarize the DMG
echo "> Notarize the DMG"
xcrun notarytool submit DB\ Browser\ for\ SQLCipher\ Nightly-universal_$DATE.dmg --apple-id "$APPLE_ID" --password "$APPLE_PW" --team-id "$TEAM_ID" --wait

# Staple the notarization ticket
echo "> Staple the notarization ticket"
xcrun stapler staple DB\ Browser\ for\ SQLCipher\ Nightly-universal_$DATE.dmg

# TODO: Upload to the nightly server
echo "> DONE <"