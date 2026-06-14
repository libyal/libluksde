#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libluksde/.libs/libluksde.1.dylib && test -f ./pyluksde/.libs/pyluksde.so
then
	install_name_tool -change /usr/local/lib/libluksde.1.dylib ${PWD}/libluksde/.libs/libluksde.1.dylib ./pyluksde/.libs/pyluksde.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

