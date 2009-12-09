# Makefile for LexParse directory
# assumes GNU make

.SUFFIXES:
.SUFFIXES: .cpp .a .o

include ../POSIX.inc
include ../POSIX.dep

CXXFLAGS := -Os -Wall -Wextra -finline -Winline -Wdisabled-optimization --param max-inline-insns-single=500 -c -pipe -DNDEBUG

LIBS := libz_langconf.a libz_langconf_iso.a

# GNU targets we know about
all: $(LIBS)

clean:
	rm -f *.a *.o

install: $(addprefix $(libdir)/, $(LIBS))

uninstall:
	rm -f $(addprefix $(libdir)/, $(LIBS))

# rules
LangConf.o: LangConf.cpp $(LangConf_hpp) $(addprefix ../, $(POD_hpp))
LangConf_iso.o: LangConf.cpp $(LangConf_hpp) $(addprefix ../, $(POD_hpp))

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS) $(ARCH_FLAGS) -o $*.o

LangConf_iso.o:
	$(CXX) -c $< $(CXXFLAGS) $(ARCH_FLAGS) -DZAIMONI_FORCE_ISO -o $*.o

libz_langconf.a: LangConf.o
	$(AR) -ru libz_langconf.a LangConf.o

libz_langconf_iso.a: LangConf_iso.o
	$(AR) -ru libz_langconf_iso.a LangConf_iso.o

$(libdir)/libz_langconf.a: libz_langconf.a
	cp libz_langconf.a $(libdir)/libz_langconf.a

$(libdir)/libz_langconf_iso.a: libz_langconf_iso.a
	cp libz_langconf_iso.a $(libdir)/libz_langconf_iso.a

