// decl.C99\Pass_union_def.h
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

// ringing the changes on _Thread_Local extern
extern _Thread_Local union good_test x21;
extern _Thread_Local const union good_test x22;
extern _Thread_Local volatile union good_test x23;
extern _Thread_Local const volatile union good_test x24;
extern _Thread_Local volatile const union good_test x25;
_Thread_Local extern union good_test x26;
_Thread_Local extern const union good_test x27;
_Thread_Local extern volatile union good_test x28;
_Thread_Local extern const volatile union good_test x29;
_Thread_Local extern volatile const union good_test x30;

// ringing the changes on _Thread_Local static
static _Thread_Local union good_test x31;
static _Thread_Local const union good_test x32;
static _Thread_Local volatile union good_test x33;
static _Thread_Local const volatile union good_test x34;
static _Thread_Local volatile const union good_test x35;
_Thread_Local static union good_test x36;
_Thread_Local static const union good_test x37;
_Thread_Local static volatile union good_test x38;
_Thread_Local static const volatile union good_test x39;
_Thread_Local static volatile const union good_test x40;

// _Thread_Local extern not in first two postions is deprecated, but legal
extern const _Thread_Local union good_test x41;
const extern _Thread_Local union good_test x42;
extern volatile _Thread_Local union good_test x43;
volatile extern _Thread_Local union good_test x44;
extern const _Thread_Local volatile union good_test x45;
extern const volatile _Thread_Local union good_test x46;
const extern _Thread_Local volatile union good_test x47;
const extern volatile _Thread_Local union good_test x48;
const volatile extern _Thread_Local union good_test x49;
extern volatile _Thread_Local const union good_test x50;
extern volatile const _Thread_Local union good_test x51;
volatile extern _Thread_Local const union good_test x52;
volatile extern const _Thread_Local union good_test x53;
volatile const extern _Thread_Local union good_test x54;
_Thread_Local const extern union good_test x55;
const _Thread_Local extern union good_test x56;
_Thread_Local volatile extern union good_test x57;
volatile _Thread_Local extern union good_test x58;
_Thread_Local const extern volatile union good_test x59;
_Thread_Local const volatile extern union good_test x60;
const _Thread_Local extern volatile union good_test x61;
const _Thread_Local volatile extern union good_test x62;
const volatile _Thread_Local extern union good_test x63;
_Thread_Local volatile extern const union good_test x64;
_Thread_Local volatile const extern union good_test x65;
volatile _Thread_Local extern const union good_test x66;
volatile _Thread_Local const extern union good_test x67;
volatile const _Thread_Local extern union good_test x68;

// _Thread_Local static not in first two postions is deprecated, but legal
static const _Thread_Local union good_test x69;
const static _Thread_Local union good_test x70;
static volatile _Thread_Local union good_test x71;
volatile static _Thread_Local union good_test x72;
static const _Thread_Local volatile union good_test x73;
static const volatile _Thread_Local union good_test x74;
const static _Thread_Local volatile union good_test x75;
const static volatile _Thread_Local union good_test x76;
const volatile static _Thread_Local union good_test x77;
static volatile _Thread_Local const union good_test x78;
static volatile const _Thread_Local union good_test x79;
volatile static _Thread_Local const union good_test x80;
volatile static const _Thread_Local union good_test x81;
volatile const static _Thread_Local union good_test x82;
_Thread_Local const static union good_test x83;
const _Thread_Local static union good_test x84;
_Thread_Local volatile static union good_test x85;
volatile _Thread_Local static union good_test x86;
_Thread_Local const static volatile union good_test x87;
_Thread_Local const volatile static union good_test x88;
const _Thread_Local static volatile union good_test x89;
const _Thread_Local volatile static union good_test x90;
const volatile _Thread_Local static union good_test x91;
_Thread_Local volatile static const union good_test x92;
_Thread_Local volatile const static union good_test x93;
volatile _Thread_Local static const union good_test x94;
volatile _Thread_Local const static union good_test x95;
volatile const _Thread_Local static union good_test x96;

