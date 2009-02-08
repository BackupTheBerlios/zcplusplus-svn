Installation instructions with GNU make
1) Adjust makeconf.inc to have the correct values for your system.
2) make
3) using a Bourne shell in the tests/cpp subdirectory:
* ./run_tests.sh to check expected behavior
** C99 conformance with -pedantic option
** default non-conforming corner-case handling (intercept errors early)
* ./run_tests_C99.sh is also present to check only C99 conformance; changing the preprocessor line here should allow checking other preprocessors for C99 conformance.
* Both shell scripts will count: accepted rejection tests, rejected acceptance tests, and tests that were rejected because an assertion went off.  (The last assumes 
compiling without NDEBUG, and that the exit code for an assertion is 3.)  All tests that have unexpected behavior should be listed.
3a) Windows batch files run_tests.bat and run_tests_C99.bat are also provided for use with the Windows command processor, with mostly-comparable meaning.
* They do not have the assertion-count capability.
* They only list the last test case in each category that had unexpected behavior.

4) make install [to be implemented before release]
