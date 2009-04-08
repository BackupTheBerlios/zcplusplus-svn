#!\bin\sh
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
    local CPP=../../z_cpp --pedantic

    echo Checking ISO error requirements
    echo ====
    for F in Error*.hpp; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;
    for F in UNICODE.C99/Error*.h; do let ++REJECT_TEST; echo $CPP $F; if $CPP $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;

    echo Checking ISO acceptance requirements
    echo ====
    for F in Warn*.hpp; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
    for F in Pass*.hpp; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;
    for F in Pass*.h; do let ++ACCEPT_TEST; echo $CPP $F; if $CPP $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;

    echo -E $BAD_PASS of $REJECT_TEST rejection tests accepted
    if test -n "$BAD_PASS_NAME"; then echo -E $BAD_PASS_NAME; fi
    echo -E $FAILED of $ACCEPT_TEST acceptance tests rejected
    if test -n "$BAD_FAIL_NAME"; then echo -E $BAD_FAIL_NAME; fi
    echo -E $ASSERT_FAILED tests failed by critical bugs
    if test -n "$ASSERT_FAIL_NAME"; then echo -E $ASSERT_FAIL_NAME; fi
}

run_tests

#@echo Checking error requirements
#@echo ====
#@for %%f in (Error*.hpp) do @echo %CPP% %%f & @%CPP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
#@for %%f in (Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1

#@echo Checking warning requirements
#@echo ====
#@for %%f in (Warn*.hpp) do @echo %CPP% %%f & @%CPP% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
#@for %%f in (Warn*.hpp) do @echo %CPP% %%f & @%CPP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
#@for %%f in (Warn*.hpp) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)

#@echo Checking error/pedantic warning requirements
#@echo ====
#@for %%f in (ErrPedWarn*.hpp) do @echo %CPP% %%f & @%CPP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
#@for %%f in (ErrPedWarn*.hpp) do @echo %CPP% %%f & @%CPP% -pedantic %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
#@for %%f in (ErrPedWarn*.hpp) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)

#@echo Checking acceptance requirements
#@echo ====
#@for %%f in (Pass*.hpp) do @echo %CPP% %%f & @%CPP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
#@for %%f in (Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1

#@echo %BAD_PASS% of %REJECT_TEST% rejection tests accepted

#@if not "%BAD_PASS_NAME%"=="LastAccepted:" @echo %BAD_PASS_NAME%
#@echo %FAILED% of %ACCEPT_TEST% acceptance tests rejected
#@if not "%BAD_FAIL_NAME%"=="LastRejected:" @echo %BAD_FAIL_NAME%

