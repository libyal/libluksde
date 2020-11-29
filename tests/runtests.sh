#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libluksde/.libs/libluksde.1.dylib && test -f ./pyluksde/.libs/pyluksde.so;
then
	install_name_tool -change /usr/local/lib/libluksde.1.dylib ${PWD}/libluksde/.libs/libluksde.1.dylib ./pyluksde/.libs/pyluksde.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

