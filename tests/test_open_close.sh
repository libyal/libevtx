#!/bin/bash
#
# Library open close testing script
#
# Copyright (c) 2009-2013, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

INPUT="input";

test_open_close()
{ 
	echo "Testing open close of input:" $*;

	./${EVTX_TEST_OPEN_CLOSE} $*;

	RESULT=$?;

	echo "";

	return ${RESULT};
}

EVTX_TEST_OPEN_CLOSE="evtx_test_open_close";

if ! test -x ${EVTX_TEST_OPEN_CLOSE};
then
	EVTX_TEST_OPEN_CLOSE="evtx_test_open_close.exe";
fi

if ! test -x ${EVTX_TEST_OPEN_CLOSE};
then
	echo "Missing executable: ${EVTX_TEST_OPEN_CLOSE}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No input directory found, to test open close create a directory named input and fill it with test files.";

	exit ${EXIT_IGNORE};
fi

OLDIFS=${IFS};
IFS="
";

for FILENAME in ${INPUT}/*;
do
	if ! test_open_close ${FILENAME};
	then
		exit ${EXIT_FAILURE};
	fi
done

IFS=${OLDIFS};

exit ${EXIT_SUCCESS};

