// decl.C99\Warn_struct_forward_def_volatile4.h
// using singly defined struct
// (C)2010 Kenneth Boyd, license: MIT.txt

struct good_test;
struct good_test volatile;

struct good_test {
	int x_factor;
};

