#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libevtx/.libs/libevtx.1.dylib && test -f ./pyevtx/.libs/pyevtx.so;
then
	install_name_tool -change /usr/local/lib/libevtx.1.dylib ${PWD}/libevtx/.libs/libevtx.1.dylib ./pyevtx/.libs/pyevtx.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

