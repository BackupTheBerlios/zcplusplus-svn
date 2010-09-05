// decl.C99\Pass_struct_def.hpp
// using singly defined struct
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

struct good_test {
	int x_factor;
};

// exercise some declarations
struct good_test x1;
const struct good_test c1;
struct good_test const c2;
volatile struct good_test v1;
struct good_test volatile v2;
const volatile struct good_test cv1;
volatile const struct good_test cv2;
const struct good_test volatile cv3;
volatile struct good_test const cv4;
struct good_test const volatile cv5;
struct good_test volatile const cv6;
