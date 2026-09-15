#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libevtx/.libs/libevtx.1.dylib" ] && [ -f ./pyevtx/.libs/pyevtx.so ]
then
    install_name_tool -change /usr/local/lib/libevtx.1.dylib "${PWD}/libevtx/.libs/libevtx.1.dylib" ./pyevtx/.libs/pyevtx.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

