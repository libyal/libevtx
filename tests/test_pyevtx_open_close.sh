#!/bin/bash
# Python-bindings open close testing script
#
# Version: 20160126

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`pwd`;
TEST_PREFIX=`dirname ${TEST_PREFIX}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib//'`;

INPUT_GLOB="*";

list_contains()
{
	LIST=$1;
	SEARCH=$2;

	for LINE in $LIST;
	do
		if test $LINE = $SEARCH;
		then
			return ${EXIT_SUCCESS};
		fi
	done

	return ${EXIT_FAILURE};
}

test_open_close()
{ 
	INPUT_FILE=$1;

	rm -rf tmp;
	mkdir tmp;

	echo "Testing open close of input: ${INPUT_FILE}";

	if test `uname -s` = 'Darwin';
	then
		DYLD_LIBRARY_PATH="../lib${TEST_PREFIX}/.libs/" PYTHONPATH="../py${TEST_PREFIX}/.libs/" ${PYTHON} ${SCRIPT} ${INPUT_FILE};
		RESULT=$?;
	else
		LD_LIBRARY_PATH="../lib${TEST_PREFIX}/.libs/" PYTHONPATH="../py${TEST_PREFIX}/.libs/" ${PYTHON} ${SCRIPT} ${INPUT_FILE};
		RESULT=$?;
	fi

	rm -rf tmp;

	return ${RESULT};
}

if ! test -z ${SKIP_PYTHON_TESTS};
then
	exit ${EXIT_IGNORE};
fi

PYTHON=`which python${PYTHON_VERSION} 2> /dev/null`;

if ! test -x ${PYTHON};
then
	echo "Missing executable: ${PYTHON}";

	exit ${EXIT_FAILURE};
fi

if ! test -d "input";
then
	echo "No input directory found.";

	exit ${EXIT_IGNORE};
fi

SCRIPT="py${TEST_PREFIX}_test_open_close.py";

if ! test -f ${SCRIPT};
then
	echo "Missing script: ${SCRIPT}";

	exit ${EXIT_FAILURE};
fi

OLDIFS=${IFS};
IFS="
";

RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

if test ${RESULT} -eq 0;
then
	echo "No files or directories found in the input directory.";

	EXIT_RESULT=${EXIT_IGNORE};
else
	IGNORELIST="";

	if test -f "input/.py${TEST_PREFIX}/ignore";
	then
		IGNORELIST=`cat input/.py${TEST_PREFIX}/ignore | sed '/^#/d'`;
	fi
	for TESTDIR in input/*;
	do
		if test -d "${TESTDIR}";
		then
			DIRNAME=`basename ${TESTDIR}`;

			if ! list_contains "${IGNORELIST}" "${DIRNAME}";
			then
				if test -f "input/.py${TEST_PREFIX}/${DIRNAME}/files";
				then
					TEST_FILES=`cat input/.py${TEST_PREFIX}/${DIRNAME}/files | sed "s?^?${TESTDIR}/?"`;
				else
					TEST_FILES=`ls -1 ${TESTDIR}/${INPUT_GLOB} 2> /dev/null`;
				fi
				for TEST_FILE in ${TEST_FILES};
				do
					BASENAME=`basename ${TEST_FILE}`;

					if ! test_open_close "${TEST_FILE}";
					then
						exit ${EXIT_FAILURE};
					fi
				done
			fi
		fi
	done

	EXIT_RESULT=${EXIT_SUCCESS};
fi

IFS=${OLDIFS};

exit ${EXIT_RESULT};

