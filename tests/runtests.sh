#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libevtx/.libs/libevtx.1.dylib && test -f ./pyevtx/.libs/pyevtx.so
then
	install_name_tool -change /usr/local/lib/libevtx.1.dylib ${PWD}/libevtx/.libs/libevtx.1.dylib ./pyevtx/.libs/pyevtx.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

