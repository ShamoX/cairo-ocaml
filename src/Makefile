
include ../config.make

CPPFLAGS = -g

TARGETS = cairo
ifdef LABLGTKDIR
TARGETS += lablgtk
ifdef GTKCAIRO_CFLAGS
TARGETS += gtkcairo
endif
endif

all : $(TARGETS) $(if $(OCAMLOPT),opt)
opt : $(addsuffix .opt,$(TARGETS))

cairo     : cairo.cma  libmlcairo.a
cairo.opt : cairo.cmxa dllmlcairo.so
lablgtk     : cairo_lablgtk.cma  libmlcairo_lablgtk.a
lablgtk.opt : cairo_lablgtk.cmxa dllmlcairo_lablgtk.so
gtkcairo     : gtkcairo.cma  libmlgtkcairo.a
gtkcairo.opt : gtkcairo.cmxa dllmlgtkcairo.so

cairo_SRC = cairo_channel.mli cairo.mli cairo.ml \
            cairo_bigarray.mli cairo_bigarray.ml \
	    ml_cairo_status.c ml_cairo_channel.c \
            ml_cairo.c ml_cairo_bigarr.c ml_cairo_path.c

cairo.cma : $(call mlobjs,$(cairo_SRC))
	$(OCAMLMKLIB) -o cairo -oc mlcairo $^ $(CAIRO_LIBS)
cairo.cmxa : $(call mloptobjs,$(cairo_SRC))
	$(OCAMLMKLIB) -o cairo -oc mlcairo $^ $(CAIRO_LIBS)
libmlcairo.a dllmlcairo.so : $(call cobjs,$(cairo_SRC))
	$(OCAMLMKLIB) -o cairo -oc mlcairo $^ $(CAIRO_LIBS)

lablgtk_SRC = cairo_lablgtk.mli cairo_lablgtk.ml ml_cairo_lablgtk.c

cairo_lablgtk.cma : $(call mlobjs,$(lablgtk_SRC))
	$(OCAMLMKLIB) -o cairo_lablgtk -oc mlcairo_lablgtk $^
cairo_lablgtk.cmxa : $(call mloptobjs,$(lablgtk_SRC))
	$(OCAMLMKLIB) -o cairo_lablgtk -oc mlcairo_lablgtk $^
libmlcairo_lablgtk.a dllmlcairo_lablgtk.so : $(call cobjs,$(lablgtk_SRC))
	$(OCAMLMKLIB) -o cairo_lablgtk -oc mlcairo_lablgtk $^

gtkcairo_SRC = cairo_gtkcairo.mli cairo_gtkcairo.ml ml_cairo_gtkcairo.c

gtkcairo.cma : $(call mlobjs,$(gtkcairo_SRC))
	$(OCAMLMKLIB) -o gtkcairo -oc mlgtkcairo $^ $(GTKCAIRO_LIBS)
gtkcairo.cmxa : $(call mloptobjs,$(gtkcairo_SRC))
	$(OCAMLMKLIB) -o gtkcairo -oc mlgtkcairo $^ $(GTKCAIRO_LIBS)
libmlgtkcairo.a dllmlgtkcairo.so : $(call cobjs,$(gtkcairo_SRC))
	$(OCAMLMKLIB) -o gtkcairo -oc mlgtkcairo $^ $(GTKCAIRO_LIBS)

$(call cobjs,$(cairo_SRC))    : CPPFLAGS+=$(CAIRO_CFLAGS)
$(call cobjs,$(lablgtk_SRC))  : CPPFLAGS+=$(GDK_CFLAGS) -I$(C_LABLGTKDIR)
$(call cobjs,$(gtkcairo_SRC)) : CPPFLAGS+=$(GTKCAIRO_CFLAGS) -I$(C_LABLGTKDIR)
$(call mlobjs,$(lablgtk_SRC))  : INCFLAGS=-I $(LABLGTKDIR)
$(call mlobjs,$(gtkcairo_SRC)) : INCFLAGS=-I $(LABLGTKDIR)
$(call mlintfs,$(lablgtk_SRC))  : INCFLAGS=-I $(LABLGTKDIR)
$(call mlintfs,$(gtkcairo_SRC)) : INCFLAGS=-I $(LABLGTKDIR)
$(call mloptobjs,$(lablgtk_SRC))  : INCFLAGS=-I $(LABLGTKDIR)
$(call mloptobjs,$(gtkcairo_SRC)) : INCFLAGS=-I $(LABLGTKDIR)

install: all
	mkdir -p $(DESTDIR)$(INSTALLDIR) $(DESTDIR)$(OCAMLLIB)/stublibs
	install -m 644 *.mli *.cmi *.cma lib*.a $(DESTDIR)$(INSTALLDIR)
ifdef OCAMLOPT
	install -m644 *.cmxa *.a *.cmx $(DESTDIR)$(INSTALLDIR)
endif
	install -m 755 dll*.so $(DESTDIR)$(INSTALLDIR)
	if test -w $(DESTDIR)$(OCAMLLIB)/stublibs ; then \
          for lib in dll*.so ; do \
            ln -s $(INSTALLDIR)/$$lib $(DESTDIR)$(OCAMLLIB)/stublibs ; done ; fi

DOCFILES = cairo_channel.mli cairo.mli cairo_bigarray.mli
ifdef LABLGTKDIR
DOCFILES += cairo_lablgtk.mli
ifdef GTKCAIRO_CFLAGS
DOCFILES += cairo_gtkcairo.mli
endif
endif
doc: $(DOCFILES:%.mli=%.cmi)
	mkdir -p ../doc/html
	ocamldoc -v -html -d ../doc/html -t Cairo-ocaml $(if $(LABLGTKDIR),-I $(LABLGTKDIR)) $(DOCFILES) 

clean :
	rm -f *.cm* *.o *.a *.so

.depend : $(wildcard *.ml *.mli)
	@echo "making deps"
	@$(OCAMLDEP) $^ > $@

-include .depend
include .depend_c

.PHONY : cairo lablgtk gtkcairo doc

include ../Makefile.rules
