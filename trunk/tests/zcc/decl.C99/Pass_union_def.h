// decl.C99\Pass_union_def.h
// using singly defined union
// (C)2009,2011 Kenneth Boyd, license: MIT.txt

union good_test {
	int x_factor;
};

// ringing the changes on extern
extern union good_test x1;
extern const union good_test x2;
extern volatile union good_test x3;
extern const volatile union good_test x4;
extern volatile const union good_test x5;

// ringing the changes on static
// (don't test static const -- no chance to initialize before use)
static union good_test x6;
static volatile union good_test x7;
static const volatile union good_test x8;
static volatile const union good_test x9;

// extern/static not in first postion is deprecated, but legal
const extern union good_test x10;
volatile extern union good_test x11;
const extern volatile union good_test x12;
const volatile extern union good_test x13;
volatile extern const union good_test x14;
volatile const extern union good_test x15;

volatile static union good_test x16;
const static volatile union good_test x17;
const volatile static union good_test x18;
volatile static const union good_test x19;
volatile const static union good_test x20;

// define-declares
// ringing the changes on extern
extern union good_test2 { int x_factor2; } x21;
extern const union good_test3 { int x_factor3; } x22;
extern volatile union good_test4 { int x_factor4; } x23;
extern const volatile union good_test5 { int x_factor5; } x24;
extern volatile const union good_test6 { int x_factor6; } x25;

// ringing the changes on static
// (don't test static const -- no chance to initialize before use)
static union good_test7 { int x_factor7; } x26;
static volatile union good_test8 { int x_factor8; } x27;
static const volatile union good_test9 { int x_factor9; } x28;
static volatile const union good_test10 { int x_factor10; } x29;

// extern/static not in first postion is deprecated, but legal
const extern union good_test11 { int x_factor11; } x30;
volatile extern union good_test12 { int x_factor12; } x31;
const extern volatile union good_test13 { int x_factor13; } x32;
const volatile extern union good_test14 { int x_factor14; } x33;
volatile extern const union good_test15 { int x_factor15; } x34;
volatile const extern union good_test16 { int x_factor16; } x35;

volatile static union good_test17 { int x_factor17; } x16;
const static volatile union good_test18 { int x_factor18; } x37;
const volatile static union good_test19 { int x_factor19; } x38;
volatile static const union good_test20 { int x_factor20; } x39;
volatile const static union good_test21 { int x_factor21; } x40;

