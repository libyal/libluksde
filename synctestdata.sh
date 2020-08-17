#!/bin/sh
# Script that synchronizes the local test data
#
# Version: 20161009

TEST_SET="public";
TEST_INPUT_DIRECTORY="tests/input";
TEST_FILES="luks1.raw";

mkdir -p "${TEST_INPUT_DIRECTORY}/${TEST_SET}";
mkdir -p "${TEST_INPUT_DIRECTORY}/.libluksde/${TEST_SET}";
mkdir -p "${TEST_INPUT_DIRECTORY}/.pyluksde/${TEST_SET}";
mkdir -p "${TEST_INPUT_DIRECTORY}/.luksdeinfo/${TEST_SET}";

for TEST_FILE in ${TEST_FILES};
do
	URL="https://github.com/log2timeline/dfvfs/blob/master/test_data/${TEST_FILE}?raw=true";

	curl -L -o "${TEST_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL};

	echo "-pluksde-TEST" > "${TEST_INPUT_DIRECTORY}/.libluksde/${TEST_SET}/${TEST_FILE}.password";
	echo "-pluksde-TEST" > "${TEST_INPUT_DIRECTORY}/.pyluksde/${TEST_SET}/${TEST_FILE}.password";
	echo "-pluksde-TEST" > "${TEST_INPUT_DIRECTORY}/.luksdeinfo/${TEST_SET}/${TEST_FILE}.password";
done

