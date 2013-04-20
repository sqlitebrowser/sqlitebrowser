#!/bin/sh
cd $1

version=`git log -n1 --format="\"%h_git\""`
version_macro="#define APP_VERSION $version"

if [ -f gen_version.h ]; then
	current_file=`cat gen_version.h`
else
	current_files=""
fi

if [ "$version_macro" != "$current_file" ]; then
	echo "$version_macro" > gen_version.h
fi
