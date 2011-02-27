// decl.C99\Pass_enum_def.hpp
// (C)2009,2011 Kenneth Boyd, license: MIT.txt
// using singly defined enum

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

// define-declares
// ringing the changes on extern
extern enum good_test1 { x_factor1 = 1 } x_1;
extern const enum good_test2 { x_factor2 = 1 } x_2;
extern volatile enum good_test3 { x_factor3 = 1 } x_3;
extern const volatile enum good_test4 { x_factor4 = 1 } x_4;
extern volatile const enum good_test5 { x_factor5 = 1 } x_5;

// ringing the changes on static
// (don't test static const -- no chance to initialize before use)
static enum good_test6 { x_factor6 = 1 } x_6;
static volatile enum good_test7 { x_factor7 = 1 } x_7;
static const volatile enum good_test8 { x_factor8 = 1 } x_8;
static volatile const enum good_test9 { x_factor9 = 1 } x_9;

// extern/static not in first postion is deprecated, but legal
const extern enum good_test10 { x_factor10 = 1 } x_10;
volatile extern enum good_test11 { x_factor11 = 1 } x_11;
const extern volatile enum good_test12 { x_factor12 = 1 } x_12;
const volatile extern enum good_test13 { x_factor13 = 1 } x_13;
volatile extern const enum good_test14 { x_factor14 = 1 } x_14;
volatile const extern enum good_test15 { x_factor15 = 1 } x_15;
volatile static enum good_test16 { x_factor16 = 1 } x_16;
const static volatile enum good_test17 { x_factor17 = 1 } x_17;
const volatile static enum good_test18 { x_factor18 = 1 } x_18;
volatile static const enum good_test19 { x_factor19 = 1 } x_19;
volatile const static enum good_test20 { x_factor20 = 1 } x_20;

// ringing the changes on thread_local extern
extern thread_local enum good_test21 { x_factor21 = 1 } x_21;
extern thread_local const enum good_test22 { x_factor22 = 1 } x_22;
extern thread_local volatile enum good_test23 { x_factor23 = 1 } x_23;
extern thread_local const volatile enum good_test24 { x_factor24 = 1 } x_24;
extern thread_local volatile const enum good_test25 { x_factor25 = 1 } x_25;
thread_local extern enum good_test26 { x_factor26 = 1 } x_26;
thread_local extern const enum good_test27 { x_factor27 = 1 } x_27;
thread_local extern volatile enum good_test28 { x_factor28 = 1 } x_28;
thread_local extern const volatile enum good_test29 { x_factor29 = 1 } x_29;
thread_local extern volatile const enum good_test30 { x_factor30 = 1 } x_30;

// ringing the changes on thread_local static
static thread_local enum good_test31 { x_factor31 = 1 } x_31;
static thread_local const enum good_test32 { x_factor32 = 1 } x_32;
static thread_local volatile enum good_test33 { x_factor33 = 1 } x_33;
static thread_local const volatile enum good_test34 { x_factor34 = 1 } x_34;
static thread_local volatile const enum good_test35 { x_factor35 = 1 } x_35;
thread_local static enum good_test36 { x_factor36 = 1 } x_36;
thread_local static const enum good_test37 { x_factor37 = 1 } x_37;
thread_local static volatile enum good_test38 { x_factor38 = 1 } x_38;
thread_local static const volatile enum good_test39 { x_factor39 = 1 } x_39;
thread_local static volatile const enum good_test40 { x_factor40 = 1 } x_40;

