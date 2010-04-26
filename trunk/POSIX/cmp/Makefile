# POSIX makefile

.SUFFIXES:
.SUFFIXES:  .c .o .exe

include ../../POSIX.inc
# override link flags
LINK_FLAGS = -L../../lib/host.zcc

# GNU targets we know about
all: cmp.exe

clean:
	rm -f *.o *.exe

# dependencies
include POSIX.dep

cmp.exe : $(OBJECTS_CMP_LINK_PRIORITY)
	$(CC) $(LINK_FLAGS) -ocmp.exe $(OBJECTS_CMP)  -lz_stdio_log
	strip --preserve-dates --strip-unneeded cmp.exe

# inference rules
# global project search paths for headers
# processing details

.c.o:
	$(CC) $(CFLAGS) $(ARCH_FLAGS) $(OTHER_INCLUDEDIR) $(C_MACROS) \
	 -o $*.o -c -xc -pipe $<
