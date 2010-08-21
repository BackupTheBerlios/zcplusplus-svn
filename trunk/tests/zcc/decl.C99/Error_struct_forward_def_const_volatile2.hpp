// decl.C99\Error_struct_forward_def_const_volatile2.hpp
// using singly defined struct
// (C)2010 Kenneth Boyd, license: MIT.txt

volatile const struct good_test;
struct good_test;

struct good_test {
	int x_factor;
};
