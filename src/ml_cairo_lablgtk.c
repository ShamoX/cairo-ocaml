/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include "ml_cairo.h"
#if CAIRO_HAS_XLIB_SURFACE
# include <cairo-xlib.h>
#endif

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#include "wrappers.h"
#include "ml_gobject.h"
#include "ml_gdkpixbuf.h"
#include "ml_gdk.h"

CAMLprim value
ml_cairo_lablgtk_of_pixbuf (value pb)
{
  GdkPixbuf *pixbuf = GdkPixbuf_val(pb);
  cairo_format_t format;
  gboolean alpha = gdk_pixbuf_get_has_alpha(pixbuf);
  int nchan = gdk_pixbuf_get_n_channels(pixbuf);
  int bps = gdk_pixbuf_get_bits_per_sample(pixbuf);
  cairo_surface_t *surf;

  if ((nchan == 4) && (bps == 8) && alpha)
    format = CAIRO_FORMAT_ARGB32;
  else 
    caml_invalid_argument ("bad GdkPixbuf format");

  surf = cairo_image_surface_create_for_data (gdk_pixbuf_get_pixels (pixbuf),
					      format,
					      gdk_pixbuf_get_width(pixbuf),
					      gdk_pixbuf_get_height(pixbuf),
					      gdk_pixbuf_get_rowstride(pixbuf));

  ml_cairo_surface_set_image_data (surf, pb);

  return Val_cairo_surface_t (surf);
}

CAMLprim value
ml_cairo_lablgtk_shuffle_pixels (value pb)
{
  GdkPixbuf *pixbuf = GdkPixbuf_val(pb);
  guint w, h, s, i, j;
  guchar *pixels, *p;

  g_return_val_if_fail (gdk_pixbuf_get_has_alpha(pixbuf) &&
			(gdk_pixbuf_get_n_channels(pixbuf) == 4) &&
			(gdk_pixbuf_get_bits_per_sample(pixbuf) == 8), Val_unit);

  w = gdk_pixbuf_get_width (pixbuf);
  h = gdk_pixbuf_get_height (pixbuf);
  s = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  for (i=0; i<h; i++) {
    p = pixels;
    for (j=0; j<w; j++) {
      guchar red = p[0];
      p[0] = p[2];
      p[2] = red;
      p += 4;
    }
    pixels += s;
  }

  return Val_unit;
}


#if CAIRO_HAS_XLIB_SURFACE
/* copied from pycairo, who got it from GTK+ */
static cairo_surface_t *
ml_gdk_cairo_surface_create (GdkDrawable *target)
{
    int width, height;
    int x_off=0, y_off=0;
    cairo_surface_t *surface;
    GdkDrawable *drawable = target;
    GdkVisual *visual;

    if (GDK_IS_WINDOW(target)) {
        /* query the window's backbuffer if it has one */
	GdkWindow *window = GDK_WINDOW(target);
	gdk_window_get_internal_paint_info (window,
					    &drawable, &x_off, &y_off);
    }
    visual = gdk_drawable_get_visual (drawable);
    gdk_drawable_get_size (drawable, &width, &height);

    if (visual) {
	surface = cairo_xlib_surface_create (GDK_DRAWABLE_XDISPLAY (drawable),
					     GDK_DRAWABLE_XID (drawable),
					     GDK_VISUAL_XVISUAL (visual),
					     width, height);
    } else if (gdk_drawable_get_depth (drawable) == 1) {
	surface = cairo_xlib_surface_create_for_bitmap
	    (GDK_PIXMAP_XDISPLAY (drawable),
	     GDK_PIXMAP_XID (drawable),
	     GDK_SCREEN_XSCREEN (gdk_drawable_get_screen (drawable)),
	     width, height);
    } else {
	g_warning ("Using Cairo rendering requires the drawable argument to\n"
		   "have a specified colormap. All windows have a colormap,\n"
		   "however, pixmaps only have colormap by default if they\n"
		   "were created with a non-NULL window argument. Otherwise\n"
		   "a colormap must be set on them with "
		   "gdk_drawable_set_colormap");
	return NULL;
    }
    cairo_surface_set_device_offset (surface, -x_off, -y_off);
    return surface;
}

CAMLprim value
ml_cairo_xlib_surface_create (value d)
{
  return Val_cairo_surface_t (ml_gdk_cairo_surface_create (GdkDrawable_val(d)));
}

ML_3 (cairo_xlib_surface_set_size, cairo_surface_t_val, Int_val, Int_val, Unit)

CAMLprim value
ml_cairo_xlib_surface_set_drawable (value s, value d, value w, value h)
{
  cairo_xlib_surface_set_drawable (cairo_surface_t_val (s),
				   GDK_DRAWABLE_XID (GdkDrawable_val (d)),
				   Int_val (w),
				   Int_val (h));
  return Val_unit;
}

#else

Cairo_Unsupported(cairo_xlib_surface_create, "Xlib backend not supported");
Cairo_Unsupported(cairo_xlib_surface_set_size, "Xlib backend not supported");
Cairo_Unsupported(cairo_xlib_surface_set_drawable, "Xlib backend not supported");

#endif /* CAIRO_HAS_XLIB_SURFACE */
