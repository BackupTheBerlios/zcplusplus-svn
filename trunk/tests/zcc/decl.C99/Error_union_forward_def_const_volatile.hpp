// decl.C99\Error_union_forward_def_const_volatile.hpp
// using singly defined union
// (C)2010 Kenneth Boyd, license: MIT.txt

const volatile union good_test;
union good_test;

union good_test {
	int x_factor;
};

