// decl.C99\Error_union_multidef.hpp
// using multiply defined union
// (C)2009 Kenneth Boyd, license: MIT.txt

union bad_test {
	int x_factor;
};

union bad_test {
	int x_factor;
};