// thread_local extern not in first two postions is deprecated, but legal
extern const thread_local enum good_test41 { x_factor41 = 1 } x_41;
const extern thread_local enum good_test42 { x_factor42 = 1 } x_42;
extern volatile thread_local enum good_test43 { x_factor43 = 1 } x_43;
volatile extern thread_local enum good_test44 { x_factor44 = 1 } x_44;
extern const thread_local volatile enum good_test45 { x_factor45 = 1 } x_45;
extern const volatile thread_local enum good_test46 { x_factor46 = 1 } x_46;
const extern thread_local volatile enum good_test47 { x_factor47 = 1 } x_47;
const extern volatile thread_local enum good_test48 { x_factor48 = 1 } x_48;
const volatile extern thread_local enum good_test49 { x_factor49 = 1 } x_49;
extern volatile thread_local const enum good_test50 { x_factor50 = 1 } x_50;
extern volatile const thread_local enum good_test51 { x_factor51 = 1 } x_51;
volatile extern thread_local const enum good_test52 { x_factor52 = 1 } x_52;
volatile extern const thread_local enum good_test53 { x_factor53 = 1 } x_53;
volatile const extern thread_local enum good_test54 { x_factor54 = 1 } x_54;
thread_local const extern enum good_test55 { x_factor55 = 1 } x_55;
const thread_local extern enum good_test56 { x_factor56 = 1 } x_56;
thread_local volatile extern enum good_test57 { x_factor57 = 1 } x_57;
volatile thread_local extern enum good_test58 { x_factor58 = 1 } x_58;
thread_local const extern volatile enum good_test59 { x_factor59 = 1 } x_59;
thread_local const volatile extern enum good_test60 { x_factor60 = 1 } x_60;
const thread_local extern volatile enum good_test61 { x_factor61 = 1 } x_61;
const thread_local volatile extern enum good_test62 { x_factor62 = 1 } x_62;
const volatile thread_local extern enum good_test63 { x_factor63 = 1 } x_63;
thread_local volatile extern const enum good_test64 { x_factor64 = 1 } x_64;
thread_local volatile const extern enum good_test65 { x_factor65 = 1 } x_65;
volatile thread_local extern const enum good_test66 { x_factor66 = 1 } x_66;
volatile thread_local const extern enum good_test67 { x_factor67 = 1 } x_67;
volatile const thread_local extern enum good_test68 { x_factor68 = 1 } x_68;

// thread_local static not in first two postions is deprecated, but legal
static const thread_local enum good_test69 { x_factor69 = 1 } x_69;
const static thread_local enum good_test70 { x_factor70 = 1 } x_70;
static volatile thread_local enum good_test71 { x_factor71 = 1 } x_71;
volatile static thread_local enum good_test72 { x_factor72 = 1 } x_72;
static const thread_local volatile enum good_test73 { x_factor73 = 1 } x_73;
static const volatile thread_local enum good_test74 { x_factor74 = 1 } x_74;
const static thread_local volatile enum good_test75 { x_factor75 = 1 } x_75;
const static volatile thread_local enum good_test76 { x_factor76 = 1 } x_76;
const volatile static thread_local enum good_test77 { x_factor77 = 1 } x_77;
static volatile thread_local const enum good_test78 { x_factor78 = 1 } x_78;
static volatile const thread_local enum good_test79 { x_factor79 = 1 } x_79;
volatile static thread_local const enum good_test80 { x_factor80 = 1 } x_80;
volatile static const thread_local enum good_test81 { x_factor81 = 1 } x_81;
volatile const static thread_local enum good_test82 { x_factor82 = 1 } x_82;
thread_local const static enum good_test83 { x_factor83 = 1 } x_83;
const thread_local static enum good_test84 { x_factor84 = 1 } x_84;
thread_local volatile static enum good_test85 { x_factor85 = 1 } x_85;
volatile thread_local static enum good_test86 { x_factor86 = 1 } x_86;
thread_local const static volatile enum good_test87 { x_factor87 = 1 } x_87;
thread_local const volatile static enum good_test88 { x_factor88 = 1 } x_88;
const thread_local static volatile enum good_test89 { x_factor89 = 1 } x_89;
const thread_local volatile static enum good_test90 { x_factor90 = 1 } x_90;
const volatile thread_local static enum good_test91 { x_factor91 = 1 } x_91;
thread_local volatile static const enum good_test92 { x_factor92 = 1 } x_92;
thread_local volatile const static enum good_test93 { x_factor93 = 1 } x_93;
volatile thread_local static const enum good_test94 { x_factor94 = 1 } x_94;
volatile thread_local const static enum good_test95 { x_factor95 = 1 } x_95;
volatile const thread_local static enum good_test96 { x_factor96 = 1 } x_96;

