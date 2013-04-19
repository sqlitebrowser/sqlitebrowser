#!/bin/sh
cd $1

version=`git log -n1 --format="\"%h_git\""`
version_macro="#define APP_VERSION $version"
current_file=`cat gen_version.h`

if [ "$version_macro" != "$current_file" ]; then
	echo "$version_macro" > gen_version.h
fi
