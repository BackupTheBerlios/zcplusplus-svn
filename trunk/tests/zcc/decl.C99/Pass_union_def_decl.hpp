// decl.C99\Pass_union_def_decl.hpp
// using singly defined union
// (C)2010 Kenneth Boyd, license: MIT.txt

union good_test {
	int x_factor;
} y;

// exercise some declarations
union good_test x1;
const union good_test c1;
union good_test const c2;
volatile union good_test v1;
union good_test volatile v2;
const volatile union good_test cv1;
volatile const union good_test cv2;
const union good_test volatile cv3;
volatile union good_test const cv4;
union good_test const volatile cv5;
union good_test volatile const cv6;

