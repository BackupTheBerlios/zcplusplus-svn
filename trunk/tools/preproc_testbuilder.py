#!/usr/bin/python
# an auxilliary script for generating the test files
# designed for Python 2.6, may work with other versions
# (C)2009 Kenneth Boyd, license: MIT.txt

from sys import argv,exit;

def ExpandMacroDefinedFalse(test_lines,ifdef_modes,target):
	if   1==ifdef_modes:
		test_lines.append('#if defined('+target+')\n')
		test_lines.append('\t#if '+target+'\n')
		test_lines.append('\t#error '+target+' is true\n')
		test_lines.append('\t#endif\n')
		test_lines.append('#else\n')
		test_lines.append('#error '+target+' is undefined\n')
		test_lines.append('#endif\n')
	elif 2==ifdef_modes:
		test_lines.append('#ifndef '+target+'\n')
		test_lines.append('#error '+target+' is undefined\n')
		test_lines.append('#else\n')
		test_lines.append('\t#if '+target+'\n')
		test_lines.append('\t#error '+target+' is true\n')
		test_lines.append('\t#endif\n')
		test_lines.append('#endif\n')
	elif 3==ifdef_modes:
		test_lines.append('#if !defined('+target+')\n')
		test_lines.append('#error '+target+' is undefined\n')
		test_lines.append('#else\n')
		test_lines.append('\t#if '+target+'\n')
		test_lines.append('\t#error '+target+' is true\n')
		test_lines.append('\t#endif\n')
		test_lines.append('#endif\n')
	else:
		test_lines.append('#ifdef '+target+'\n')
		test_lines.append('\t#if '+target+'\n')
		test_lines.append('\t#error '+target+' is true\n')
		test_lines.append('\t#endif\n')
		test_lines.append('#else\n')
		test_lines.append('#error '+target+' is undefined\n')
		test_lines.append('#endif\n')
	ifdef_modes = (ifdef_modes+1)%4

def SpawnTestCases(filename):
	root_name = filename[:filename.rfind('.')]
	core_root_name = root_name[:root_name.rfind('_')+1]
	index_ifdef_modes = 0
	index_ifndef_modes = 0
	test_lines = []
	suffixes = []
	include_files = []
	object_macros = {}
	with open(filename,'r') as f:
		for line in f:
			if line.startswith('SUFFIXES '):
				suffixes = line[9:].strip().split()
			elif line.startswith('INCLUDE_FILES '):
				include_files = line[14:].strip().split()
				test_lines.append(line)
			elif line.startswith('MACRO_DEFINED_FALSE '):
				target = line[20:].strip()
				ExpandMacroDefinedFalse(test_lines,index_ifdef_modes,target)
			elif line.startswith('VALID_EXPRESSION '):
				target = line[17:].strip()
				test_lines.append('#if '+target+'\n')
				test_lines.append('#endif\n')
			elif line.startswith('TRUE_EXPRESSION '):
				target = line[16:].strip()
				test_lines.append('#if '+target+'\n')
				test_lines.append('#else\n')
				test_lines.append('#error '+target+' is false\n')
				test_lines.append('#endif\n')
			elif line.startswith('FALSE_EXPRESSION '):
				target = line[17:].strip()
				test_lines.append('#if '+target+'\n')
				test_lines.append('#error '+target+' is true\n')
				test_lines.append('#endif\n')
			elif line.startswith('OBJECTLIKE_MACRO '):
				target = line[17:].strip().split()
				object_macros[target[0]] = target[1:]
			else:
				test_lines.append(line);

	Idx = 0
	for suffix in suffixes:
		if include_files:
			root_name = core_root_name+include_files[Idx].replace('.','_')
		TargetFile = open(root_name+'.'+suffix,'w')	# *.sh
		TargetFile.write('// '+root_name.replace('/','\\')+'.'+suffix+'\n')
		for line in test_lines:
			if line.startswith('INCLUDE_FILES '):
				TargetFile.write('#include <'+include_files[Idx]+'>\n')
			elif 0<len(object_macros):
				for x in object_macros.keys():
					if -1<line.find(x):
						line = object_macros[x][Idx].join(line.split(x))
				TargetFile.write(line)
			else:
				TargetFile.write(line)
		TargetFile.close()
		Idx = Idx+1

if __name__ == '__main__':
	# online help
	if 1>=len(argv):
		print "Usage: filename [filename ...]"
		print "\tSyntax for contents of filename"
		print "\tSUFFIXES suffix [suffix]"
		print "\t\tsets file suffixes for tests.  Use once."
		print "\tINCLUDE_FILES include_file [include]"
		print "\t\tsets include files for tests.  Use once."
		exit(0)

	for filename in argv[1:]:
		SpawnTestCases(filename)

