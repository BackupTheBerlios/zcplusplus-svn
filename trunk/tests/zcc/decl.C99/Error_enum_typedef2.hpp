// decl.C99\Error_enum_typedef2.hpp
// using multiply defined enum
// (C)2010 Kenneth Boyd, license: MIT.txt

namespace test {

typedef int x_factor;

enum bad_test {
	x_factor = 1
}

}
