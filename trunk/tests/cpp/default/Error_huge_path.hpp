// tests/cpp/Error_huge_path.hpp
// include something whose length exceeds FILENAME_MAX
// note that this test can fail if FILENAME_MAX is unusually spacious
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "nonexistent_exceptionally_long_awful_path_to_header/nonexistent_exceptionally_long_awful_path_to_header/nonexistent_exceptionally_long_awful_path_to_header/nonexistent_exceptionally_long_awful_path_to_header/nonexistent_exceptionally_long_awful_path_to_header/nonexistent_exceptionally_long_awful_path_to_header"

