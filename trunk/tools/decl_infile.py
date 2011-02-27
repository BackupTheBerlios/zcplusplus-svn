#!/usr/bin/python
# an auxilliary script for generating the test files
# verified for Python 2.5, 2.6, 2.7
# fails: Python 3.0, 3.1, 3.2 (xrange not supported)
# (C)2011 Kenneth Boyd, license: MIT.txt

# target files
target_files = ['Pass_enum_def.in']

invariant_header_lines = [
'SUFFIXES h hpp\n'
'OBJECTLIKE_MACRO THREAD_LOCAL _Thread_Local thread_local\n'
'// (C)2009,2011 Kenneth Boyd, license: MIT.txt\n'
]

context = {'Pass_enum_def.in':'// using singly defined enum\n'}

global_define = {'Pass_enum_def.in':'\nenum good_test {\n\tx_factor = 1\n};\n\n'}

section_comments = ['// ringing the changes on extern\n',
"// ringing the changes on static\n// (don't test static const -- no chance to initialize before use)\n",
'// extern/static not in first postion is deprecated, but legal\n',
'// ringing the changes on THREAD_LOCAL extern\n',
'// ringing the changes on THREAD_LOCAL static\n',
'// THREAD_LOCAL extern not in first two postions is deprecated, but legal\n',
'// THREAD_LOCAL static not in first two postions is deprecated, but legal\n']

def enum_decl(i):
	return "enum good_test x"+i
	
var_decl = {'Pass_enum_def.in':enum_decl}

def enum_def(i):
	return 'enum good_test'+i+' { x_factor'+i+' = 1 } x_'+i
	
var_def = {'Pass_enum_def.in':enum_def}

test_qualifiers = [
'extern',
'extern const',
'extern volatile',
'extern const volatile',
'extern volatile const',

'static',
'static volatile',
'static const volatile',
'static volatile const',

'const extern',
'volatile extern',
'const extern volatile',
'const volatile extern',
'volatile extern const',
'volatile const extern',

'volatile static',
'const static volatile',
'const volatile static',
'volatile static const',
'volatile const static',

'extern THREAD_LOCAL',
'extern THREAD_LOCAL const',
'extern THREAD_LOCAL volatile',
'extern THREAD_LOCAL const volatile',
'extern THREAD_LOCAL volatile const',
'THREAD_LOCAL extern',
'THREAD_LOCAL extern const',
'THREAD_LOCAL extern volatile',
'THREAD_LOCAL extern const volatile',
'THREAD_LOCAL extern volatile const',

'static THREAD_LOCAL',
'static THREAD_LOCAL const',	# next
'static THREAD_LOCAL volatile',
'static THREAD_LOCAL const volatile',
'static THREAD_LOCAL volatile const',
'THREAD_LOCAL static',
'THREAD_LOCAL static const',
'THREAD_LOCAL static volatile',
'THREAD_LOCAL static const volatile',
'THREAD_LOCAL static volatile const',

'extern const THREAD_LOCAL',
'const extern THREAD_LOCAL',
'extern volatile THREAD_LOCAL',
'volatile extern THREAD_LOCAL',
'extern const THREAD_LOCAL volatile',
'extern const volatile THREAD_LOCAL',
'const extern THREAD_LOCAL volatile',
'const extern volatile THREAD_LOCAL',
'const volatile extern THREAD_LOCAL',
'extern volatile THREAD_LOCAL const',
'extern volatile const THREAD_LOCAL',
'volatile extern THREAD_LOCAL const',
'volatile extern const THREAD_LOCAL',
'volatile const extern THREAD_LOCAL',
'THREAD_LOCAL const extern',
'const THREAD_LOCAL extern',
'THREAD_LOCAL volatile extern',
'volatile THREAD_LOCAL extern',
'THREAD_LOCAL const extern volatile',
'THREAD_LOCAL const volatile extern',
'const THREAD_LOCAL extern volatile',
'const THREAD_LOCAL volatile extern',
'const volatile THREAD_LOCAL extern',
'THREAD_LOCAL volatile extern const',
'THREAD_LOCAL volatile const extern',
'volatile THREAD_LOCAL extern const',
'volatile THREAD_LOCAL const extern',
'volatile const THREAD_LOCAL extern',

'static const THREAD_LOCAL',
'const static THREAD_LOCAL',
'static volatile THREAD_LOCAL',
'volatile static THREAD_LOCAL',
'static const THREAD_LOCAL volatile',
'static const volatile THREAD_LOCAL',
'const static THREAD_LOCAL volatile',
'const static volatile THREAD_LOCAL',
'const volatile static THREAD_LOCAL',
'static volatile THREAD_LOCAL const',
'static volatile const THREAD_LOCAL',
'volatile static THREAD_LOCAL const',
'volatile static const THREAD_LOCAL',
'volatile const static THREAD_LOCAL',
'THREAD_LOCAL const static',
'const THREAD_LOCAL static',
'THREAD_LOCAL volatile static',
'volatile THREAD_LOCAL static',
'THREAD_LOCAL const static volatile',
'THREAD_LOCAL const volatile static',
'const THREAD_LOCAL static volatile',
'const THREAD_LOCAL volatile static',
'const volatile THREAD_LOCAL static',
'THREAD_LOCAL volatile static const',
'THREAD_LOCAL volatile const static',
'volatile THREAD_LOCAL static const',
'volatile THREAD_LOCAL const static',
'volatile const THREAD_LOCAL static'
]

