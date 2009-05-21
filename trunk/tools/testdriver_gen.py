#!/usr/bin/python
# an auxilliary script for generating the test files
# designed for Python 2.6, may work with other versions
# (C)2009 Kenneth Boyd, license: MIT.txt

from sys import argv,exit;

def SpawnTestDrivers(filename):
	shell_lines = ['#!/bin/sh\n']
	batch_lines = ['@REM not sure how far below WinXP this will run\n']
	suffixes = []
	with open(filename,'r') as f:
		for line in f:
			if line.startswith('SUFFIXES '):
				suffixes = line[9:].strip().split()
			elif line.startswith('DESC '):
				desc = line[5:].strip()
				shell_lines.append('# '+desc+'\n')
				batch_lines.append('@REM '+desc+'\n')
			elif line.startswith('PROLOG'):
				shell_lines.append('ASSERT_FAILED=0\n')
				shell_lines.append('ASSERT_FAIL_NAME=\n\n')
				shell_lines.append('function code_screen {\n')
				shell_lines.append('\tif test ${1} -eq 3; then let ++ASSERT_FAILED; ASSERT_FAIL_NAME="$ASSERT_FAIL_NAME ${2}"; fi;\n')
				shell_lines.append('}\n\n')
				shell_lines.append('function run_tests {\n')
				shell_lines.append('\tlocal BAD_PASS=0\n')
				shell_lines.append('\tlocal BAD_PASS_NAME=\n')
				shell_lines.append('\tlocal REJECT_TEST=0\n')
				shell_lines.append('\tlocal FAILED=0\n')
				shell_lines.append('\tlocal BAD_FAIL_NAME=\n')
				shell_lines.append('\tlocal ACCEPT_TEST=0\n');

				batch_lines.append('@setlocal\n')
				batch_lines.append('@set BAD_PASS=0\n')
				batch_lines.append('@set BAD_PASS_NAME=LastAccepted:\n')
				batch_lines.append('@set REJECT_TEST=0\n')
				batch_lines.append('@set FAILED=0\n')
				batch_lines.append('@set BAD_FAIL_NAME=LastRejected:\n')
				batch_lines.append('@set ACCEPT_TEST=0\n')
			elif line.startswith('VAR '):
				desc = line[4:].strip().split(None,1)
				if -1<desc[1].rfind(' '):
					shell_lines.append('\tlocal '+desc[0]+'="'+desc[1]+'"\n')
				else:
					shell_lines.append('\tlocal '+desc[0]+'='+desc[1]+'\n')
				batch_lines.append('@set '+desc[0]+'='+desc[1].replace('/','\\')+'\n')
			elif line.startswith('ECHO '):
				desc = line[5:].strip()
				shell_lines.append('\techo '+desc+'\n\techo ====\n')
				batch_lines.append('@echo '+desc+'\n@echo ====\n')
			elif line.startswith('ERROR '):
				desc = line[6:].strip().split(None,1)
				batch_desc = [desc[0].replace('/','\\'),desc[1].replace('/','\\')]
				for suffix in suffixes:
					shell_lines.append('\tfor F in '+desc[1]+'.'+suffix+'; do let ++REJECT_TEST; echo $'+desc[0]+' $F; if $'+desc[0]+' $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;\n');
					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @echo %'+batch_desc[0]+'% %%f & @%'+batch_desc[0]+'% %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)\n')
					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @set /a REJECT_TEST=REJECT_TEST+1\n')
			elif line.startswith('PASS '):
				desc = line[5:].strip().split(None,1)
				batch_desc = [desc[0].replace('/','\\'),desc[1].replace('/','\\')]
				for suffix in suffixes:
					shell_lines.append('\tfor F in '+desc[1]+'.'+suffix+'; do let ++ACCEPT_TEST; echo $'+desc[0]+' $F; if $'+desc[0]+' $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;\n');
					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @echo %'+batch_desc[0]+'% %%f & @%'+batch_desc[0]+'% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)\n')
					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @set /a ACCEPT_TEST=ACCEPT_TEST+1\n')
			elif line.startswith('TEXTTRANSFORM '):
				desc = line[14:].strip().split(None,1)
				batch_desc = [desc[0].replace('/','\\'),desc[1].replace('/','\\')]
				for suffix in suffixes:
					shell_lines.append('\tfor F in '+desc[1]+'.'+suffix+'; do let ++ACCEPT_TEST; echo $'+desc[0]+' $F; if $'+desc[0]+' $F | cmp - $F.txt; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;\n');
