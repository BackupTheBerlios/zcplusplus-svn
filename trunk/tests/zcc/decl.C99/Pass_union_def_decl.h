// decl.C99\Pass_union_def_decl.h
// (C)2009,2011 Kenneth Boyd, license: MIT.txt
// using singly defined union

union good_test {
	int x_factor
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

// ringing the changes on _Thread_local extern
extern _Thread_local union good_test x21;
extern _Thread_local const union good_test x22;
extern _Thread_local volatile union good_test x23;
extern _Thread_local const volatile union good_test x24;
extern _Thread_local volatile const union good_test x25;
_Thread_local extern union good_test x26;
_Thread_local extern const union good_test x27;
_Thread_local extern volatile union good_test x28;
_Thread_local extern const volatile union good_test x29;
_Thread_local extern volatile const union good_test x30;

// ringing the changes on _Thread_local static
static _Thread_local union good_test x31;
static _Thread_local const union good_test x32;
static _Thread_local volatile union good_test x33;
static _Thread_local const volatile union good_test x34;
static _Thread_local volatile const union good_test x35;
_Thread_local static union good_test x36;
_Thread_local static const union good_test x37;
_Thread_local static volatile union good_test x38;
_Thread_local static const volatile union good_test x39;
_Thread_local static volatile const union good_test x40;

// _Thread_local extern not in first two postions is deprecated, but legal
extern const _Thread_local union good_test x41;
const extern _Thread_local union good_test x42;
extern volatile _Thread_local union good_test x43;
volatile extern _Thread_local union good_test x44;
extern const _Thread_local volatile union good_test x45;
extern const volatile _Thread_local union good_test x46;
const extern _Thread_local volatile union good_test x47;
const extern volatile _Thread_local union good_test x48;
const volatile extern _Thread_local union good_test x49;
extern volatile _Thread_local const union good_test x50;
extern volatile const _Thread_local union good_test x51;
volatile extern _Thread_local const union good_test x52;
volatile extern const _Thread_local union good_test x53;
volatile const extern _Thread_local union good_test x54;
_Thread_local const extern union good_test x55;
const _Thread_local extern union good_test x56;
_Thread_local volatile extern union good_test x57;
volatile _Thread_local extern union good_test x58;
_Thread_local const extern volatile union good_test x59;
_Thread_local const volatile extern union good_test x60;
const _Thread_local extern volatile union good_test x61;
const _Thread_local volatile extern union good_test x62;
const volatile _Thread_local extern union good_test x63;
_Thread_local volatile extern const union good_test x64;
_Thread_local volatile const extern union good_test x65;
volatile _Thread_local extern const union good_test x66;
volatile _Thread_local const extern union good_test x67;
volatile const _Thread_local extern union good_test x68;

// _Thread_local static not in first two postions is deprecated, but legal
static const _Thread_local union good_test x69;
const static _Thread_local union good_test x70;
static volatile _Thread_local union good_test x71;
volatile static _Thread_local union good_test x72;
static const _Thread_local volatile union good_test x73;
static const volatile _Thread_local union good_test x74;
const static _Thread_local volatile union good_test x75;
const static volatile _Thread_local union good_test x76;
const volatile static _Thread_local union good_test x77;
static volatile _Thread_local const union good_test x78;
static volatile const _Thread_local union good_test x79;
volatile static _Thread_local const union good_test x80;
volatile static const _Thread_local union good_test x81;
volatile const static _Thread_local union good_test x82;
_Thread_local const static union good_test x83;
const _Thread_local static union good_test x84;
_Thread_local volatile static union good_test x85;
volatile _Thread_local static union good_test x86;
_Thread_local const static volatile union good_test x87;
_Thread_local const volatile static union good_test x88;
const _Thread_local static volatile union good_test x89;
const _Thread_local volatile static union good_test x90;
const volatile _Thread_local static union good_test x91;
_Thread_local volatile static const union good_test x92;
_Thread_local volatile const static union good_test x93;
volatile _Thread_local static const union good_test x94;
volatile _Thread_local const static union good_test x95;
volatile const _Thread_local static union good_test x96;

