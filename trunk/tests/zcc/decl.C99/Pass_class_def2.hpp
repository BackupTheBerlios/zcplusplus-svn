// decl.C99/Pass_class_def2.hpp
// (C)2009,2011 Kenneth Boyd, license: MIT.txt
// using singly defined class

class good_test {
	int x_factor
};

// this section checks that suppressing keyword works
// ringing the changes on extern
extern good_test x1;
extern const good_test x2;
extern volatile good_test x3;
extern const volatile good_test x4;
extern volatile const good_test x5;

// ringing the changes on static
static good_test x6;
static const good_test x7;
static volatile good_test x8;
static const volatile good_test x9;
static volatile const good_test x10;

// extern/static not in first position is deprecated, but legal
const extern good_test x11;
volatile extern good_test x12;
const extern volatile good_test x13;
const volatile extern good_test x14;
volatile extern const good_test x15;
volatile const extern good_test x16;
const static good_test x17;
volatile static good_test x18;
const static volatile good_test x19;
const volatile static good_test x20;
volatile static const good_test x21;
volatile const static good_test x22;

// ringing the changes on thread_local extern
extern thread_local good_test x23;
extern thread_local const good_test x24;
extern thread_local volatile good_test x25;
extern thread_local const volatile good_test x26;
extern thread_local volatile const good_test x27;
thread_local extern good_test x28;
thread_local extern const good_test x29;
thread_local extern volatile good_test x30;
thread_local extern const volatile good_test x31;
thread_local extern volatile const good_test x32;

// ringing the changes on thread_local static
static thread_local good_test x33;
static thread_local const good_test x34;
static thread_local volatile good_test x35;
static thread_local const volatile good_test x36;
static thread_local volatile const good_test x37;
thread_local static good_test x38;
thread_local static const good_test x39;
thread_local static volatile good_test x40;
thread_local static const volatile good_test x41;
thread_local static volatile const good_test x42;

// thread_local extern not in first two positions is deprecated, but legal
extern const thread_local good_test x43;
const extern thread_local good_test x44;
extern volatile thread_local good_test x45;
volatile extern thread_local good_test x46;
extern const thread_local volatile good_test x47;
extern const volatile thread_local good_test x48;
const extern thread_local volatile good_test x49;
const extern volatile thread_local good_test x50;
const volatile extern thread_local good_test x51;
extern volatile thread_local const good_test x52;
extern volatile const thread_local good_test x53;
volatile extern thread_local const good_test x54;
volatile extern const thread_local good_test x55;
volatile const extern thread_local good_test x56;
thread_local const extern good_test x57;
const thread_local extern good_test x58;
thread_local volatile extern good_test x59;
volatile thread_local extern good_test x60;
thread_local const extern volatile good_test x61;
thread_local const volatile extern good_test x62;
const thread_local extern volatile good_test x63;
const thread_local volatile extern good_test x64;
const volatile thread_local extern good_test x65;
thread_local volatile extern const good_test x66;
thread_local volatile const extern good_test x67;
volatile thread_local extern const good_test x68;
volatile thread_local const extern good_test x69;
volatile const thread_local extern good_test x70;

// thread_local static not in first two positions is deprecated, but legal
static const thread_local good_test x71;
const static thread_local good_test x72;
static volatile thread_local good_test x73;
volatile static thread_local good_test x74;
static const thread_local volatile good_test x75;
static const volatile thread_local good_test x76;
const static thread_local volatile good_test x77;
const static volatile thread_local good_test x78;
const volatile static thread_local good_test x79;
static volatile thread_local const good_test x80;
static volatile const thread_local good_test x81;
volatile static thread_local const good_test x82;
volatile static const thread_local good_test x83;
volatile const static thread_local good_test x84;
thread_local const static good_test x85;
const thread_local static good_test x86;
thread_local volatile static good_test x87;
volatile thread_local static good_test x88;
thread_local const static volatile good_test x89;
thread_local const volatile static good_test x90;
const thread_local static volatile good_test x91;
const thread_local volatile static good_test x92;
const volatile thread_local static good_test x93;
thread_local volatile static const good_test x94;
thread_local volatile const static good_test x95;
volatile thread_local static const good_test x96;
volatile thread_local const static good_test x97;
volatile const thread_local static good_test x98;