// define-declares
// ringing the changes on extern
extern union good_test1 { int x_factor1; } x_1;
extern const union good_test2 { int x_factor2; } x_2;
extern volatile union good_test3 { int x_factor3; } x_3;
extern const volatile union good_test4 { int x_factor4; } x_4;
extern volatile const union good_test5 { int x_factor5; } x_5;

// ringing the changes on static
// (don't test static const -- no chance to initialize before use)
static union good_test6 { int x_factor6; } x_6;
static volatile union good_test7 { int x_factor7; } x_7;
static const volatile union good_test8 { int x_factor8; } x_8;
static volatile const union good_test9 { int x_factor9; } x_9;

// extern/static not in first postion is deprecated, but legal
const extern union good_test10 { int x_factor10; } x_10;
volatile extern union good_test11 { int x_factor11; } x_11;
const extern volatile union good_test12 { int x_factor12; } x_12;
const volatile extern union good_test13 { int x_factor13; } x_13;
volatile extern const union good_test14 { int x_factor14; } x_14;
volatile const extern union good_test15 { int x_factor15; } x_15;
volatile static union good_test16 { int x_factor16; } x_16;
const static volatile union good_test17 { int x_factor17; } x_17;
const volatile static union good_test18 { int x_factor18; } x_18;
volatile static const union good_test19 { int x_factor19; } x_19;
volatile const static union good_test20 { int x_factor20; } x_20;

// ringing the changes on _Thread_Local extern
extern _Thread_Local union good_test21 { int x_factor21; } x_21;
extern _Thread_Local const union good_test22 { int x_factor22; } x_22;
extern _Thread_Local volatile union good_test23 { int x_factor23; } x_23;
extern _Thread_Local const volatile union good_test24 { int x_factor24; } x_24;
extern _Thread_Local volatile const union good_test25 { int x_factor25; } x_25;
_Thread_Local extern union good_test26 { int x_factor26; } x_26;
_Thread_Local extern const union good_test27 { int x_factor27; } x_27;
_Thread_Local extern volatile union good_test28 { int x_factor28; } x_28;
_Thread_Local extern const volatile union good_test29 { int x_factor29; } x_29;
_Thread_Local extern volatile const union good_test30 { int x_factor30; } x_30;

// ringing the changes on _Thread_Local static
static _Thread_Local union good_test31 { int x_factor31; } x_31;
static _Thread_Local const union good_test32 { int x_factor32; } x_32;
static _Thread_Local volatile union good_test33 { int x_factor33; } x_33;
static _Thread_Local const volatile union good_test34 { int x_factor34; } x_34;
static _Thread_Local volatile const union good_test35 { int x_factor35; } x_35;
_Thread_Local static union good_test36 { int x_factor36; } x_36;
_Thread_Local static const union good_test37 { int x_factor37; } x_37;
_Thread_Local static volatile union good_test38 { int x_factor38; } x_38;
_Thread_Local static const volatile union good_test39 { int x_factor39; } x_39;
_Thread_Local static volatile const union good_test40 { int x_factor40; } x_40;

