// decl.C99\Pass_enum_def.h
// using singly defined enum
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

enum good_test {
	x_factor = 1
};

// ringing the changes on extern
extern enum good_test x1;
extern const enum good_test x2;
extern volatile enum good_test x3;
extern const volatile enum good_test x4;
extern volatile const enum good_test x5;

// ringing the changes on static
// (don't test static const -- no chance to initialize before use)
static enum good_test x6;
static volatile enum good_test x7;
static const volatile enum good_test x8;
static volatile const enum good_test x9;

// extern/static not in first postion is deprecated, but legal
const extern enum good_test x10;
volatile extern enum good_test x11;
const extern volatile enum good_test x12;
const volatile extern enum good_test x13;
volatile extern const enum good_test x14;
volatile const extern enum good_test x15;

volatile static enum good_test x16;
const static volatile enum good_test x17;
const volatile static enum good_test x18;
volatile static const enum good_test x19;
volatile const static enum good_test x20;

// define-declares
// ringing the changes on extern
extern enum good_test2 { x_factor2 = 1 } x21;
extern const enum good_test3 { x_factor3 = 1 } x22;
extern volatile enum good_test4 { x_factor4 = 1 } x23;
extern const volatile enum good_test5 { x_factor5 = 1 } x24;
extern volatile const enum good_test6 { x_factor6 = 1 } x25;

// ringing the changes on static
// (don't test static const -- no chance to initialize before use)
static enum good_test7 { x_factor7 = 1 } x26;
static volatile enum good_test8 { x_factor8 = 1 } x27;
static const volatile enum good_test9 { x_factor9 = 1 } x28;
static volatile const enum good_test10 { x_factor10 = 1 } x29;

// extern/static not in first postion is deprecated, but legal
const extern enum good_test11 { x_factor11 = 1 } x30;
volatile extern enum good_test12 { x_factor12 = 1 } x31;
const extern volatile enum good_test13 { x_factor13 = 1 } x32;
const volatile extern enum good_test14 { x_factor14 = 1 } x33;
volatile extern const enum good_test15 { x_factor15 = 1 } x34;
volatile const extern enum good_test16 { x_factor16 = 1 } x35;

volatile static enum good_test17 { x_factor17 = 1 } x16;
const static volatile enum good_test18 { x_factor18 = 1 } x37;
const volatile static enum good_test19 { x_factor19 = 1 } x38;
volatile static const enum good_test20 { x_factor20 = 1 } x39;
volatile const static enum good_test21 { x_factor21 = 1 } x40;