// check that things work properly in namespaces
namespace test {

class good_test {
	int x_factor
};

// ringing the changes on extern
extern class good_test x1;
extern const class good_test x2;
extern volatile class good_test x3;
extern const volatile class good_test x4;
extern volatile const class good_test x5;

// ringing the changes on static
static class good_test x6;
static const class good_test x7;
static volatile class good_test x8;
static const volatile class good_test x9;
static volatile const class good_test x10;

// extern/static not in first position is deprecated, but legal
const extern class good_test x11;
volatile extern class good_test x12;
const extern volatile class good_test x13;
const volatile extern class good_test x14;
volatile extern const class good_test x15;
volatile const extern class good_test x16;
const static class good_test x17;
volatile static class good_test x18;
const static volatile class good_test x19;
const volatile static class good_test x20;
volatile static const class good_test x21;
volatile const static class good_test x22;

// ringing the changes on thread_local extern
extern thread_local class good_test x23;
extern thread_local const class good_test x24;
extern thread_local volatile class good_test x25;
extern thread_local const volatile class good_test x26;
extern thread_local volatile const class good_test x27;
thread_local extern class good_test x28;
thread_local extern const class good_test x29;
thread_local extern volatile class good_test x30;
thread_local extern const volatile class good_test x31;
thread_local extern volatile const class good_test x32;

// ringing the changes on thread_local static
static thread_local class good_test x33;
static thread_local const class good_test x34;
static thread_local volatile class good_test x35;
static thread_local const volatile class good_test x36;
static thread_local volatile const class good_test x37;
thread_local static class good_test x38;
thread_local static const class good_test x39;
thread_local static volatile class good_test x40;
thread_local static const volatile class good_test x41;
thread_local static volatile const class good_test x42;

// thread_local extern not in first two positions is deprecated, but legal
extern const thread_local class good_test x43;
const extern thread_local class good_test x44;
extern volatile thread_local class good_test x45;
volatile extern thread_local class good_test x46;
extern const thread_local volatile class good_test x47;
extern const volatile thread_local class good_test x48;
const extern thread_local volatile class good_test x49;
const extern volatile thread_local class good_test x50;
const volatile extern thread_local class good_test x51;
extern volatile thread_local const class good_test x52;
extern volatile const thread_local class good_test x53;
volatile extern thread_local const class good_test x54;
volatile extern const thread_local class good_test x55;
volatile const extern thread_local class good_test x56;
thread_local const extern class good_test x57;
const thread_local extern class good_test x58;
thread_local volatile extern class good_test x59;
volatile thread_local extern class good_test x60;
thread_local const extern volatile class good_test x61;
thread_local const volatile extern class good_test x62;
const thread_local extern volatile class good_test x63;
const thread_local volatile extern class good_test x64;
const volatile thread_local extern class good_test x65;
thread_local volatile extern const class good_test x66;
thread_local volatile const extern class good_test x67;
volatile thread_local extern const class good_test x68;
volatile thread_local const extern class good_test x69;
volatile const thread_local extern class good_test x70;

// thread_local static not in first two positions is deprecated, but legal
static const thread_local class good_test x71;
const static thread_local class good_test x72;
static volatile thread_local class good_test x73;
volatile static thread_local class good_test x74;
static const thread_local volatile class good_test x75;
static const volatile thread_local class good_test x76;
const static thread_local volatile class good_test x77;
const static volatile thread_local class good_test x78;
const volatile static thread_local class good_test x79;
static volatile thread_local const class good_test x80;
static volatile const thread_local class good_test x81;
volatile static thread_local const class good_test x82;
volatile static const thread_local class good_test x83;
volatile const static thread_local class good_test x84;
thread_local const static class good_test x85;
const thread_local static class good_test x86;
thread_local volatile static class good_test x87;
volatile thread_local static class good_test x88;
thread_local const static volatile class good_test x89;
thread_local const volatile static class good_test x90;
const thread_local static volatile class good_test x91;
const thread_local volatile static class good_test x92;
const volatile thread_local static class good_test x93;
thread_local volatile static const class good_test x94;
thread_local volatile const static class good_test x95;
volatile thread_local static const class good_test x96;
volatile thread_local const static class good_test x97;
volatile const thread_local static class good_test x98;

// define-declares
// ringing the changes on extern
extern class good_test1 { int x_factor1; } x_1;
extern const class good_test2 { int x_factor2; } x_2;
extern volatile class good_test3 { int x_factor3; } x_3;
extern const volatile class good_test4 { int x_factor4; } x_4;
extern volatile const class good_test5 { int x_factor5; } x_5;

// ringing the changes on static
static class good_test6 { int x_factor6; } x_6;
static const class good_test7 { int x_factor7; } x_7;
static volatile class good_test8 { int x_factor8; } x_8;
static const volatile class good_test9 { int x_factor9; } x_9;
static volatile const class good_test10 { int x_factor10; } x_10;

// extern/static not in first position is deprecated, but legal
const extern class good_test11 { int x_factor11; } x_11;
volatile extern class good_test12 { int x_factor12; } x_12;
const extern volatile class good_test13 { int x_factor13; } x_13;
const volatile extern class good_test14 { int x_factor14; } x_14;
volatile extern const class good_test15 { int x_factor15; } x_15;
volatile const extern class good_test16 { int x_factor16; } x_16;
const static class good_test17 { int x_factor17; } x_17;
volatile static class good_test18 { int x_factor18; } x_18;
const static volatile class good_test19 { int x_factor19; } x_19;
const volatile static class good_test20 { int x_factor20; } x_20;
volatile static const class good_test21 { int x_factor21; } x_21;
volatile const static class good_test22 { int x_factor22; } x_22;

// ringing the changes on thread_local extern
extern thread_local class good_test23 { int x_factor23; } x_23;
extern thread_local const class good_test24 { int x_factor24; } x_24;
extern thread_local volatile class good_test25 { int x_factor25; } x_25;
extern thread_local const volatile class good_test26 { int x_factor26; } x_26;
extern thread_local volatile const class good_test27 { int x_factor27; } x_27;
thread_local extern class good_test28 { int x_factor28; } x_28;
thread_local extern const class good_test29 { int x_factor29; } x_29;
thread_local extern volatile class good_test30 { int x_factor30; } x_30;
thread_local extern const volatile class good_test31 { int x_factor31; } x_31;
thread_local extern volatile const class good_test32 { int x_factor32; } x_32;

// ringing the changes on thread_local static
static thread_local class good_test33 { int x_factor33; } x_33;
static thread_local const class good_test34 { int x_factor34; } x_34;
static thread_local volatile class good_test35 { int x_factor35; } x_35;
static thread_local const volatile class good_test36 { int x_factor36; } x_36;
static thread_local volatile const class good_test37 { int x_factor37; } x_37;
thread_local static class good_test38 { int x_factor38; } x_38;
thread_local static const class good_test39 { int x_factor39; } x_39;
thread_local static volatile class good_test40 { int x_factor40; } x_40;
thread_local static const volatile class good_test41 { int x_factor41; } x_41;
thread_local static volatile const class good_test42 { int x_factor42; } x_42;

// thread_local extern not in first two positions is deprecated, but legal
extern const thread_local class good_test43 { int x_factor43; } x_43;
const extern thread_local class good_test44 { int x_factor44; } x_44;
extern volatile thread_local class good_test45 { int x_factor45; } x_45;
volatile extern thread_local class good_test46 { int x_factor46; } x_46;
extern const thread_local volatile class good_test47 { int x_factor47; } x_47;
extern const volatile thread_local class good_test48 { int x_factor48; } x_48;
const extern thread_local volatile class good_test49 { int x_factor49; } x_49;
const extern volatile thread_local class good_test50 { int x_factor50; } x_50;
const volatile extern thread_local class good_test51 { int x_factor51; } x_51;
extern volatile thread_local const class good_test52 { int x_factor52; } x_52;
extern volatile const thread_local class good_test53 { int x_factor53; } x_53;
volatile extern thread_local const class good_test54 { int x_factor54; } x_54;
volatile extern const thread_local class good_test55 { int x_factor55; } x_55;
volatile const extern thread_local class good_test56 { int x_factor56; } x_56;
thread_local const extern class good_test57 { int x_factor57; } x_57;
const thread_local extern class good_test58 { int x_factor58; } x_58;
thread_local volatile extern class good_test59 { int x_factor59; } x_59;
volatile thread_local extern class good_test60 { int x_factor60; } x_60;
thread_local const extern volatile class good_test61 { int x_factor61; } x_61;
thread_local const volatile extern class good_test62 { int x_factor62; } x_62;
const thread_local extern volatile class good_test63 { int x_factor63; } x_63;
const thread_local volatile extern class good_test64 { int x_factor64; } x_64;
const volatile thread_local extern class good_test65 { int x_factor65; } x_65;
thread_local volatile extern const class good_test66 { int x_factor66; } x_66;
thread_local volatile const extern class good_test67 { int x_factor67; } x_67;
volatile thread_local extern const class good_test68 { int x_factor68; } x_68;
volatile thread_local const extern class good_test69 { int x_factor69; } x_69;
volatile const thread_local extern class good_test70 { int x_factor70; } x_70;

// thread_local static not in first two positions is deprecated, but legal
static const thread_local class good_test71 { int x_factor71; } x_71;
const static thread_local class good_test72 { int x_factor72; } x_72;
static volatile thread_local class good_test73 { int x_factor73; } x_73;
volatile static thread_local class good_test74 { int x_factor74; } x_74;
static const thread_local volatile class good_test75 { int x_factor75; } x_75;
static const volatile thread_local class good_test76 { int x_factor76; } x_76;
const static thread_local volatile class good_test77 { int x_factor77; } x_77;
const static volatile thread_local class good_test78 { int x_factor78; } x_78;
const volatile static thread_local class good_test79 { int x_factor79; } x_79;
static volatile thread_local const class good_test80 { int x_factor80; } x_80;
static volatile const thread_local class good_test81 { int x_factor81; } x_81;
volatile static thread_local const class good_test82 { int x_factor82; } x_82;
volatile static const thread_local class good_test83 { int x_factor83; } x_83;
volatile const static thread_local class good_test84 { int x_factor84; } x_84;
thread_local const static class good_test85 { int x_factor85; } x_85;
const thread_local static class good_test86 { int x_factor86; } x_86;
thread_local volatile static class good_test87 { int x_factor87; } x_87;
volatile thread_local static class good_test88 { int x_factor88; } x_88;
thread_local const static volatile class good_test89 { int x_factor89; } x_89;
thread_local const volatile static class good_test90 { int x_factor90; } x_90;
const thread_local static volatile class good_test91 { int x_factor91; } x_91;
const thread_local volatile static class good_test92 { int x_factor92; } x_92;
const volatile thread_local static class good_test93 { int x_factor93; } x_93;
thread_local volatile static const class good_test94 { int x_factor94; } x_94;
thread_local volatile const static class good_test95 { int x_factor95; } x_95;
volatile thread_local static const class good_test96 { int x_factor96; } x_96;
volatile thread_local const static class good_test97 { int x_factor97; } x_97;
volatile const thread_local static class good_test98 { int x_factor98; } x_98;

}	// end namespace test

