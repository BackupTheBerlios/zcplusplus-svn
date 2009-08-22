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
@set CPP=..\..\z_cpp
@set CPP_ISO=..\..\z_cpp --pedantic
@set CPP_SIGNMAG_NOTRAP=..\..\z_cpp --int-sign-magnitude
@set CPP_SIGNMAG_TRAP=..\..\z_cpp --int-sign-magnitude --int-traps
@set CPP_ONESCOMP_NOTRAP=..\..\z_cpp --int-ones-complement
@set CPP_ONESCOMP_TRAP=..\..\z_cpp --int-ones-complement --int-traps
@set CPP_TWOSCOMP_NOTRAP=..\..\z_cpp --int-twos-complement
@set CPP_TWOSCOMP_TRAP=..\..\z_cpp --int-twos-complement --int-traps

@echo Checking ISO error requirements
@echo ====
@for %%f in (Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (UNICODE.C99\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (UNICODE.C99\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (UNICODE.C99\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (UNICODE.C99\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (define.C99\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (define.C99\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (define.C99\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (define.C99\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (defined.C99\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (defined.C99\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (defined.C99\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (defined.C99\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (ifdef.C99\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (ifdef.C99\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (ifdef.C99\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (ifdef.C99\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (if.C99\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (if.C99\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (if.C99\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (if.C99\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (line.C99\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (line.C99\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (line.C99\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (line.C99\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (pragma.C99\Error*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (pragma.C99\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (pragma.C99\Error*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (pragma.C99\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1

@echo Checking ZCC warnings on ISO-accepted code
@echo ====
@for %%f in (Warn*.h) do @echo %CPP_ISO% -Werror %%f & @%CPP_ISO% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (Warn*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (Warn*.h) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)
@for %%f in (Warn*.hpp) do @echo %CPP_ISO% -Werror %%f & @%CPP_ISO% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (Warn*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (Warn*.hpp) do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)

@echo Checking ISO acceptance requirements
@echo ====
@for %%f in (Pass*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (Pass*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (define.C99\Pass*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (define.C99\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (define.C99\Pass*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (define.C99\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (if.C99\Pass*.h) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (if.C99\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (if.C99\Pass*.hpp) do @echo %CPP_ISO% %%f & @%CPP_ISO% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (if.C99\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1

@echo Checking ZCC implementation-defined errors and features
@echo ====
@for %%f in (default\Error*.h) do @echo %CPP% %%f & @%CPP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\Error*.hpp) do @echo %CPP% %%f & @%CPP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\keywords\Error*.h) do @echo %CPP% %%f & @%CPP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\keywords\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\keywords\Error*.hpp) do @echo %CPP% %%f & @%CPP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\keywords\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\Pass*.h) do @echo %CPP% %%f & @%CPP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\Pass*.hpp) do @echo %CPP% %%f & @%CPP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\signmag.core\Pass*.h) do @echo %CPP_SIGNMAG_NOTRAP% %%f & @%CPP_SIGNMAG_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\signmag.core\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\signmag.core\Pass*.hpp) do @echo %CPP_SIGNMAG_NOTRAP% %%f & @%CPP_SIGNMAG_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\signmag.core\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\signmag.notrap\Pass*.h) do @echo %CPP_SIGNMAG_NOTRAP% %%f & @%CPP_SIGNMAG_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\signmag.notrap\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\signmag.notrap\Pass*.hpp) do @echo %CPP_SIGNMAG_NOTRAP% %%f & @%CPP_SIGNMAG_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\signmag.notrap\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\signmag.core\Pass*.h) do @echo %CPP_SIGNMAG_TRAP% %%f & @%CPP_SIGNMAG_TRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\signmag.core\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\signmag.core\Pass*.hpp) do @echo %CPP_SIGNMAG_TRAP% %%f & @%CPP_SIGNMAG_TRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\signmag.core\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\signmag.trap\Error*.h) do @echo %CPP_SIGNMAG_TRAP% %%f & @%CPP_SIGNMAG_TRAP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\signmag.trap\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\signmag.trap\Error*.hpp) do @echo %CPP_SIGNMAG_TRAP% %%f & @%CPP_SIGNMAG_TRAP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\signmag.trap\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\onescomp.core\Pass*.h) do @echo %CPP_ONESCOMP_NOTRAP% %%f & @%CPP_ONESCOMP_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\onescomp.core\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\onescomp.core\Pass*.hpp) do @echo %CPP_ONESCOMP_NOTRAP% %%f & @%CPP_ONESCOMP_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\onescomp.core\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\onescomp.notrap\Pass*.h) do @echo %CPP_ONESCOMP_NOTRAP% %%f & @%CPP_ONESCOMP_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\onescomp.notrap\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\onescomp.notrap\Pass*.hpp) do @echo %CPP_ONESCOMP_NOTRAP% %%f & @%CPP_ONESCOMP_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\onescomp.notrap\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\onescomp.core\Pass*.h) do @echo %CPP_ONESCOMP_TRAP% %%f & @%CPP_ONESCOMP_TRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\onescomp.core\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\onescomp.core\Pass*.hpp) do @echo %CPP_ONESCOMP_TRAP% %%f & @%CPP_ONESCOMP_TRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\onescomp.core\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\onescomp.trap\Error*.h) do @echo %CPP_ONESCOMP_TRAP% %%f & @%CPP_ONESCOMP_TRAP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\onescomp.trap\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\onescomp.trap\Error*.hpp) do @echo %CPP_ONESCOMP_TRAP% %%f & @%CPP_ONESCOMP_TRAP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\onescomp.trap\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\twoscomp.notrap\Pass*.h) do @echo %CPP_TWOSCOMP_NOTRAP% %%f & @%CPP_TWOSCOMP_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\twoscomp.notrap\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\twoscomp.notrap\Pass*.hpp) do @echo %CPP_TWOSCOMP_NOTRAP% %%f & @%CPP_TWOSCOMP_NOTRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\twoscomp.notrap\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\twoscomp.trap\Pass*.h) do @echo %CPP_TWOSCOMP_TRAP% %%f & @%CPP_TWOSCOMP_TRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\twoscomp.trap\Pass*.h) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\twoscomp.trap\Pass*.hpp) do @echo %CPP_TWOSCOMP_TRAP% %%f & @%CPP_TWOSCOMP_TRAP% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)
@for %%f in (default\twoscomp.trap\Pass*.hpp) do @set /a ACCEPT_TEST=ACCEPT_TEST+1
@for %%f in (default\twoscomp.trap\Error*.h) do @echo %CPP_TWOSCOMP_TRAP% %%f & @%CPP_TWOSCOMP_TRAP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\twoscomp.trap\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default\twoscomp.trap\Error*.hpp) do @echo %CPP_TWOSCOMP_TRAP% %%f & @%CPP_TWOSCOMP_TRAP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default\twoscomp.trap\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1

@echo Checking ZCC nonconforming errors
@echo ====
@for %%f in (default.nonconforming\Error*.h) do @echo %CPP% %%f & @%CPP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default.nonconforming\Error*.h) do @set /a REJECT_TEST=REJECT_TEST+1
@for %%f in (default.nonconforming\Error*.hpp) do @echo %CPP% %%f & @%CPP% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)
@for %%f in (default.nonconforming\Error*.hpp) do @set /a REJECT_TEST=REJECT_TEST+1

@echo Checking ZCC content transforms
@echo ====

@echo %BAD_PASS% of %REJECT_TEST% rejection tests accepted
@if not "%BAD_PASS_NAME%"=="LastAccepted:" @echo %BAD_PASS_NAME%
@echo %FAILED% of %ACCEPT_TEST% acceptance tests rejected
@if not "%BAD_FAIL_NAME%"=="LastRejected:" @echo %BAD_FAIL_NAME%

