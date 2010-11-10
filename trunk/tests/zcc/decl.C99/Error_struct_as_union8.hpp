// decl.C99\Error_struct_as_union8.hpp
// using struct as union
// (C)2010 Kenneth Boyd, license: MIT.txt

namespace test {

struct bad_test;

union bad_test {
	int x_factor;
};

}
