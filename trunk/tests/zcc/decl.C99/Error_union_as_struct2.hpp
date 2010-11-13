// decl.C99\Error_union_as_struct2.hpp
// using union as struct
// (C)2010 Kenneth Boyd, license: MIT.txt

union bad_test {
	int x_factor;
};

struct bad_test {
	int x_factor;
};

