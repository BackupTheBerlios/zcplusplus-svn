# Makefile for OS directory
# assumes GNU make

.SUFFIXES:
.SUFFIXES: .cpp .a .o

include ../makeconf.inc
include ../GNUdep.inc

CFLAGS := -Os -Wall -Wextra -finline -Winline -Wdisabled-optimization --param max-inline-insns-single=500 -c -pipe -DNDEBUG
CXXFLAGS := $(CFLAGS)

LIBS := libz_stdio_log.a libz_stdio_log_c.a libz_log_adapter.a

# GNU targets we know about
all: $(LIBS)

clean:
	rm -f *.a *.o

install: $(addprefix $(libdir)/, $(LIBS))

uninstall:
	rm -f $(addprefix $(libdir)/, $(LIBS))

# rules
stdio_log_c.o: stdio_log.cpp
	$(CC) -c -xc stdio_log.cpp $(CFLAGS) $(ARCH_FLAGS) -o stdio_log_c.o

stdio_log.o: stdio_log.cpp
log_adapter_inform.o: log_adapter_inform.cpp ../pure.C/format_util.c
log_adapter_inc_inform.o: log_adapter_inc_inform.cpp ../pure.C/format_util.c
log_adapter_log.o: log_adapter_log.cpp ../pure.C/format_util.c

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS) $(ARCH_FLAGS) -o $*.o

libz_stdio_log.a: stdio_log.o
	$(AR) -ru libz_stdio_log.a stdio_log.o

libz_stdio_log_c.a: stdio_log_c.o
	$(AR) -ru libz_stdio_log_c.a stdio_log_c.o

libz_log_adapter.a: log_adapter_inc_inform.o log_adapter_inform.o log_adapter_log.o
	$(AR) -ru libz_log_adapter.a log_adapter_inc_inform.o log_adapter_inform.o log_adapter_log.o

$(libdir)/libz_stdio_log.a: libz_stdio_log.a
	cp libz_stdio_log.a $(libdir)/libz_stdio_log.a

$(libdir)/libz_stdio_log_c.a: libz_stdio_log_c.a
	cp libz_stdio_log_c.a $(libdir)/libz_stdio_log_c.a

$(libdir)/libz_log_adapter.a: libz_log_adapter.a
	cp libz_log_adapter.a $(libdir)/libz_log_adapter.a

