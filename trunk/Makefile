# POSIX makefile

.SUFFIXES:
.SUFFIXES: .cpp .o .exe

include POSIX.inc

# GNU targets we know about
all: z_cpp.exe zcc.exe

clean:
	rm -f *.o *.exe

# dependencies
include POSIX.dep

z_cpp.exe : $(OBJECTS_Z_CPP_LINK_PRIORITY)
	g++ $(LINK_FLAGS) -oz_cpp.exe $(OBJECTS_Z_CPP) -lz_langconf  -lz_langconf -lz_log_adapter -lz_stdio_log -lz_mutex -lz_format_util -lz_memory
	strip --preserve-dates --strip-unneeded z_cpp.exe

zcc.exe : $(OBJECTS_ZCC_LINK_PRIORITY)
	g++ $(LINK_FLAGS) -ozcc.exe $(OBJECTS_ZCC) -lz_langconf  -lz_langconf -lz_log_adapter -lz_stdio_log -lz_mutex -lz_format_util -lz_memory
	strip --preserve-dates --strip-unneeded zcc.exe

# inference rules
# processing details
.cpp.o:
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(ARCH_FLAGS) $(OTHER_INCLUDEDIR) $(C_MACROS) $(CXX_MACROS) \
	 -o $*.o -c -xc++ -pipe $<
