// decl.C99\Pass_union_anon_def_decl.h
// using singly defined union
// (C)2010 Kenneth Boyd, license: MIT.txt

// exercise some declarations
union {int x_factor;} x1;
const union {int x_factor;} c1;
union {int x_factor;} const c2;
volatile union {int x_factor;} v1;
union {int x_factor;} volatile v2;
const volatile union {int x_factor;} cv1;
volatile const union {int x_factor;} cv2;
const union {int x_factor;} volatile cv3;
volatile union {int x_factor;} const cv4;
union {int x_factor;} const volatile cv5;
union {int x_factor;} volatile const cv6;
