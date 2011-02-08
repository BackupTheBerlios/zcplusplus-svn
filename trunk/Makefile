# POSIX makefile

.SUFFIXES:
.SUFFIXES: .cpp .o .exe

include POSIX.inc

# GNU targets we know about
all: bin/z_cpp.exe bin/zcc.exe

clean:
	rm -f *.o *.exe lib/host.zcc/*.a
	cd Zaimoni.STL; make clean

# dependencies
include POSIX.dep

make_Zaimoni_STL:
	cd Zaimoni.STL; make host_install

bin/zcc.exe : make_Zaimoni_STL $(OBJECTS_BIN/ZCC_LINK_PRIORITY)
	g++ $(LINK_FLAGS) -obin/zcc.exe $(OBJECTS_BIN/ZCC) -lz_langconf  -lz_langconf -lz_log_adapter -lz_stdio_log -lz_mutex -lz_format_util -lz_stdio_c -lz_memory
	strip --preserve-dates --strip-unneeded bin/zcc.exe

bin/z_cpp.exe : make_Zaimoni_STL $(OBJECTS_BIN/Z_CPP_LINK_PRIORITY)
	g++ $(LINK_FLAGS) -obin/z_cpp.exe $(OBJECTS_BIN/Z_CPP) -lz_langconf  -lz_langconf -lz_log_adapter -lz_stdio_log -lz_mutex -lz_format_util -lz_stdio_c -lz_memory
	strip --preserve-dates --strip-unneeded bin/z_cpp.exe

# inference rules
# processing details
.cpp.o:
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(ARCH_FLAGS) $(OTHER_INCLUDEDIR) $(C_MACROS) $(CXX_MACROS) \
	 -o $*.o -c -xc++ -pipe $<
