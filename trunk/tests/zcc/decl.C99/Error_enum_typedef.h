// decl.C99\Error_enum_typedef.h
// using multiply defined enum
// (C)2010 Kenneth Boyd, license: MIT.txt

typedef int x_factor;

enum bad_test {
	x_factor = 1
}

