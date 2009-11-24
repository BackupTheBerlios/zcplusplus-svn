// decl.C99\Error_enum_multidef.h
// using multiply defined enum
// (C)2009 Kenneth Boyd, license: MIT.txt

enum bad_test {
	x_factor = 1
}

enum bad_test {
	x_factor = 1
}

