/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include "ml_cairo.h"

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdk.h>

#include "wrappers.h"
#include "ml_gobject.h"
#include "ml_gdkpixbuf.h"
#include "ml_gdk.h"


#if ! GTK_CHECK_VERSION(2,8,0)
/* For "old" versions of GTK+, provide the GTK+/cairo integration,
   (stolen from GTK+)
*/
#include <gdk/gdkx.h>
#ifdef CAIRO_HAS_XLIB_SURFACE
#  include <cairo-xlib.h>
#else
#  error "Cairo was not compiled with support for the xlib backend"
#endif

static cairo_t *
gdk_cairo_create (GdkDrawable *target)
{
  int width, height;
  int x_off=0, y_off=0;
  cairo_t *cr;
  cairo_surface_t *surface;
  GdkDrawable *drawable = target;
  GdkVisual *visual;

  g_return_val_if_fail (GDK_IS_DRAWABLE (drawable), NULL);

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

  cr = cairo_create (surface);
  cairo_surface_destroy (surface);

  return cr;
}

static void
gdk_cairo_set_source_color (cairo_t  *cr,
			    GdkColor *color)
{
  g_return_if_fail (cr != NULL);
  g_return_if_fail (color != NULL);
    
  cairo_set_source_rgb (cr,
			color->red / 65535.,
			color->green / 65535.,
			color->blue / 65535.);
}

static void
gdk_cairo_rectangle (cairo_t      *cr,
		     GdkRectangle *rectangle)
{
  g_return_if_fail (cr != NULL);
  g_return_if_fail (rectangle != NULL);

  cairo_rectangle (cr,
		   rectangle->x,     rectangle->y,
		   rectangle->width, rectangle->height);
}

static void
gdk_cairo_region (cairo_t   *cr,
		  GdkRegion *region)
{
  caml_failwith("Cairo_lablgtk.region is unsupported with this version of GTK+");
}

static void
gdk_cairo_set_source_pixbuf (cairo_t   *cr,
			     GdkPixbuf *pixbuf,
			     double     pixbuf_x,
			     double     pixbuf_y)
{
  gint width = gdk_pixbuf_get_width (pixbuf);
  gint height = gdk_pixbuf_get_height (pixbuf);
  guchar *gdk_pixels = gdk_pixbuf_get_pixels (pixbuf);
  int gdk_rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  int n_channels = gdk_pixbuf_get_n_channels (pixbuf);
  guchar *cairo_pixels;
  cairo_format_t format;
  cairo_surface_t *surface;
  static const cairo_user_data_key_t key;
  int j;

  if (n_channels == 3)
    format = CAIRO_FORMAT_RGB24;
  else
    format = CAIRO_FORMAT_ARGB32;

  cairo_pixels = g_malloc (4 * width * height);
  surface = cairo_image_surface_create_for_data ((unsigned char *)cairo_pixels,
						 format,
						 width, height, 4 * width);
  cairo_surface_set_user_data (surface, &key,
			       cairo_pixels, (cairo_destroy_func_t)g_free);

  for (j = height; j; j--)
    {
      guchar *p = gdk_pixels;
      guchar *q = cairo_pixels;

      if (n_channels == 3)
	{
	  guchar *end = p + 3 * width;
	  
	  while (p < end)
	    {
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
	      q[0] = p[2];
	      q[1] = p[1];
	      q[2] = p[0];
#else	  
	      q[1] = p[0];
	      q[2] = p[1];
	      q[3] = p[2];
#endif
	      p += 3;
	      q += 4;
	    }
	}
      else
	{
	  guchar *end = p + 4 * width;
	  guint t1,t2,t3;
	    
#define MULT(d,c,a,t) G_STMT_START { t = c * a; d = ((t >> 8) + t) >> 8; } G_STMT_END

	  while (p < end)
	    {
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
	      MULT(q[0], p[2], p[3], t1);
	      MULT(q[1], p[1], p[3], t2);
	      MULT(q[2], p[0], p[3], t3);
	      q[3] = p[3];
#else	  
	      q[0] = p[3];
	      MULT(q[1], p[0], p[3], t1);
	      MULT(q[2], p[1], p[3], t2);
	      MULT(q[3], p[2], p[3], t3);
#endif
	      
	      p += 4;
	      q += 4;
	    }
	  
#undef MULT
	}

      gdk_pixels += gdk_rowstride;
      cairo_pixels += 4 * width;
    }

  cairo_set_source_surface (cr, surface, pixbuf_x, pixbuf_y);
  cairo_surface_destroy (surface);
}

#endif /* GTK_CHECK_VERSION(2,8,0) */

wML_1(gdk_cairo_create, GdkDrawable_val, Val_cairo_t)
wML_2(gdk_cairo_set_source_color, cairo_t_val, GdkColor_val, Unit)
wML_2(gdk_cairo_rectangle, cairo_t_val, GdkRectangle_val, Unit)
wML_2(gdk_cairo_region, cairo_t_val, GdkRegion_val, Unit)
wML_4(gdk_cairo_set_source_pixbuf, cairo_t_val, GdkPixbuf_val, Double_val, Double_val, Unit)
