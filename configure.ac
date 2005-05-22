AC_INIT(src/cairo.ml)
AC_CONFIG_AUX_DIR(support)

# Check for OCaml programs 
AC_PROG_OCAML()

# Check for cairo
PKG_CHECK_MODULES(CAIRO, cairo >= 0.5.0)

# Optional GTK support (for the X11 backend)
AC_ARG_WITH(gtk, 
	    AS_HELP_STRING([--with-gtk],[X11 backend support via GTK+ and LablGTK]),
	    use_gtk=$withval, use_gtk=yes)

if test $use_gtk = yes ; then
  # Check for LablGTK
  AC_CHECK_OCAML_MODULE(lablgtk, LABLGTKDIR, Gobject, +lablgtk2 +lablgtk)

  if test "$LABLGTKDIR" ; then
    # Check for gdk-pixbuf
    PKG_CHECK_MODULES(GDK, cairo gdk-2.0 gdk-pixbuf-2.0, :, use_gtk=no)
  else
    use_gtk=no
  fi
fi

dnl if test $use_gtk = yes ; then
dnl  # Check for gtkcairo
dnl  PKG_CHECK_MODULES(GTKCAIRO, gtkcairo, use_gtkcairo=yes, use_gtkcairo=no)
dnl else
dnl  use_gtkcairo=no
dnl fi

# Optional libsvg-cairo support
PKG_CHECK_MODULES(LIBSVG_CAIRO, libsvg-cairo, use_libsvg_cairo=yes, use_libsvg_cairo=no)

echo
echo "  GTK+         support: $use_gtk"
dnl echo "  GTKCairo     support: $use_gtkcairo"
echo "  libsvg-cairo support: $use_libsvg_cairo"
echo

AC_OUTPUT(config.make)
