Installation instructions with GNU make for MingW32 (Official 3.4.5, official 4.2.1, or TDM 4.3.3)
0) Install the Boost libraries ( http://www.boost.org/ ) first.

1) Adjust POSIX.inc and Zaimoni.STL to have the correct values for your system.  Also adjust 
   the INSTALL_TO macro in CPreproc.cpp to point to the root build directory.
2) make
3) using a Bourne shell in the tests/cpp subdirectory:
* ./run_tests.sh to check expected behavior
** C99 conformance with -pedantic option
** default non-conforming corner-case handling (intercept errors early)
* ./run_tests_C99.sh is also present to check only C99 conformance; changing 
  the preprocessor line here should allow checking other preprocessors for 
  C99 conformance.
* Both shell scripts will count: accepted rejection tests, rejected 
  acceptance tests, and tests that were rejected because an assertion went off.  
  (The last assumes compiling without NDEBUG, and that the exit code for 
  an assertion is 3.)  All tests that have unexpected behavior should be listed.
3a) Windows batch files run_tests.bat and run_tests_C99.bat are also provided 
  for use with the Windows command processor, with mostly-comparable meaning.
* They do not have the assertion-count capability.
* They only list the last test case in each category that had unexpected behavior.
* They do not run the content-comparison tests.

4) make install [to be implemented]

Installation instructions with Microsoft NMake for Visual C++ 2008 Express, 32-bit
-1) Install the Boost libraries ( http://www.boost.org/ ) first.
0) Install the Zaimoni.STL directory second.
0.1) Adjust NMake.inc to have the correct values for your system.  The files in MSVC32.zcc must be on an include path, they are C99 compatibility glue.
0.2) nmake /F NMakefile [this is incomplete currently compared to GNU Make]
0.3) nmake /F NMakefile install if there were no errors

1) Adjust NMake.inc to have the correct values for your system.  Also adjust 
   the INSTALL_TO macro in CPreproc.cpp to point to the root build directory.
2) nmake /F NMakefile [to be implemented]
3) using a Bourne shell in the tests/cpp subdirectory:
* ./run_tests.sh to check expected behavior
** C99 conformance with -pedantic option
** default non-conforming corner-case handling (intercept errors early)
* ./run_tests_C99.sh is also present to check only C99 conformance; changing 
  the preprocessor line here should allow checking other preprocessors for 
  C99 conformance.
* Both shell scripts will count: accepted rejection tests, rejected 
  acceptance tests, and tests that were rejected because an assertion went off.  
  (The last assumes compiling without NDEBUG, and that the exit code for 
  an assertion is 3.)  All tests that have unexpected behavior should be listed.
3a) Windows batch files run_tests.bat and run_tests_C99.bat are also provided 
  for use with the Windows command processor, with mostly-comparable meaning.
* They do not have the assertion-count capability.
* They only list the last test case in each category that had unexpected behavior.
* They do not run the content-comparison tests.

4) nmake /F NMakefile install [to be implemented]
