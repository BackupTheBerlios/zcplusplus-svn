# Makefile for OS/Win32 directory
# assumes GNU make

.SUFFIXES:
.SUFFIXES: .cpp .a .o

include ../../makeconf.inc

CFLAGS := -Os -Wall -Wextra -finline -Winline -Wdisabled-optimization --param max-inline-insns-single=500 -c -pipe -DNDEBUG
CXXFLAGS := $(CFLAGS)

LIBS := libz_mutex.a

# GNU targets we know about
all: $(LIBS)

clean:
	rm -f *.a *.o

install: $(addprefix $(libdir)/, $(LIBS))

uninstall:
	rm -f $(addprefix $(libdir)/, $(LIBS))

# rules
mutex.o: mutex.cpp
	$(CXX) -c mutex.cpp $(CXXFLAGS) $(ARCH_FLAGS) -o mutex.o

libz_mutex.a: mutex.o
	$(AR) -ru libz_mutex.a mutex.o

#define lib_install
#$(libdir)/$(1): $(1)
#	cp $(1) $(libdir)/$(1)
#endef

#$(foreach lib,$(LIBS),$(eval(call lib_install,$(lib))))
$(libdir)/libz_mutex.a: libz_mutex.a
	cp libz_mutex.a $(libdir)/libz_mutex.a

