// decl.C99\Error_union_runon_def.hpp
// using singly defined union
// (C)2011 Kenneth Boyd, license: MIT.txt

union bad_test {
	int x_factor;
};

// ringing the changes on extern
extern union bad_test x1
extern const union bad_test x2;
