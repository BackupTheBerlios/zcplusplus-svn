// decl.C99\Error_struct_runon_def.hpp
// using singly defined struct
// (C)2011 Kenneth Boyd, license: MIT.txt

struct bad_test {
	int x_factor;
};

// ringing the changes on extern
extern struct bad_test x1
extern const struct bad_test x2;
