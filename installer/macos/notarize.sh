#!/usr/bin/env bash
set -euo pipefail

CODESIGN=${CODESIGN:-true}
case "$CODESIGN" in
    true | false) ;;
    *)
        echo "::error::CODESIGN must be 'true' or 'false', not '$CODESIGN'."
        exit 1
        ;;
esac

if [ "$CODESIGN" = "true" ]; then
    # Create a new keychain
    CERTIFICATE_PATH=$RUNNER_TEMP/build_certificate.p12
    KEYCHAIN_PATH=$RUNNER_TEMP/app-signing.keychain-db
    echo -n "$P12" | base64 --decode -o "$CERTIFICATE_PATH"
    security create-keychain -p "$KEYCHAIN_PW" "$KEYCHAIN_PATH"
    security set-keychain-settings -lut 21600 "$KEYCHAIN_PATH"
    security unlock-keychain -p "$KEYCHAIN_PW" "$KEYCHAIN_PATH"
    security import "$CERTIFICATE_PATH" -P "$P12_PW" -A -t cert -f pkcs12 -k "$KEYCHAIN_PATH"
    security list-keychain -d user -s "$KEYCHAIN_PATH"
fi

QT_PREFIX=$(brew --prefix sqlb-qt@5)

APPS=()
while IFS= read -r -d '' APP; do
    APPS+=("$APP")
done < <(find build -name "DB Browser for SQL*.app" -print0)
if [ "${#APPS[@]}" -eq 0 ]; then
    echo "::error::No app bundle was found in the build directory."
    exit 1
fi

# Run macdeployqt
for TARGET in "${APPS[@]}"; do
    if [ "$CODESIGN" = "true" ]; then
        "$QT_PREFIX/bin/macdeployqt" "$TARGET" -sign-for-notarization="$DEV_ID"
    else
        "$QT_PREFIX/bin/macdeployqt" "$TARGET"
    fi
done

# Add the 'formats' and 'nalgeon/sqlean' extensions to the app bundle
SQLITE_PREFIX=$(brew --prefix sqlb-sqlite)
clang -arch x86_64 -arch arm64 -I "$SQLITE_PREFIX/include" -fno-common -dynamiclib \
    -install_name "@executable_path/../Extensions/formats.dylib" \
    src/extensions/extension-formats.c -o formats.dylib

gh release download --pattern "sqlean-macos-x64.zip" --repo "nalgeon/sqlean"
unzip sqlean-macos-x64.zip -d sqlean-macos-x64
gh release download --pattern "sqlean-macos-arm64.zip" --repo "nalgeon/sqlean"
unzip sqlean-macos-arm64.zip -d sqlean-macos-arm64
lipo -create sqlean-macos-x64/sqlean.dylib sqlean-macos-arm64/sqlean.dylib -output sqlean.dylib

for EXTENSION in formats.dylib sqlean.dylib; do
    for ARCH in x86_64 arm64; do
        if ! lipo "$EXTENSION" -verify_arch "$ARCH"; then
            echo "::error::$EXTENSION has no $ARCH slice. It only has: $(lipo -archs "$EXTENSION")."
            exit 1
        fi
    done
done

for TARGET in "${APPS[@]}"; do
    mkdir -p "$TARGET/Contents/Extensions"

    cp formats.dylib "$TARGET/Contents/Extensions/"

    cp sqlean.dylib "$TARGET/Contents/Extensions/"
    install_name_tool -id "@executable_path/../Extensions/sqlean.dylib" "$TARGET/Contents/Extensions/sqlean.dylib"
done

# Copy the license file to the app bundle
for TARGET in "${APPS[@]}"; do
    cp LICENSE* "$TARGET/Contents/Resources/"

    if [ "$SQLCIPHER" = "true" ]; then
        cp "$(brew --prefix sqlb-sqlcipher)/share/sqlb-sqlcipher/LICENSE-ICU" "$TARGET/Contents/Resources/"
    else
        cp "$(brew --prefix sqlb-sqlite)/share/sqlb-sqlite/LICENSE-ICU" "$TARGET/Contents/Resources/"
    fi
done

COPY_ARGS=()
for TARGET in "${APPS[@]}"; do
    COPY_ARGS+=(--copy-to "$TARGET/Contents/translations")
done
python3 tools/collect_qt_translations.py --qt-translations "$QT_PREFIX/translations" "${COPY_ARGS[@]}"

# Copy the icon file to the app bundle
for TARGET in "${APPS[@]}"; do
    cp installer/macos/macapp-nightly.icns "$TARGET/Contents/Resources/"
    /usr/libexec/PlistBuddy -c "Set :CFBundleIconFile macapp-nightly.icns" "$TARGET/Contents/Info.plist"
done

# Sign the manually added extensions
if [ "$CODESIGN" = "true" ]; then
    for TARGET in "${APPS[@]}"; do
        codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp "$TARGET/Contents/Extensions/formats.dylib"
        codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp "$TARGET/Contents/Extensions/sqlean.dylib"
        codesign --sign "$DEV_ID" --deep --force --options=runtime --strict --timestamp "$TARGET"
    done
fi

# Move app bundle to installer folder for DMG creation
mv build/*.app installer/macos

# Create the DMG
if [ "$NIGHTLY" != "false" ]; then
    DATE=${NIGHTLY_DATE:?A nightly build needs NIGHTLY_DATE, the date of the whole run.}
fi
SHORT_SHA=$(git rev-parse --short --verify HEAD)

if [ "$SQLCIPHER" = "true" ]; then
    if [ "$NIGHTLY" = "false" ]; then
    # Continuous with SQLCipher
    sed -i "" 's/"DB Browser for SQLCipher Nightly.app"/"DB Browser for SQLCipher-dev-'"$SHORT_SHA"'.app"/' installer/macos/sqlcipher-nightly.json
    DMG="DB.Browser.for.SQLCipher-dev-$SHORT_SHA.dmg"
    else
    # Nightly with SQLCipher
    DMG="DB.Browser.for.SQLCipher-universal_$DATE.dmg"
    fi
    appdmg --quiet installer/macos/sqlcipher-nightly.json "$DMG"
else
    if [ "$NIGHTLY" = "false" ]; then
    # Continuous without SQLCipher
    sed -i "" 's/"DB Browser for SQLite Nightly.app"/"DB Browser for SQLite-dev-'"$SHORT_SHA"'.app"/' installer/macos/nightly.json
    DMG="DB.Browser.for.SQLite-dev-$SHORT_SHA.dmg"
    else
    # Nightly without SQLCipher
    DMG="DB.Browser.for.SQLite-universal_$DATE.dmg"
    fi
    appdmg --quiet installer/macos/nightly.json "$DMG"
fi

if [ "$CODESIGN" != "true" ]; then
    echo "Created $DMG without signing and notarization."
    exit 0
fi

codesign --sign "$DEV_ID" --verbose --options=runtime --timestamp "$DMG"
codesign -vvv --deep --strict --verbose=4 "$DMG"

# Notarize the dmg
xcrun notarytool submit "$DMG" --apple-id "$APPLE_ID" --password "$APPLE_PW" --team-id "$TEAM_ID" --wait

# Staple the notarization ticket
xcrun stapler staple "$DMG"
