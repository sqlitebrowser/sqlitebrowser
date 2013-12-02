#!/bin/sh
cd $1

version=`git log -n1 --format="\"%h_git\""`
inc_guard_head="#ifndef __GEN_VERSIONS_H__\n#define __GEN_VERISONS_H__\n"
inc_guard_foot="#endif"
version_numbers="#define MAJOR_VERSION 999\n#define MINOR_VERSION 0\n#define PATCH_VERSION 0\n"
version_macro="#define APP_VERSION $version\n"

if [ -f gen_version.h ]; then
	current_file=`cat gen_version.h`
fi

if [ "$version_macro" != "$current_file" ]; then
	echo -e "$inc_guard_head$version_macro$version_numbers$inc_guard_foot" > gen_version.h
fi
