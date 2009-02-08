# Makefile for LexParse directory
# assumes GNU make

.SUFFIXES:
.SUFFIXES: .cpp .a .o

include ../makeconf.inc
include ../GNUdep.inc

CXXFLAGS := -Os -Wall -Wextra -finline -Winline -Wdisabled-optimization --param max-inline-insns-single=500 -c -pipe -DNDEBUG

LIBS := libz_langconf.a

# GNU targets we know about
all: $(LIBS)

clean:
	rm -f *.a *.o

install: $(addprefix $(libdir)/, $(LIBS))

uninstall:
	rm -f $(addprefix $(libdir)/, $(LIBS))

# rules
LangConf.o: LangConf.cpp $(LangConf_hpp) $(addprefix ../, $(POD_hpp))

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS) $(ARCH_FLAGS) $(FORCE_ISO) -o $*.o

libz_langconf.a: LangConf.o
	$(AR) -ru libz_langconf.a LangConf.o

$(libdir)/libz_langconf.a: libz_langconf.a
	cp libz_langconf.a $(libdir)/libz_langconf.a

