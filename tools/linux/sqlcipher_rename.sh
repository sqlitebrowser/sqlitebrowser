#!/usr/bin/env bash

# rename.sh
# A script to revert SQLCipher v.4.7.0+ build artifacts being named 'sqlite3'.

set -euo pipefail

patch_pkgconfig() {
    PC_FILE="${1:-sqlite3.pc}"

    if [[ ! -f "$PC_FILE" ]]; then
      echo "Error: file not found: $PC_FILE" >&2
      exit 1
    fi

    tmp="$(mktemp)"
    trap 'rm -f "$tmp"' EXIT

    awk -v pc_file="$PC_FILE" '
    /^includedir=/ {
      if ($0 !~ /\/sqlcipher$/) $0 = $0 "/sqlcipher"
      found_includedir = 1
    }
    /^Name:/ {
      if (sub(/^Name:[[:space:]]*SQLite[[:space:]]*$/, "Name: SQLCipher")) found_name = 1
    }
    /^Libs:/ {
      gsub(/-lsqlite3/, "-lsqlcipher")
      found_libs = 1
    }
    { print }
    END {
      if (!found_includedir || !found_name || !found_libs) {
        print "Error: the includedir, Name or Libs line is missing from " pc_file ", or its Name is not SQLite" > "/dev/stderr"
        exit 1
      }
    }
    ' "$PC_FILE" > "$tmp"

    cp "$tmp" "$PC_FILE"
}

VERSION=$(cat VERSION)

mv ./prefix/bin/sqlite3 ./prefix/bin/sqlcipher
mv ./prefix/lib/libsqlite3.a ./prefix/lib/libsqlcipher.a
mv "./prefix/lib/libsqlite3.so.$VERSION" "./prefix/lib/libsqlcipher.so.$VERSION"
ln -s "$(pwd)/prefix/lib/libsqlcipher.so.$VERSION" ./prefix/lib/libsqlcipher.so
ln -s "$(pwd)/prefix/lib/libsqlcipher.so.$VERSION" ./prefix/lib/libsqlcipher.so.0
rm -f ./prefix/lib/libsqlite*
patch_pkgconfig ./prefix/lib/pkgconfig/sqlite3.pc
mv ./prefix/lib/pkgconfig/sqlite3.pc ./prefix/lib/pkgconfig/sqlcipher.pc
mkdir ./prefix/include/sqlcipher
mv ./prefix/include/*.h ./prefix/include/sqlcipher/
mv ./prefix/share/man/man1/sqlite3.1 ./prefix/share/man/man1/sqlcipher.1
