
include ../config.make

ifdef LABLGTKDIR
TARGETS += spline basket knockout oknockout
ifdef GTKCAIRO_CFLAGS
TARGETS += cube
endif
endif

all : $(TARGETS)

cube : cube.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) lablgtk.cmxa cairo.cmxa cairo_lablgtk.cmxa gtkcairo.cmxa gtkInit.cmx $^

spline : spline.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) lablgtk.cmxa cairo.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

basket : basket.ml
	$(OCAMLOPT) -o $@ -I ../src -I $(LABLGTKDIR) bigarray.cmxa cairo.cmxa lablgtk.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

knockout : knockout.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) cairo.cmxa lablgtk.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

oknockout : oknockout.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) cairo.cmxa lablgtk.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

clean :
	rm -f *.cm* *.o $(TARGETS) *.ps *.ppm *.png

include ../Makefile.rules
