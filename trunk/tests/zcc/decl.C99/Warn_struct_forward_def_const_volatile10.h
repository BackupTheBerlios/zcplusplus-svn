// decl.C99\Warn_struct_forward_def_const_volatile10.h
// struct singly defined union
// (C)2010 Kenneth Boyd, license: MIT.txt

struct good_test;
volatile struct good_test const;

struct good_test {
	int x_factor;
};

