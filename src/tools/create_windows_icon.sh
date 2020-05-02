#!/usr/bin/env sh

# This script depends on ImageMagick being installed

for imgsize in 16 32 64 128
do
	convert ../icons/sqlitebrowser.png -resize "$imgsize"x"$imgsize" -background transparent icon"$imgsize".png
	files="$files icon$imgsize.png"
done

convert $files ../iconwin.ico
rm $files
