# POSIX makefile

.SUFFIXES:
.SUFFIXES: .cpp .o .exe

include POSIX.inc

# GNU targets we know about
all: z_cpp.exe

clean:
	rm -f *.o *.exe

z_cpp.exe : CPreproc.o CPreproc_autogen.o unsigned_aux.o CSupport.o z_cpp.o load_src.o CPUInfo.o ParseTree.o type_system.o filesystem.o C_PPOctalInteger.o C_PPDecimalFloat.o AtomicString.o C_PPNumCore.o C_PPDecimalInteger.o C_PPHexInteger.o C_PPHexFloat.o Trigraph.o Flat_UNI.o
	g++ -oz_cpp.exe z_cpp.o AtomicString.o filesystem.o CPUInfo.o type_system.o ParseTree.o CSupport.o C_PPNumCore.o C_PPDecimalInteger.o C_PPOctalInteger.o C_PPHexInteger.o C_PPDecimalFloat.o C_PPHexFloat.o unsigned_aux.o Trigraph.o Flat_UNI.o load_src.o CPreproc.o CPreproc_autogen.o -lz_memory  -lz_langconf -lz_log_adapter -lz_stdio_log -lz_mutex -lz_format_util
	strip --preserve-dates --strip-unneeded z_cpp.exe

# dependencies
include POSIX.dep

# inference rules
# processing details
.cpp.o:
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(ARCH_FLAGS) $(OTHER_INCLUDEDIR) $(C_MACROS) $(CXX_MACROS) \
	 -o $*.o -c -xc++ -pipe $<
