



all opt doc install clean :
	$(MAKE) -C src $@

configure : configure.ac
	aclocal -I support
	autoconf
config.make : config.make.in configure
	$(error run ./configure)
include config.make

.PHONY : doc
