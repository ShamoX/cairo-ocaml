AC_INIT(src/cairo.ml)
AC_CONFIG_AUX_DIR(support)

# Check for OCaml programs 
AC_PROG_OCAML()

# Check for LablGTK
AC_CHECK_OCAML_MODULE(lablgtk, LABLGTKDIR, Gobject, +lablgtk2 +lablgtk)

# Check for cairo
PKG_CHECK_MODULES(CAIRO, cairo >= 0.1.21)

# Check for gdk-pixbuf
PKG_CHECK_MODULES(GDK, cairo gdk-2.0 gdk-pixbuf-2.0)

# Check for gtkcairo
PKG_CHECK_MODULES(GTKCAIRO, gtkcairo, : , : )

AC_OUTPUT(config.make)
