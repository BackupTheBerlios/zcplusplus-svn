// decl.C99\Warn_struct_forward_def_const_volatile4.h
// using singly defined struct
// (C)2010 Kenneth Boyd, license: MIT.txt

volatile struct good_test const;
struct good_test;

struct good_test {
	int x_factor;
};

