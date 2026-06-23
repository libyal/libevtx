#!/bin/sh
# Script that synchronizes the local test data

TESTS_INPUT_DIRECTORY="tests/input"
TEST_SET="public"
TEST_FILES="System.evtx"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.evtxexport_xml"
echo "-fxml" > "${TESTS_INPUT_DIRECTORY}/.evtxexport_xml/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/${TEST_SET}"

for TEST_FILE in ${TEST_FILES}
do
	URL="https://raw.githubusercontent.com/log2timeline/plaso/refs/heads/main/test_data/evtx/${TEST_FILE}"

	curl -L -o "${TESTS_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL}
done
