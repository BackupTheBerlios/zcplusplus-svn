// decl.C99\Error_struct_as_union2.h
// using struct as union
// (C)2010 Kenneth Boyd, license: MIT.txt

struct bad_test {
	int x_factor;
};

union bad_test {
	int x_factor;
};

