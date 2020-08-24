#!/usr/bin/env sh

# This script depends on ImageMagick being installed

FILES=()	# array accumulating names of converted images
# commands to give to convert for each iteration
MAGICK_CMDS="+antialias -units PixelsPerInch -alpha set -background transparent"
SRC_FILE="../../images/logo.svg" # conversion source
ICO_FILE="../iconwin.ico"        # ouput icon file
for imgsize in 16 32 64 128
do
    RESIZE="${imgsize}x${imgsize}"
    DEST_FILE="icon${imgsize}.png"
	# NOTE: explicitly not using quotes - outpput raw contents to convert command
	convert ${MAGICK_CMDS} ${SRC_FILE} -resize ${RESIZE} ${DEST_FILE}
    FILES+=("${DEST_FILE}")
done
convert "${FILES[@]}" "${ICO_FILE}"
rm "${FILES[@]}"
