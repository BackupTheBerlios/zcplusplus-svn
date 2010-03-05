#!/usr/bin/python
# designed for Python 2.6, may work with other versions
# (C)2010 Kenneth Boyd, license: MIT.txt

from sys import argv,exit;

# this suppresses the comments used by selective_cut.py

#   This is inspired by Perl's Plain Old Documentation format, but 
# works generically with languages that use shell one-line comments
# (e.g.: Python, Perl, PHP, Ruby, C/C++
#   Idea is that lines #/* cut-cut_id */ act like =cut in Perl.
#   C,C++ are fine because the C-comment becomes whitespace, leaving behind 
# a null preprocessing directive which is removed by the preprocessor.
#   Languages with native shell comments just treat these lines as comments

def SuppressCut(cut_id,srcfile,destfile):
	test_lines = []
	TargetFile = open(destfile,'w')	# *.sh
	with open(srcfile,'r') as f:
		for line in f:
			line_proxy = line.strip()
			if line_proxy!='#/*cut-'+cut_id+'*/':
				TargetFile.write(line)
	TargetFile.close()


if __name__ == '__main__':
	# online help
	if 4!=len(argv):
		print "Usage: cut_id srcfile destfile"
		print "\texcises #/*cut-cut_id*/ comments"
		exit(0)

	SuppressCut(argv[1],argv[2],argv[3])

