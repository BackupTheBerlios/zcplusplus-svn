#!/usr/bin/python
# an auxilliary script for generating the test files
# designed for Python 2.6, may work with other versions
# (C)2009 Kenneth Boyd, license: MIT.txt

from sys import argv,exit;

test_lines = [ '// check that #define of reserved keyword errors', 
	'// (C)2009 Kenneth Boyd, license: MIT.txt\n']

def ExpandKeywordTestCase(keyword,file_prefix,file_suffix):
	TargetFile = open(file_prefix+'_'+keyword+'.'+file_suffix,'w')	# *.sh
	TargetFile.write('// '+file_prefix.replace('/','\\')+'_'+keyword+'.'+file_suffix+'\n')
	TargetFile.write('// check that #define of '+keyword+' errors')
	TargetFile.write('// (C)2009 Kenneth Boyd, license: MIT.txt\n')
	TargetFile.write('#define '+keyword+' 1\n')
	if 'h'==file_suffix:
		TargetFile.write('#include <stdbool.h>\n')
	TargetFile.close()


if __name__ == '__main__':
	# online help
	if 2>=len(argv):
		print "Usage: target_path suffix"
		print "	suffix h generates C99 keyword rejection test cases"
		print " suffix hpp generates C++98 keyword rejection test cases"
		exit(0)

	C_CPP_keyword_list = ["__asm", "restrict", "_Bool",  "_Complex",
		"_Imaginary", "auto", "break", "case", "char", "const",
		"continue", "default", "do", "double", "else", "enum",
		"extern", "float", "for", "goto", "if", "inline", "int",
		"long", "register", "return", "short", "signed", "sizeof",
		"static", "struct", "switch", "typedef", "union", "unsigned",
		"void", "volatile", "while", "asm", "bool", "catch", "class",
		"const_cast", "delete", "dynamic_cast", "explicit", "false",
		"friend", "mutable", "namespace", "new", "operator", "private",
		"protected", "public", "reinterpret_cast", "static_cast",
		"template", "this", "throw", "true", "try", "typeid", 
		"typename", "using", "virtual", "wchar_t", "and", "and_eq",
		"bitand", "bitor", "compl", "not", "not_eq", "or", "or_eq",
		"xor", "xor_eq"]

	if 'h'==argv[2]:
		for keyword in C_CPP_keyword_list[:38]:
			ExpandKeywordTestCase(keyword,argv[1],argv[2])
	if 'hpp'==argv[2]:
		for keyword in C_CPP_keyword_list[5:]:
			ExpandKeywordTestCase(keyword,argv[1],argv[2])

