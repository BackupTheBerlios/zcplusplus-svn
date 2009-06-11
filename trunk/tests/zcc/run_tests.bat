@REM not sure how far below WinXP this will run
@REM runs regression tests for z_cpp.exe
@REM (C)2009 Kenneth Boyd, license: MIT.txt

@setlocal
@set BAD_PASS=0
@set BAD_PASS_NAME=LastAccepted:
@set REJECT_TEST=0
@set FAILED=0
@set BAD_FAIL_NAME=LastRejected:
@set ACCEPT_TEST=0
@set CPP=..\..\zcc
@set CPP_ISO=..\..\zcc --pedantic

@echo Checking ISO acceptance requirements
@echo ====
@for %%f in (Pass*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (Pass*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1

@echo %BAD_PASS% of %REJECT_TEST% rejection tests accepted
@if not "%BAD_PASS_NAME%"=="LastAccepted:" @echo %BAD_PASS_NAME%
@echo %FAILED% of %ACCEPT_TEST% acceptance tests rejected
@if not "%BAD_FAIL_NAME%"=="LastRejected:" @echo %BAD_FAIL_NAME%

