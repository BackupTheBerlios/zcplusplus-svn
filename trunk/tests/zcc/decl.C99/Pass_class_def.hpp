// decl.C99\Pass_class_def.hpp
// using singly defined struct
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

class good_test {
	int x_factor;
};

// exercise some declarations
class good_test x1;
const class good_test c1;
class good_test const c2;
volatile class good_test v1;
class good_test volatile v2;
const volatile class good_test cv1;
volatile const class good_test cv2;
const class good_test volatile cv3;
volatile class good_test const cv4;
class good_test const volatile cv5;
class good_test volatile const cv6;
