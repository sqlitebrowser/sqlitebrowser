#!/bin/bash

# Moving the nightly builds into appropriate subdirs.  Designed to be
# run automatically from cron, using something like this:
#   10 0 14 * * /usr/local/bin/move_nightlies_into_dirs.sh

# Retrieve the month number for last month
YEARMONTH=`date -d "last month 13:00" '+%Y-%m'`
YEARMONTHOSX=`date -d "last month 13:00" '+%Y%m'`

# Create appropriate new subfolders
mkdir /nightlies/osx/${YEARMONTH}
mkdir /nightlies/win32/${YEARMONTH}
mkdir /nightlies/win64/${YEARMONTH}

# Move builds
mv /nightlies/osx/DB*${YEARMONTHOSX}* /nightlies/osx/${YEARMONTH}/
mv /nightlies/win32/DB*${YEARMONTH}* /nightlies/win32/${YEARMONTH}/
mv /nightlies/win64/DB*${YEARMONTH}* /nightlies/win64/${YEARMONTH}/

# Fix ownership and context
chown -Rh nightlies: /nightlies/osx/${YEARMONTH} /nightlies/win32/${YEARMONTH} /nightlies/win64/${YEARMONTH}
restorecon -RFv /nightlies/osx/${YEARMONTH} /nightlies/win32/${YEARMONTH} /nightlies/win64/${YEARMONTH}

echo Nightlies moved for $YEARMONTH
