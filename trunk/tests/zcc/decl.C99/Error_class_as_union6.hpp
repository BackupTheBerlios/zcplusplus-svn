// decl.C99\Error_class_as_union6.hpp
// using class as union
// (C)2010 Kenneth Boyd, license: MIT.txt

namespace test {

class bad_test {
	int x_factor;
};

union bad_test {
	int x_factor;
};

}
