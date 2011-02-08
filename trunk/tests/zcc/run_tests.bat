@REM not sure how far below WinXP this will run
@REM runs regression tests for z_cpp.exe
@REM (C)2009,2011 Kenneth Boyd, license: MIT.txt
@REM Note: namespace.CPP, compat subdirectories have no C files

@setlocal
@set BAD_PASS=0
@set BAD_PASS_NAME=LastAccepted:
@set REJECT_TEST=0
@set FAILED=0
@set BAD_FAIL_NAME=LastRejected:
@set ACCEPT_TEST=0
@set CPP=..\..\bin\zcc
@set CPP_ISO=..\..\bin\zcc --pedantic
@set CPP_BACKPORT=..\..\bin\zcc -Wbackport
@set CPP_COMPAT=..\..\bin\zcc -Wc-c++-compat

@echo Checking ISO error requirements
@echo ====
@for %%f in (decl.C99\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (decl.C99\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (decl.C99\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (decl.C99\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (namespace.CPP\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (namespace.CPP\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (staticassert.C1X\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (staticassert.C1X\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (staticassert.C1X\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (staticassert.C1X\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1

@echo Checking ZCC warnings on ISO-accepted code
@echo ====
@for %%f in (backport\Warn*.h) do @echo %CPP_BACKPORT% -Werror %%f & @%CPP_BACKPORT% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (backport\Warn*.h) do @echo %CPP_BACKPORT% %%f & @%CPP_BACKPORT% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (backport\Warn*.h) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)
@for %%f in (backport\Warn*.hpp) do @echo %CPP_BACKPORT% -Werror %%f & @%CPP_BACKPORT% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (backport\Warn*.hpp) do @echo %CPP_BACKPORT% %%f & @%CPP_BACKPORT% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (backport\Warn*.hpp) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)
@for %%f in (compat\Warn*.hpp) do @echo %CPP_COMPAT% -Werror %%f & @%CPP_COMPAT% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (compat\Warn*.hpp) do @echo %CPP_COMPAT% %%f & @%CPP_COMPAT% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (compat\Warn*.hpp) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)
@for %%f in (decl.C99\Warn*.h) do @echo %CPP_ISO% -Werror %%f & @%CPP_ISO% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (decl.C99\Warn*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (decl.C99\Warn*.h) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)
@for %%f in (decl.C99\Warn*.hpp) do @echo %CPP_ISO% -Werror %%f & @%CPP_ISO% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (decl.C99\Warn*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (decl.C99\Warn*.hpp) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)
@for %%f in (namespace.CPP\Warn*.hpp) do @echo %CPP_ISO% -Werror %%f & @%CPP_ISO% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (namespace.CPP\Warn*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (namespace.CPP\Warn*.hpp) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)

@echo Checking ISO acceptance requirements
@echo ====
@for %%f in (Pass*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (Pass*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (decl.C99\Pass*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (decl.C99\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (decl.C99\Pass*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (decl.C99\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (staticassert.C1X\Pass*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (staticassert.C1X\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (staticassert.C1X\Pass*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (staticassert.C1X\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1

@echo Checking ZCC implementation-defined errors and features
@echo ====
@for %%f in (default\staticassert\Pass*.h) do @echo %CPP% %%f & @%CPP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\staticassert\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\staticassert\Pass*.hpp) do @echo %CPP% %%f & @%CPP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\staticassert\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1

@echo %BAD_PASS% of %REJECT_TEST% rejection tests accepted
@if not "%BAD_PASS_NAME%"=="LastAccepted:" @echo %BAD_PASS_NAME%
@echo %FAILED% of %ACCEPT_TEST% acceptance tests rejected
@if not "%BAD_FAIL_NAME%"=="LastRejected:" @echo %BAD_FAIL_NAME%

