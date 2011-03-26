// decl.C99\Pass_enum_def_decl.hpp
// (C)2009,2011 Kenneth Boyd, license: MIT.txt
// using singly defined enum

enum good_test {
	x_factor = 1
} y;

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

// ringing the changes on thread_local extern
extern thread_local enum good_test x21;
extern thread_local const enum good_test x22;
extern thread_local volatile enum good_test x23;
extern thread_local const volatile enum good_test x24;
extern thread_local volatile const enum good_test x25;
thread_local extern enum good_test x26;
thread_local extern const enum good_test x27;
thread_local extern volatile enum good_test x28;
thread_local extern const volatile enum good_test x29;
thread_local extern volatile const enum good_test x30;

// ringing the changes on thread_local static
static thread_local enum good_test x31;
static thread_local const enum good_test x32;
static thread_local volatile enum good_test x33;
static thread_local const volatile enum good_test x34;
static thread_local volatile const enum good_test x35;
thread_local static enum good_test x36;
thread_local static const enum good_test x37;
thread_local static volatile enum good_test x38;
thread_local static const volatile enum good_test x39;
thread_local static volatile const enum good_test x40;

// thread_local extern not in first two postions is deprecated, but legal
extern const thread_local enum good_test x41;
const extern thread_local enum good_test x42;
extern volatile thread_local enum good_test x43;
volatile extern thread_local enum good_test x44;
extern const thread_local volatile enum good_test x45;
extern const volatile thread_local enum good_test x46;
const extern thread_local volatile enum good_test x47;
const extern volatile thread_local enum good_test x48;
const volatile extern thread_local enum good_test x49;
extern volatile thread_local const enum good_test x50;
extern volatile const thread_local enum good_test x51;
volatile extern thread_local const enum good_test x52;
volatile extern const thread_local enum good_test x53;
volatile const extern thread_local enum good_test x54;
thread_local const extern enum good_test x55;
const thread_local extern enum good_test x56;
thread_local volatile extern enum good_test x57;
volatile thread_local extern enum good_test x58;
thread_local const extern volatile enum good_test x59;
thread_local const volatile extern enum good_test x60;
const thread_local extern volatile enum good_test x61;
const thread_local volatile extern enum good_test x62;
const volatile thread_local extern enum good_test x63;
thread_local volatile extern const enum good_test x64;
thread_local volatile const extern enum good_test x65;
volatile thread_local extern const enum good_test x66;
volatile thread_local const extern enum good_test x67;
volatile const thread_local extern enum good_test x68;

// thread_local static not in first two postions is deprecated, but legal
static const thread_local enum good_test x69;
const static thread_local enum good_test x70;
static volatile thread_local enum good_test x71;
volatile static thread_local enum good_test x72;
static const thread_local volatile enum good_test x73;
static const volatile thread_local enum good_test x74;
const static thread_local volatile enum good_test x75;
const static volatile thread_local enum good_test x76;
const volatile static thread_local enum good_test x77;
static volatile thread_local const enum good_test x78;
static volatile const thread_local enum good_test x79;
volatile static thread_local const enum good_test x80;
volatile static const thread_local enum good_test x81;
volatile const static thread_local enum good_test x82;
thread_local const static enum good_test x83;
const thread_local static enum good_test x84;
thread_local volatile static enum good_test x85;
volatile thread_local static enum good_test x86;
thread_local const static volatile enum good_test x87;
thread_local const volatile static enum good_test x88;
const thread_local static volatile enum good_test x89;
const thread_local volatile static enum good_test x90;
const volatile thread_local static enum good_test x91;
thread_local volatile static const enum good_test x92;
thread_local volatile const static enum good_test x93;
volatile thread_local static const enum good_test x94;
volatile thread_local const static enum good_test x95;
volatile const thread_local static enum good_test x96;

