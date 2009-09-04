# GNU compatibility core
srcdir = \CPP_App\Z.C++
prefix = \CPP_App\Z.C++
exec_prefix = $(prefix)
bindir = $(exec_prefix)\bin
includedir = $(exec_prefix)\include
libdir = $(exec_prefix)\lib

AR = lib

EXEEXT = exe
OBJEXT = obj

# /Wall warns on system headers in VC 2008, not good
CFLAGS = /O1 /EHsc /MD /W3
CXXFLAGS =

OTHER_INCLUDEDIR = -I \MingWin.aux\Headers.lib
C_MACROS = 
CXX_MACROS = 