// check that keyword suppression works in namespaces
namespace test2 {

class good_test {
	int x_factor
};

// ringing the changes on extern
extern good_test x1;
extern const good_test x2;
extern volatile good_test x3;
extern const volatile good_test x4;
extern volatile const good_test x5;

// ringing the changes on static
static good_test x6;
static const good_test x7;
static volatile good_test x8;
static const volatile good_test x9;
static volatile const good_test x10;

// extern/static not in first position is deprecated, but legal
const extern good_test x11;
volatile extern good_test x12;
const extern volatile good_test x13;
const volatile extern good_test x14;
volatile extern const good_test x15;
volatile const extern good_test x16;
const static good_test x17;
volatile static good_test x18;
const static volatile good_test x19;
const volatile static good_test x20;
volatile static const good_test x21;
volatile const static good_test x22;

// ringing the changes on thread_local extern
extern thread_local good_test x23;
extern thread_local const good_test x24;
extern thread_local volatile good_test x25;
extern thread_local const volatile good_test x26;
extern thread_local volatile const good_test x27;
thread_local extern good_test x28;
thread_local extern const good_test x29;
thread_local extern volatile good_test x30;
thread_local extern const volatile good_test x31;
thread_local extern volatile const good_test x32;

// ringing the changes on thread_local static
static thread_local good_test x33;
static thread_local const good_test x34;
static thread_local volatile good_test x35;
static thread_local const volatile good_test x36;
static thread_local volatile const good_test x37;
thread_local static good_test x38;
thread_local static const good_test x39;
thread_local static volatile good_test x40;
thread_local static const volatile good_test x41;
thread_local static volatile const good_test x42;

// thread_local extern not in first two positions is deprecated, but legal
extern const thread_local good_test x43;
const extern thread_local good_test x44;
extern volatile thread_local good_test x45;
volatile extern thread_local good_test x46;
extern const thread_local volatile good_test x47;
extern const volatile thread_local good_test x48;
const extern thread_local volatile good_test x49;
const extern volatile thread_local good_test x50;
const volatile extern thread_local good_test x51;
extern volatile thread_local const good_test x52;
extern volatile const thread_local good_test x53;
volatile extern thread_local const good_test x54;
volatile extern const thread_local good_test x55;
volatile const extern thread_local good_test x56;
thread_local const extern good_test x57;
const thread_local extern good_test x58;
thread_local volatile extern good_test x59;
volatile thread_local extern good_test x60;
thread_local const extern volatile good_test x61;
thread_local const volatile extern good_test x62;
const thread_local extern volatile good_test x63;
const thread_local volatile extern good_test x64;
const volatile thread_local extern good_test x65;
thread_local volatile extern const good_test x66;
thread_local volatile const extern good_test x67;
volatile thread_local extern const good_test x68;
volatile thread_local const extern good_test x69;
volatile const thread_local extern good_test x70;

// thread_local static not in first two positions is deprecated, but legal
static const thread_local good_test x71;
const static thread_local good_test x72;
static volatile thread_local good_test x73;
volatile static thread_local good_test x74;
static const thread_local volatile good_test x75;
static const volatile thread_local good_test x76;
const static thread_local volatile good_test x77;
const static volatile thread_local good_test x78;
const volatile static thread_local good_test x79;
static volatile thread_local const good_test x80;
static volatile const thread_local good_test x81;
volatile static thread_local const good_test x82;
volatile static const thread_local good_test x83;
volatile const static thread_local good_test x84;
thread_local const static good_test x85;
const thread_local static good_test x86;
thread_local volatile static good_test x87;
volatile thread_local static good_test x88;
thread_local const static volatile good_test x89;
thread_local const volatile static good_test x90;
const thread_local static volatile good_test x91;
const thread_local volatile static good_test x92;
const volatile thread_local static good_test x93;
thread_local volatile static const good_test x94;
thread_local volatile const static good_test x95;
volatile thread_local static const good_test x96;
volatile thread_local const static good_test x97;
volatile const thread_local static good_test x98;

}	// end namespace test2
