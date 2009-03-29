# POSIX makefile for Zaimoni.STL
# POSIX requires make sets $(MAKE)
# pure.C must be done first because of Compiler.h dependency on auto_int.h

# GNU targets we know about
all:
	cd pure.C; $(MAKE) all
	cd core.RAM; $(MAKE) all
	cd LexParse; $(MAKE) all
	cd OS; $(MAKE) all
	cd OS; cd Win32; $(MAKE) all

clean:
	cd pure.C; $(MAKE) clean
	cd core.RAM; $(MAKE) clean
	cd LexParse; $(MAKE) clean
	cd OS; $(MAKE) clean
	cd OS; cd Win32; $(MAKE) clean

install:
	cd pure.C; $(MAKE) install
	cd core.RAM; $(MAKE) install
	cd LexParse; $(MAKE) install
	cd OS; $(MAKE) install
	cd OS; cd Win32; $(MAKE) install

uninstall:
	cd pure.C; $(MAKE) uninstall
	cd core.RAM; $(MAKE) uninstall
	cd LexParse; $(MAKE) uninstall
	cd OS; $(MAKE) uninstall
	cd OS; cd Win32; $(MAKE) uninstall