// _Thread_Local extern not in first two postions is deprecated, but legal
extern const _Thread_Local union good_test41 { int x_factor41; } x_41;
const extern _Thread_Local union good_test42 { int x_factor42; } x_42;
extern volatile _Thread_Local union good_test43 { int x_factor43; } x_43;
volatile extern _Thread_Local union good_test44 { int x_factor44; } x_44;
extern const _Thread_Local volatile union good_test45 { int x_factor45; } x_45;
extern const volatile _Thread_Local union good_test46 { int x_factor46; } x_46;
const extern _Thread_Local volatile union good_test47 { int x_factor47; } x_47;
const extern volatile _Thread_Local union good_test48 { int x_factor48; } x_48;
const volatile extern _Thread_Local union good_test49 { int x_factor49; } x_49;
extern volatile _Thread_Local const union good_test50 { int x_factor50; } x_50;
extern volatile const _Thread_Local union good_test51 { int x_factor51; } x_51;
volatile extern _Thread_Local const union good_test52 { int x_factor52; } x_52;
volatile extern const _Thread_Local union good_test53 { int x_factor53; } x_53;
volatile const extern _Thread_Local union good_test54 { int x_factor54; } x_54;
_Thread_Local const extern union good_test55 { int x_factor55; } x_55;
const _Thread_Local extern union good_test56 { int x_factor56; } x_56;
_Thread_Local volatile extern union good_test57 { int x_factor57; } x_57;
volatile _Thread_Local extern union good_test58 { int x_factor58; } x_58;
_Thread_Local const extern volatile union good_test59 { int x_factor59; } x_59;
_Thread_Local const volatile extern union good_test60 { int x_factor60; } x_60;
const _Thread_Local extern volatile union good_test61 { int x_factor61; } x_61;
const _Thread_Local volatile extern union good_test62 { int x_factor62; } x_62;
const volatile _Thread_Local extern union good_test63 { int x_factor63; } x_63;
_Thread_Local volatile extern const union good_test64 { int x_factor64; } x_64;
_Thread_Local volatile const extern union good_test65 { int x_factor65; } x_65;
volatile _Thread_Local extern const union good_test66 { int x_factor66; } x_66;
volatile _Thread_Local const extern union good_test67 { int x_factor67; } x_67;
volatile const _Thread_Local extern union good_test68 { int x_factor68; } x_68;

// _Thread_Local static not in first two postions is deprecated, but legal
static const _Thread_Local union good_test69 { int x_factor69; } x_69;
const static _Thread_Local union good_test70 { int x_factor70; } x_70;
static volatile _Thread_Local union good_test71 { int x_factor71; } x_71;
volatile static _Thread_Local union good_test72 { int x_factor72; } x_72;
static const _Thread_Local volatile union good_test73 { int x_factor73; } x_73;
static const volatile _Thread_Local union good_test74 { int x_factor74; } x_74;
const static _Thread_Local volatile union good_test75 { int x_factor75; } x_75;
const static volatile _Thread_Local union good_test76 { int x_factor76; } x_76;
const volatile static _Thread_Local union good_test77 { int x_factor77; } x_77;
static volatile _Thread_Local const union good_test78 { int x_factor78; } x_78;
static volatile const _Thread_Local union good_test79 { int x_factor79; } x_79;
volatile static _Thread_Local const union good_test80 { int x_factor80; } x_80;
volatile static const _Thread_Local union good_test81 { int x_factor81; } x_81;
volatile const static _Thread_Local union good_test82 { int x_factor82; } x_82;
_Thread_Local const static union good_test83 { int x_factor83; } x_83;
const _Thread_Local static union good_test84 { int x_factor84; } x_84;
_Thread_Local volatile static union good_test85 { int x_factor85; } x_85;
volatile _Thread_Local static union good_test86 { int x_factor86; } x_86;
_Thread_Local const static volatile union good_test87 { int x_factor87; } x_87;
_Thread_Local const volatile static union good_test88 { int x_factor88; } x_88;
const _Thread_Local static volatile union good_test89 { int x_factor89; } x_89;
const _Thread_Local volatile static union good_test90 { int x_factor90; } x_90;
const volatile _Thread_Local static union good_test91 { int x_factor91; } x_91;
_Thread_Local volatile static const union good_test92 { int x_factor92; } x_92;
_Thread_Local volatile const static union good_test93 { int x_factor93; } x_93;
volatile _Thread_Local static const union good_test94 { int x_factor94; } x_94;
volatile _Thread_Local const static union good_test95 { int x_factor95; } x_95;
volatile const _Thread_Local static union good_test96 { int x_factor96; } x_96;

