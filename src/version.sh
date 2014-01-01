#!/bin/sh
cd $1

version=`git log -n1 --format="\"%h_git\""`
inc_guard_head="#ifndef __GEN_VERSION_H__\n#define __GEN_VERISON_H__"
inc_guard_foot="#endif"
version_numbers="#define MAJOR_VERSION 999\n#define MINOR_VERSION 0\n#define PATCH_VERSION 0"
version_macro="#define APP_VERSION $version"

if [ -f gen_version.h ]; then
	file_unchanged=`grep "$version_macro" gen_version.h`
fi

if [ "$file_unchanged" = "" ]; then
	printf "$inc_guard_head\n$version_macro\n$version_numbers\n$inc_guard_foot\n" > gen_version.h
fi
