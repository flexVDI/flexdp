#! /bin/sh

set -e # exit on errors

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.


(
    cd "$srcdir"
    autoreconf --verbose --force --install
)

