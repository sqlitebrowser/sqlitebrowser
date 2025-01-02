#!/usr/bin/env bash
# Create a new keychain
CERTIFICATE_PATH=$RUNNER_TEMP/build_certificate.p12
KEYCHAIN_PATH=$RUNNER_TEMP/app-signing.keychain-db
echo -n "$P12" | base64 --decode -o $CERTIFICATE_PATH
security create-keychain -p "$KEYCHAIN_PW" $KEYCHAIN_PATH
security set-keychain-settings -lut 21600 $KEYCHAIN_PATH
security unlock-keychain -p "$KEYCHAIN_PW" $KEYCHAIN_PATH
security import $CERTIFICATE_PATH -P "$P12_PW" -A -t cert -f pkcs12 -k $KEYCHAIN_PATH
security list-keychain -d user -s $KEYCHAIN_PATH

# Run macdeployqt
find build -name "DB Browser for SQL*.app" -exec $(brew --prefix sqlb-qt@5)/bin/macdeployqt {} -sign-for-notarization=$DEV_ID \;

# Add the 'formats' and 'nalgeon/sqlean' extensions to the app bundle
gh auth login --with-token <<< "$GH_TOKEN"
gh release download --pattern "sqlean-macos-x86.zip" --repo "nalgeon/sqlean"
unzip sqlean-macos-x86.zip -d sqlean-macos-x86
gh release download --pattern "sqlean-macos-arm64.zip" --repo "nalgeon/sqlean"
unzip sqlean-macos-arm64.zip -d sqlean-macos-arm64
lipo -create sqlean-macos-x86/sqlean.dylib sqlean-macos-arm64/sqlean.dylib -output sqlean.dylib
for TARGET in $(find build -name "DB Browser for SQL*.app" | sed -e 's/ /_/g'); do
    TARGET=$(echo $TARGET | sed -e 's/_/ /g')
    mkdir "$TARGET/Contents/Extensions"

    arch -x86_64 clang -I /opt/homebrew/opt/sqlb-sqlite/include -L /opt/homebrew/opt/sqlb-sqlite/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o formats_x86_64.dylib
    clang -I /opt/homebrew/opt/sqlb-sqlite/include -L /opt/homebrew/opt/sqlb-sqlite/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o formats_arm64.dylib
    lipo -create formats_x86_64.dylib formats_arm64.dylib -output "$TARGET/Contents/Extensions/formats.dylib"

    if [ -f "$TARGET/Contents/Extensions/formats.dylib" ]; then
        install_name_tool -id "@executable_path/../Extensions/formats.dylib" "$TARGET/Contents/Extensions/formats.dylib"
        ln -s formats.dylib "$TARGET/Contents/Extensions/formats.dylib.dylib"
    fi

    cp sqlean.dylib "$TARGET/Contents/Extensions/"
    if [ -f "$TARGET/Contents/Extensions/sqlean.dylib" ]; then
        install_name_tool -id "@executable_path/../Extensions/sqlean.dylib" "$TARGET/Contents/Extensions/sqlean.dylib"
        ln -s sqlean.dylib "$TARGET/Contents/Extensions/sqlean.dylib.dylib"
    fi
done

# Copy the license file to the app bundle
for TARGET in $(find build -name "DB Browser for SQL*.app" | sed -e 's/ /_/g'); do
    TARGET=$(echo $TARGET | sed -e 's/_/ /g')
    cp LICENSE* "$TARGET/Contents/Resources/"
done

