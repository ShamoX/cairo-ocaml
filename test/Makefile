
include ../config.make

TARGETS = kapow
ifdef LABLGTKDIR
TARGETS += text demo spline basket knockout font
# ifdef GTKCAIRO_CFLAGS
# TARGETS += cube
# endif
endif

all : $(TARGETS)

kapow : kapow.ml
	$(OCAMLOPT) -o $@ -I ../src cairo.cmxa $^

font : font.ml
	$(OCAMLOPT) -o $@ -I ../src cairo.cmxa $^

text : text.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) lablgtk.cmxa cairo.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

demo : demo.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) lablgtk.cmxa cairo.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

cube : cube.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) lablgtk.cmxa cairo.cmxa gtkcairo.cmxa gtkInit.cmx $^

spline : spline.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) lablgtk.cmxa cairo.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

basket : basket.ml
	$(OCAMLOPT) -o $@ -I ../src -I $(LABLGTKDIR) bigarray.cmxa cairo.cmxa lablgtk.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

knockout : knockout.ml
	$(OCAMLOPT) -w s -o $@ -I ../src -I $(LABLGTKDIR) cairo.cmxa lablgtk.cmxa cairo_lablgtk.cmxa gtkInit.cmx $^

clean :
	rm -f *.cm* *.o $(TARGETS) *.ps *.ppm *.png

include ../Makefile.rules
