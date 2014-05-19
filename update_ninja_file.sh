#!/bin/sh
if test $# -lt 2; then
    echo "Usage: $0 <build.ninja.erb> <build.ninja>"
    exit 1
fi
erb -T - $1 > $2.tmp || exit 1
ninja -n -f $2.tmp 2>&1 build.ninja >/dev/null
if test "$?" = "0"; then
  mv $2.tmp $2
  exit 0
else
#  rm $2.tmp
  exit 1
fi
