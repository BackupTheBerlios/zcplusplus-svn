// decl.C99\Error_enum_as_union2.h
// using enum as union
// (C)2010 Kenneth Boyd, license: MIT.txt

enum bad_test {
	x_factor = 1
};

union bad_test {
	int x_factor;
};

