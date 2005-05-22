/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include "ml_cairo.h"

#if CAIRO_HAS_PNG_FUNCTIONS
wML_1(cairo_image_surface_create_from_png, String_val, Val_cairo_surface_t)

CAMLprim value
ml_cairo_image_surface_create_from_png_stream (value f)
{
  CAMLparam0();
  CAMLlocal1(c);
  cairo_surface_t *surf;

  c = caml_alloc_small (2, 0);
  Field (c, 0) = f;
  Field (c, 1) = Val_unit;
  surf = cairo_image_surface_create_from_png_stream (ml_cairo_read_func, &c);
  if (Is_exception_result (Field (c, 1)))
    caml_raise (Extract_exception (Field (c, 1)));

  CAMLreturn (Val_cairo_surface_t (surf));
}

CAMLprim value
ml_cairo_surface_write_to_png (value surf, value fname)
{
  cairo_status_t s;
  s = cairo_surface_write_to_png (cairo_surface_t_val (surf), String_val (fname));
  cairo_treat_status (s);
  return Val_unit;
}

CAMLprim value
ml_cairo_surface_write_to_png_stream (value surf, value f)
{
  CAMLparam1(surf);
  CAMLlocal1(c);
  cairo_status_t s;

  c = caml_alloc_small (2, 0);
  Field (c, 0) = f;
  Field (c, 1) = Val_unit;

  s = cairo_surface_write_to_png_stream (cairo_surface_t_val (surf),
					 ml_cairo_write_func,
					 &c);
  if (Is_exception_result (Field (c, 1)))
    caml_raise (Extract_exception (Field (c, 1)));
  cairo_treat_status (s);

  CAMLreturn (Val_unit);
}

#else

Cairo_Unsupported(cairo_image_surface_create_from_png,        "PNG functions not supported")
Cairo_Unsupported(cairo_image_surface_create_from_png_stream, "PNG functions not supported")
Cairo_Unsupported(cairo_surface_write_to_png,        "PNG functions not supported")
Cairo_Unsupported(cairo_surface_write_to_png_stream, "PNG functions not supported")

#endif
