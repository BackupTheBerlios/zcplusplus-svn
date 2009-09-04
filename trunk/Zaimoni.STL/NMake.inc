# GNU compatibility core
prefix = \MSVC9.0\VC
exec_prefix = $(prefix)
bindir = $(exec_prefix)\bin
includedir = $(exec_prefix)\include
libdir = $(exec_prefix)\lib

AR = lib
# CC, CXX handled by NMake defaults

EXEEXT = exe
OBJEXT = obj

# other control points
# FORCE_ISO: ZAIMONI_FORCE_ISO disables the custom memory manager
FORCE_ISO =
# FORCE_ISO = -DZAIMONI_FORCE_ISO

