#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libluksde/.libs/libluksde.1.dylib" ] && [ -f ./pyluksde/.libs/pyluksde.so ]
then
    install_name_tool -change /usr/local/lib/libluksde.1.dylib "${PWD}/libluksde/.libs/libluksde.1.dylib" ./pyluksde/.libs/pyluksde.so
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

