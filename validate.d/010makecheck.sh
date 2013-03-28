#! /bin/sh
#
# This expects that package ROOT as the first argument.
#

# CMT commands must be run in the CMT directory
if [ x$2 == x ] ; then
    echo "Cannot get the current package root"
    echo "NOT RUN"
    exit 0
fi

cd $2/cmt
# DO NOT RUN "make check"
# cmt broadcast make check

