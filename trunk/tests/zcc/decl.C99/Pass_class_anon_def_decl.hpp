// decl.C99\Pass_class_anon_def_decl.hpp
// using singly defined struct
// (C)2010 Kenneth Boyd, license: MIT.txt

// exercise some declarations
class {int x_factor;} x1;
const class {int x_factor;} c1;
class {int x_factor;} const c2;
volatile class {int x_factor;} v1;
class {int x_factor;} volatile v2;
const volatile class {int x_factor;} cv1;
volatile const class {int x_factor;} cv2;
const class {int x_factor;} volatile cv3;
volatile class {int x_factor;} const cv4;
class {int x_factor;} const volatile cv5;
class {int x_factor;} volatile const cv6;
