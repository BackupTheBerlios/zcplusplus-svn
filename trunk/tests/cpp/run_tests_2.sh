#!/bin/sh
# runs regression tests for zcc.exe -E
# (C)2009,2011 Kenneth Boyd, license: MIT.txt

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
	local CPP="../../bin/zcc -E"
	local CPP_ISO="../../bin/zcc -E --pedantic"
	local CPP_SIGNMAG_NOTRAP="../../bin/zcc -E --int-sign-magnitude"
	local CPP_SIGNMAG_TRAP="../../bin/zcc -E --int-sign-magnitude --int-traps"
	local CPP_ONESCOMP_NOTRAP="../../bin/zcc -E --int-ones-complement"
	local CPP_ONESCOMP_TRAP="../../bin/zcc -E --int-ones-complement --int-traps"
	local CPP_TWOSCOMP_NOTRAP="../../bin/zcc -E --int-twos-complement"
	local CPP_TWOSCOMP_TRAP="../../bin/zcc -E --int-twos-complement --int-traps"

	echo Checking ISO error requirements
	echo ====
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
	for F in line.C99/Error*.h; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in line.C99/Error*.hpp; do let ++REJECT_TEST; echo $CPP_ISO $F; if $CPP_ISO $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
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
	for F in default/has_include/Error*.h; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/has_include/Error*.hpp; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/has_include/Pass*.h; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/has_include/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/Pass*.h; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/signmag.core/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_SIGNMAG_NOTRAP $F; if $CPP_SIGNMAG_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/signmag.core/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_SIGNMAG_NOTRAP $F; if $CPP_SIGNMAG_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/signmag.notrap/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_SIGNMAG_NOTRAP $F; if $CPP_SIGNMAG_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/signmag.notrap/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_SIGNMAG_NOTRAP $F; if $CPP_SIGNMAG_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/signmag.core/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_SIGNMAG_TRAP $F; if $CPP_SIGNMAG_TRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/signmag.core/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_SIGNMAG_TRAP $F; if $CPP_SIGNMAG_TRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/signmag.trap/Error*.h; do let ++REJECT_TEST; echo $CPP_SIGNMAG_TRAP $F; if $CPP_SIGNMAG_TRAP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/signmag.trap/Error*.hpp; do let ++REJECT_TEST; echo $CPP_SIGNMAG_TRAP $F; if $CPP_SIGNMAG_TRAP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/onescomp.core/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_ONESCOMP_NOTRAP $F; if $CPP_ONESCOMP_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/onescomp.core/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_ONESCOMP_NOTRAP $F; if $CPP_ONESCOMP_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/onescomp.notrap/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_ONESCOMP_NOTRAP $F; if $CPP_ONESCOMP_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/onescomp.notrap/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_ONESCOMP_NOTRAP $F; if $CPP_ONESCOMP_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/onescomp.core/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_ONESCOMP_TRAP $F; if $CPP_ONESCOMP_TRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/onescomp.core/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_ONESCOMP_TRAP $F; if $CPP_ONESCOMP_TRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/onescomp.trap/Error*.h; do let ++REJECT_TEST; echo $CPP_ONESCOMP_TRAP $F; if $CPP_ONESCOMP_TRAP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/onescomp.trap/Error*.hpp; do let ++REJECT_TEST; echo $CPP_ONESCOMP_TRAP $F; if $CPP_ONESCOMP_TRAP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/twoscomp.notrap/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_TWOSCOMP_NOTRAP $F; if $CPP_TWOSCOMP_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/twoscomp.notrap/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_TWOSCOMP_NOTRAP $F; if $CPP_TWOSCOMP_NOTRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/twoscomp.trap/Pass*.h; do let ++ACCEPT_TEST; echo $CPP_TWOSCOMP_TRAP $F; if $CPP_TWOSCOMP_TRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/twoscomp.trap/Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP_TWOSCOMP_TRAP $F; if $CPP_TWOSCOMP_TRAP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/twoscomp.trap/Error*.h; do let ++REJECT_TEST; echo $CPP_TWOSCOMP_TRAP $F; if $CPP_TWOSCOMP_TRAP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default/twoscomp.trap/Error*.hpp; do let ++REJECT_TEST; echo $CPP_TWOSCOMP_TRAP $F; if $CPP_TWOSCOMP_TRAP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;

	echo Checking ZCC nonconforming errors
	echo ====
	for F in default.nonconforming/Error*.h; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
	for F in default.nonconforming/Error*.hpp; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;

	echo Checking ZCC content transforms
	echo ====
	for F in default/Preprocess*.h; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F | cmp - $F.txt; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in default/Preprocess*.hpp; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F | cmp - $F.txt; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in line.C99/Preprocess*.h; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F | cmp - $F.txt; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
	for F in line.C99/Preprocess*.hpp; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F | cmp - $F.txt; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;

	echo -E $BAD_PASS of $REJECT_TEST rejection tests accepted
	if test -n "$BAD_PASS_NAME"; then echo -E $BAD_PASS_NAME; fi
	echo -E $FAILED of $ACCEPT_TEST acceptance tests rejected
	if test -n "$BAD_FAIL_NAME"; then echo -E $BAD_FAIL_NAME; fi
	echo -E $ASSERT_FAILED tests failed by critical bugs
	if test -n "$ASSERT_FAIL_NAME"; then echo -E $ASSERT_FAIL_NAME; fi
}

run_tests

