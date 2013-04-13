#!/bin/bash
#
# Windows Shortcut File (EVTX) format library Python-bindings open close testing script
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

PYTHON="/usr/bin/python";

if ! test -x ${PYTHON};
then
	echo "Missing executable: ${PYTHON}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No input directory found, to test pyevtx create a directory named input and fill it with test files.";

	exit ${EXIT_IGNORE};
fi

OLDIFS=${IFS};
IFS="
";

for FILENAME in ${INPUT}/*;
do
	if ! PYTHONPATH=../pyevtx/.libs/ ${PYTHON} pyevtx_test_open_close.py ${FILENAME};
	then
		exit ${EXIT_FAILURE};
	fi
done

IFS=${OLDIFS};

exit ${EXIT_SUCCESS};