# Copy the translation files to the app bundle
for TARGET in $(find build -name "DB Browser for SQL*.app" | sed -e 's/ /_/g'); do
    TARGET=$(echo $TARGET | sed -e 's/_/ /g')
    mkdir "$TARGET/Contents/translations"
    for i in ar cs de en es fr it ko pl pt pt_BR ru uk zh_CN zh_TW; do
    find $(brew --prefix sqlb-qt@5)/translations -name "qt_${i}.qm" 2> /dev/null -exec cp {} "$TARGET/Contents/translations/" \;
    find $(brew --prefix sqlb-qt@5)/translations -name "qtbase_${i}.qm" 2> /dev/null -exec cp {} "$TARGET/Contents/translations/" \;
    find $(brew --prefix sqlb-qt@5)/translations -name "qtmultimedia_${i}.qm" 2> /dev/null -exec cp {} "$TARGET/Contents/translations/" \;
    find $(brew --prefix sqlb-qt@5)/translations -name "qtscript_${i}.qm" 2> /dev/null -exec cp {} "$TARGET/Contents/translations/" \;
    find $(brew --prefix sqlb-qt@5)/translations -name "qtxmlpatterns_${i}.qm" 2> /dev/null -exec cp {} "$TARGET/Contents/translations/" \;
    done 
done

# Copy the icon file to the app bundle
for TARGET in $(find build -name "DB Browser for SQL*.app" | sed -e 's/ /_/g'); do
    TARGET=$(echo $TARGET | sed -e 's/_/ /g')
    if [ "$NIGHTLY" = "false" ]; then
    cp installer/macos/macapp.icns "$TARGET/Contents/Resources/"
    /usr/libexec/PlistBuddy -c "Set :CFBundleIconFile macapp.icns" "$TARGET/Contents/Info.plist"
    else
    cp installer/macos/macapp-nightly.icns "$TARGET/Contents/Resources/"
    /usr/libexec/PlistBuddy -c "Set :CFBundleIconFile macapp-nightly.icns" "$TARGET/Contents/Info.plist"
    fi
done

# Sign the manually added extensions
for TARGET in $(find build -name "DB Browser for SQL*.app" | sed -e 's/ /_/g'); do
    TARGET=$(echo $TARGET | sed -e 's/_/ /g')
    codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp "$TARGET/Contents/Extensions/formats.dylib"
    codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp "$TARGET/Contents/Extensions/sqlean.dylib"
    codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp "$TARGET"
done

# Move app bundle to installer folder for DMG creation
mv build/*.app installer/macos

# Create the DMG
export DATE=$(date +%Y%m%d)

if [ "$SQLCIPHER" = "true" ]; then
    if [ "$NIGHTLY" = "false" ]; then
    # Continuous with SQLCipher
    sed -i "" 's/"DB Browser for SQLCipher Nightly.app"/"DB Browser for SQLCipher-dev-'$(git rev-parse --short --verify HEAD)'.app"/' installer/macos/sqlcipher-nightly.json
    TARGET="DB.Browser.for.SQLCipher-dev-$(git rev-parse --short --verify HEAD).dmg"
    else
    # Nightly with SQLCipher
    TARGET="DB.Browser.for.SQLCipher-universal_$DATE.dmg"
    fi
    appdmg --quiet installer/macos/sqlcipher-nightly.json "$TARGET"
else
    if [ "$NIGHTLY" = "false" ]; then
    # Continuous without SQLCipher
    sed -i "" 's/"DB Browser for SQLite Nightly.app"/"DB Browser for SQLite-dev-'$(git rev-parse --short --verify HEAD)'.app"/' installer/macos/nightly.json
    TARGET="DB.Browser.for.SQLite-dev-$(git rev-parse --short --verify HEAD).dmg"
    else
    # Nightly without SQLCipher
    TARGET="DB.Browser.for.SQLite-universal_$DATE.dmg"
    fi
    appdmg --quiet installer/macos/nightly.json "$TARGET"
fi

codesign --sign "$DEV_ID" --verbose --options=runtime --timestamp "$TARGET"
codesign -vvv --deep --strict --verbose=4 "$TARGET"

# Notarize the dmg
xcrun notarytool submit *.dmg --apple-id $APPLE_ID --password $APPLE_PW --team-id $TEAM_ID --wait

# Staple the notarization ticket
xcrun stapler staple *.dmg
