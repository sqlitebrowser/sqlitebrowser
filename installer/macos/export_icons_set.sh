#!/usr/bin/env sh

# This script depends on Inkscape being installed

logo='../../images/logo.svg'
iconset='macapp.iconset'

mkdir "$iconset"

for size in '16' '32' '128' '256' '512'; do
	retina_size=$(($size * 2))

	inkscape "$logo" --without-gui --export-width "$size" --export-file="$iconset/icon_${size}x${size}.png"
	inkscape "$logo" --without-gui --export-width "$retina_size" --export-file="$iconset/icon_${size}x${size}@2x.png"
done

iconutil --convert icns "$iconset"
rm -r "$iconset"
