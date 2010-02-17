// decl.C99\Error_typedef_enum.hpp
// using multiply defined typedef
// (C)2010 Kenneth Boyd, license: MIT.txt

enum bad_test {
	x_factor = 1
}

typedef int x_factor;