#					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @echo %'+batch_desc[0]+'% %%f & @%'+batch_desc[0]+'% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)\n')
#					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @set /a ACCEPT_TEST=ACCEPT_TEST+1\n')
			elif line.startswith('WARN '):
				desc = line[5:].strip().split(None,1)
				batch_desc = [desc[0].replace('/','\\'),desc[1].replace('/','\\')]
				for suffix in suffixes:
					shell_lines.append('\tfor F in '+desc[1]+'.'+suffix+'; do let ++REJECT_TEST; echo $'+desc[0]+' -Werror $F; if $'+desc[0]+' -Werror $F; then let ++BAD_PASS; BAD_PASS_NAME="$BAD_PASS_NAME $F"; else code_screen $? $F; fi; done;\n');
					shell_lines.append('\tfor F in '+desc[1]+'.'+suffix+'; do let ++ACCEPT_TEST; echo $'+desc[0]+' $F; if $'+desc[0]+' $F; then :; else code_screen $? $F; let ++FAILED; BAD_FAIL_NAME="$BAD_FAIL_NAME $F"; fi; done;\n');
					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @echo %'+batch_desc[0]+'% -Werror %%f & @%'+batch_desc[0]+'% -Werror %%f && (set /a BAD_PASS=BAD_PASS+1 & set BAD_PASS_NAME=%BAD_PASS_NAME% %%f)\n')
					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @echo %'+batch_desc[0]+'% %%f & @%'+batch_desc[0]+'% %%f || (set /a FAILED=FAILED+1 & set BAD_FAIL_NAME=%BAD_FAIL_NAME% %%f)\n')
					batch_lines.append('@for %%f in ('+batch_desc[1]+'.'+suffix+') do @(set /a ACCEPT_TEST=ACCEPT_TEST+1 & set /a REJECT_TEST=REJECT_TEST+1)\n')
			elif line.startswith('EPILOG'):
				shell_lines.append('\techo -E $BAD_PASS of $REJECT_TEST rejection tests accepted\n')
				shell_lines.append('\tif test -n "$BAD_PASS_NAME"; then echo -E $BAD_PASS_NAME; fi\n')
				shell_lines.append('\techo -E $FAILED of $ACCEPT_TEST acceptance tests rejected\n')
				shell_lines.append('\tif test -n "$BAD_FAIL_NAME"; then echo -E $BAD_FAIL_NAME; fi\n')
				shell_lines.append('\techo -E $ASSERT_FAILED tests failed by critical bugs\n')
				shell_lines.append('\tif test -n "$ASSERT_FAIL_NAME"; then echo -E $ASSERT_FAIL_NAME; fi\n')
				shell_lines.append('}\n\n')
				shell_lines.append('run_tests\n')
				batch_lines.append('@echo %BAD_PASS% of %REJECT_TEST% rejection tests accepted\n')
				batch_lines.append('@if not "%BAD_PASS_NAME%"=="LastAccepted:" @echo %BAD_PASS_NAME%\n')
				batch_lines.append('@echo %FAILED% of %ACCEPT_TEST% acceptance tests rejected\n')
				batch_lines.append('@if not "%BAD_FAIL_NAME%"=="LastRejected:" @echo %BAD_FAIL_NAME%\n')
			else:
				shell_lines.append(line);
				batch_lines.append(line);

	root_name = filename[:filename.rfind('.')]
	TargetFile = open(root_name+'.sh','w')	# *.sh
	TargetFile.writelines(shell_lines)
	TargetFile.close()

	TargetFile = open(root_name+'.bat','w')	# *.bat
	TargetFile.writelines(batch_lines)
	TargetFile.close()

if __name__ == '__main__':
	# online help
	if 1>=len(argv):
		print "Usage: filename [filename ...]"
		print "\tSyntax for contents of filename"
		print "\tSUFFIXES suffix [suffix]"
		print "\t\tsets file suffixes for tests.  Use anywhere."
		print "\tDESC [description]"
		print "\t\tinjects a comment line of description.  Use anywhere."
		print "\tPROLOG"
		print "\t\timplementation-specific prolog.  Use exactly once."
		print "\tVAR variable-name variable-definition"
		print "\t\tinjects a localized variable.  Use between PROLOG and EPILOG"
		print "\tECHO [informative text]"
		print "\t\trelays informative text"
		print "\n\tThe following three test all active suffixes at the time.\n\tUse between PROLOG and EPILOG."
		print "\tERROR var posix-relative-file-path"
		print "\t\tinjects test that requires failed return code to pass."
		print "\tPASS var posix-relative-file-path"
		print "\t\tinjects test that requires successful return code to pass."
		print "\tWARN var posix-relative-file-path"
		print "\t\tinjects test that requires successful return code to pass."
		print "\t\tinjects test that requires failed return code with -Werror\n\t\tto pass."
		print "\tEPILOG"
		print "\t\timplementation-specific epilog.  Use exactly once."
		exit(0)

	for filename in argv[1:]:
		SpawnTestDrivers(filename)