def SpawnTestCase(dest_file):
	TargetFile = open(dest_file,'w')
	for line in invariant_header_lines:
		TargetFile.write(line)
	TargetFile.write(context[dest_file])
	TargetFile.write(global_define[dest_file])

	TargetFile.write(section_comments[0])
	for i in xrange(5):
		TargetFile.write(test_qualifiers[i]+' '+var_decl[dest_file](str(i+1))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[1])
	for i in xrange(4):
		TargetFile.write(test_qualifiers[i+5]+' '+var_decl[dest_file](str(i+6))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[2])
	for i in xrange(11):
		TargetFile.write(test_qualifiers[i+9]+' '+var_decl[dest_file](str(i+10))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[3])
	for i in xrange(10):
		TargetFile.write(test_qualifiers[i+20]+' '+var_decl[dest_file](str(i+21))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[4])
	for i in xrange(10):
		TargetFile.write(test_qualifiers[i+30]+' '+var_decl[dest_file](str(i+31))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[5])
	for i in xrange(28):
		TargetFile.write(test_qualifiers[i+40]+' '+var_decl[dest_file](str(i+41))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[6])
	for i in xrange(28):
		TargetFile.write(test_qualifiers[i+68]+' '+var_decl[dest_file](str(i+69))+';\n')
	TargetFile.write('\n')

	TargetFile.write('// define-declares\n')
	TargetFile.write(section_comments[0])
	for i in xrange(5):
		TargetFile.write(test_qualifiers[i]+' '+var_def[dest_file](str(i+1))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[1])
	for i in xrange(4):
		TargetFile.write(test_qualifiers[i+5]+' '+var_def[dest_file](str(i+6))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[2])
	for i in xrange(11):
		TargetFile.write(test_qualifiers[i+9]+' '+var_def[dest_file](str(i+10))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[3])
	for i in xrange(10):
		TargetFile.write(test_qualifiers[i+20]+' '+var_def[dest_file](str(i+21))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[4])
	for i in xrange(10):
		TargetFile.write(test_qualifiers[i+30]+' '+var_def[dest_file](str(i+31))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[5])
	for i in xrange(28):
		TargetFile.write(test_qualifiers[i+40]+' '+var_def[dest_file](str(i+41))+';\n')
	TargetFile.write('\n')

	TargetFile.write(section_comments[6])
	for i in xrange(28):
		TargetFile.write(test_qualifiers[i+68]+' '+var_def[dest_file](str(i+69))+';\n')
	TargetFile.write('\n')

	TargetFile.close()


if __name__ == '__main__':
	for filename in target_files:
		SpawnTestCase(filename)

