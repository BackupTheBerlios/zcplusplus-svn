// decl.C99\Warn_struct_forward_def_const_volatile8.h
// using singly defined struct
// (C)2010 Kenneth Boyd, license: MIT.txt

volatile const struct good_test;
struct good_test;

struct good_test {
	int x_factor;
};

