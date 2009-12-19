# GNU compatibility core
srcdir = \CPP_App\Z.C++
prefix = \CPP_App\Z.C++
exec_prefix = $(prefix)
bindir = $(exec_prefix)\bin
includedir = $(exec_prefix)\include
libdir = $(exec_prefix)\lib

host_prefix = c:\MSVC9.0\VC
host_exec_prefix = $(host_prefix)
host_bindir = $(host_exec_prefix)\bin
host_includedir = $(host_exec_prefix)\include
host_libdir = $(host_exec_prefix)\lib

AR = lib

EXEEXT = exe
OBJEXT = obj

# /Wall warns on system headers in VC 2008, not good
CFLAGS = /O1 /EHsc /MD /W3
CXXFLAGS =

OTHER_INCLUDEDIR = -I \MingWin.aux\Headers.lib
C_MACROS = 
CXX_MACROS = 
