// decl.C99\Error_enum_brace.h
// ZCC-specific check that non-atomic internal representation fails
// (C)2009 Kenneth Boyd, license: MIT.txt

enum bad_test {
	{} = 1
};

