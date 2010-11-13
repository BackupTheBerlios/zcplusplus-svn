// decl.C99\Error_enum_as_struct2.hpp
// using enum as struct
// (C)2010 Kenneth Boyd, license: MIT.txt

enum bad_test {
	x_factor = 1
};

struct bad_test {
	int x_factor;
};

