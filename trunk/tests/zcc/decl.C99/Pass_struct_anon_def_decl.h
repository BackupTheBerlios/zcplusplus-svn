// decl.C99\Pass_struct_anon_def_decl.h
// using singly defined struct
// (C)2010 Kenneth Boyd, license: MIT.txt

// exercise some declarations
struct {int x_factor;} x1;
const struct {int x_factor;} c1;
struct {int x_factor;} const c2;
volatile struct {int x_factor;} v1;
struct {int x_factor;} volatile v2;
const volatile struct {int x_factor;} cv1;
volatile const struct {int x_factor;} cv2;
const struct {int x_factor;} volatile cv3;
volatile struct {int x_factor;} const cv4;
struct {int x_factor;} const volatile cv5;
struct {int x_factor;} volatile const cv6;
