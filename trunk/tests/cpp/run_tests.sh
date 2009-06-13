#!/bin/sh
# runs regression tests for z_cpp.exe
# (C)2009 Kenneth Boyd, license: MIT.txt

ASSERT_FAILED=0
ASSERT_FAIL_NAME=

function code_screen {
	if test ${1} -eq 3; then let ++ASSERT_FAILED; ASSERT_FAIL_NAME="$ASSERT_FAIL_NAME ${2}"; fi;
}

function run_tests {
	local BAD_PASS=0
	local BAD_PASS_NAME=
	local REJECT_TEST=0
	local FAILED=0
	local BAD_FAIL_NAME=
	local ACCEPT_TEST=0
	local CPP=../../z_cpp
	local CPP_ISO="../../z_cpp --pedantic"

	echo Checking ISO error requirements
	echo ====
	for F in Error*.h; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in Error*.hpp; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in UNICODE.C99/Error*.h; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in UNICODE.C99/Error*.hpp; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in define.C99/Error*.h; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in define.C99/Error*.hpp; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in defined.C99/Error*.h; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in defined.C99/Error*.hpp; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in ifdef.C99/Error*.h; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in ifdef.C99/Error*.hpp; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in if.C99/Error*.h; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in if.C99/Error*.hpp; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in pragma.C99/Error*.h; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in pragma.C99/Error*.hpp; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;

	echo Checking ZCC warnings on ISO-accepted code
	echo ====
	for F in Warn*.h; do let ++REJECT_TEST; echo $CPP_ISO -Werror $F; if $CPP_ISO -Werror $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in Warn*.h; do let ++ACCEPT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in Warn*.hpp; do let ++REJECT_TEST; echo $CPP_ISO -Werror $F; if $CPP_ISO -Werror $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in Warn*.hpp; do let ++ACCEPT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;

	echo Checking ISO acceptance requirements
	echo ====
	for F in Pass*.h; do let ++ACCEPT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in define.C99/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in define.C99/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in if.C99/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in if.C99/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;

	echo Checking ZCC implementation-defined errors and features
	echo ====
	for F in default/Error*.h; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/Error*.hpp; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/keywords/Error*.h; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/keywords/Error*.hpp; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/Pass*.h; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;

	echo Checking ZCC nonconforming errors
	echo ====
	for F in default.nonconforming/Error*.h; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default.nonconforming/Error*.hpp; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;

	echo Checking ZCC content transforms
	echo ====
	for F in default/Preprocess*.h; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F | cmp - $F.txt; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/Preprocess*.hpp; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F | cmp - $F.txt; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;

	echo -E $BAD_PASS of $REJECT_TEST rejection tests accepted
	if test -n "$BAD_PASS_NAME"; then echo -E $BAD_PASS_NAME; fi
	echo -E $FAILED of $ACCEPT_TEST acceptance tests rejected
	if test -n "$BAD_FAIL_NAME"; then echo -E $BAD_FAIL_NAME; fi
	echo -E $ASSERT_FAILED tests failed by critical bugs
	if test -n "$ASSERT_FAIL_NAME"; then echo -E $ASSERT_FAIL_NAME; fi
}

run_tests

