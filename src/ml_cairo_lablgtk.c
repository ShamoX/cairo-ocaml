#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkx.h>

#include <cairo.h>

#include <caml/mlvalues.h>
#include <caml/alloc.h>

#include "wrappers.h"
#include "ml_gobject.h"
#include "ml_gdkpixbuf.h"
#include "ml_gdk.h"

#include "ml_cairo.h"
#include "ml_cairo_status.h"

CAMLprim value
cairo_lablgtk_of_pixbuf(value pb)
{
  value v;
  GdkPixbuf *pixbuf = GdkPixbuf_val(pb);
  cairo_format_t format;
  gboolean alpha = gdk_pixbuf_get_has_alpha(pixbuf);
  int nchan = gdk_pixbuf_get_n_channels(pixbuf);
  int bps = gdk_pixbuf_get_bits_per_sample(pixbuf);

  if ((nchan == 4) && (bps == 8) && alpha)
    format = CAIRO_FORMAT_ARGB32;
  else 
    failwith("bad GdkPixbuf format");

  v = alloc_small(5, 0);
  Field(v, 0) = Val_bp(gdk_pixbuf_get_pixels(pixbuf));
  Field(v, 1) = Val_cairo_format_t(format);
  Field(v, 2) = Val_int(gdk_pixbuf_get_width(pixbuf));
  Field(v, 3) = Val_int(gdk_pixbuf_get_height(pixbuf));
  Field(v, 4) = Val_int(gdk_pixbuf_get_rowstride(pixbuf));

  return v;
}

CAMLprim value
cairo_lablgtk_shuffle_pixels(value pb)
{
  GdkPixbuf *pixbuf = GdkPixbuf_val(pb);
  guint w, h, s, i, j;
  guchar *pixels, *p;

  g_return_val_if_fail (gdk_pixbuf_get_has_alpha(pixbuf) &&
			(gdk_pixbuf_get_n_channels(pixbuf) == 4) &&
			(gdk_pixbuf_get_bits_per_sample(pixbuf) == 8), Val_unit);

  w = gdk_pixbuf_get_width(pixbuf);
  h = gdk_pixbuf_get_height(pixbuf);
  s = gdk_pixbuf_get_rowstride(pixbuf);
  pixels = gdk_pixbuf_get_pixels(pixbuf);

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


#ifdef CAIRO_HAS_XLIB_SURFACE
CAMLprim value
cairo_lablgtk_surface_create_for_drawable(value d, value fmt)
{
  GdkDrawable *draw = GdkDrawable_val(d);
  cairo_surface_t *s;
  
  s = cairo_xlib_surface_create (
				 GDK_DRAWABLE_XDISPLAY(draw),
				 GDK_DRAWABLE_XID(draw),
				 GDK_VISUAL_XVISUAL(gdk_drawable_get_visual(draw)),
				 cairo_format_t_val(fmt),
				 GDK_COLORMAP_XCOLORMAP(gdk_drawable_get_colormap(draw)));
  return Val_cairo_surface_t(s);
}

CAMLprim value
cairo_lablgtk_set_target_drawable(value cr, value d)
{
  cairo_t *c = cairo_t_val(cr);
  GdkDrawable *draw = GdkDrawable_val(d);
  GdkDrawable *real_drawable;
  gint x_offset, y_offset;

  if (GDK_IS_WINDOW(draw))
    gdk_window_get_internal_paint_info(GDK_WINDOW(draw),
				       &real_drawable,
				       &x_offset, &y_offset);
  else {
    real_drawable = draw;
    x_offset = 0;
    y_offset = 0;
  }
    
  cairo_set_target_drawable(c,
			    GDK_DRAWABLE_XDISPLAY(real_drawable),
			    GDK_DRAWABLE_XID(real_drawable));
  check_cairo_status(cr);
  cairo_translate(c, -x_offset, -y_offset);
  check_cairo_status(cr);

  return Val_unit;
}

#else

Unsupported(cairo_lablgtk_surface_create_for_drawable)
Unsupported(cairo_lablgtk_set_target_drawable)

#endif /* CAIRO_HAS_XLIB_SURFACE */
