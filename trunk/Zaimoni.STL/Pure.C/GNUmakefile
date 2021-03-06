# Makefile for pure.C directory
# assumes GNU make

.SUFFIXES:
.SUFFIXES: .c .a .o

include ../POSIX.inc

CFLAGS := -Os -Wall -Wextra -finline -Winline -Wdisabled-optimization --param max-inline-insns-single=500 -pipe -DNDEBUG

LIBS := libz_format_util.a libz_stdio_c.a

# GNU targets we know about
all: auto_int.h comptest.h $(LIBS)

clean:
	rm -f *.a *.o auto_int.h comptest.h

install: $(addprefix $(libdir)/, $(LIBS))
host_install: auto_int.h comptest.h $(addprefix ../../$(host_libdir)/, $(LIBS))

uninstall:
	rm -f $(libdir)/libz_format_util.a
	rm -f $(libdir)/libz_stdio_c.a

# rules
format_util.o: format_util.c format_util.h

auto_int.h : int_probe.c
	$(CC) -oint_probe.exe $(CFLAGS) -mconsole $(ARCH_FLAGS) int_probe.c
	./int_probe.exe > auto_int.h
	rm int_probe.exe

comptest.h : comptest.in compile.test/exists_MS_io_h.c
	cp comptest.in comptest.h
	-$(CC) -oexists_MS_io_h.exe $(CFLAGS) -mconsole $(ARCH_FLAGS) compile.test/exists_MS_io_h.c
	-./exists_MS_io_h.exe >> comptest.h
	-rm exists_MS_io_h.exe
	-$(CC) -orealloc_0.exe $(CFLAGS) -mconsole $(ARCH_FLAGS) compile.test/realloc_0.c
	-./realloc_0.exe >> comptest.h
	-rm realloc_0.exe

.c.o:
	$(CC) -c $< $(CFLAGS) $(ARCH_FLAGS) -o $*.o

libz_format_util.a: format_util.o
	$(AR) -ru libz_format_util.a format_util.o

libz_stdio_c.a: stdio_c.o
	$(AR) -ru libz_stdio_c.a stdio_c.o

$(libdir)/libz_format_util.a: libz_format_util.a
	cp libz_format_util.a $(libdir)/libz_format_util.a
	
$(libdir)/libz_stdio_c.a: libz_stdio_c.a
	cp libz_stdio_c.a $(libdir)/libz_stdio_c.a

../../$(host_libdir)/libz_format_util.a: libz_format_util.a
	cp libz_format_util.a ../../$(host_libdir)/libz_format_util.a
	
../../$(host_libdir)/libz_stdio_c.a: libz_stdio_c.a
	cp libz_stdio_c.a ../../$(host_libdir)/libz_stdio_c.a

