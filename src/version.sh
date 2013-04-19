#!/bin/sh
cd $1
echo -n "#define APP_VERSION " > gen_version.h
git log -n1 --format="\"%h_git\"" >> gen_version.h
