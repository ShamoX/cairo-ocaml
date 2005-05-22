/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include "ml_cairo.h"

wMake_Val_final_pointer(cairo_pattern_t, cairo_pattern_destroy, 0)

wML_1(cairo_pattern_create_for_surface, cairo_surface_t_val, Val_cairo_pattern_t)

wML_4(cairo_pattern_create_linear, Double_val, Double_val, Double_val, Double_val, Val_cairo_pattern_t)

wML_6(cairo_pattern_create_radial, Double_val, Double_val, Double_val, Double_val, Double_val, Double_val, Val_cairo_pattern_t)

/* pattern_reference */
/* pattern_destroy */

CAMLprim value
ml_cairo_pattern_add_color_stop_rgb (value p, value off, value r, value g, value b)
{
  cairo_status_t s;
  s = cairo_pattern_add_color_stop_rgb (cairo_pattern_t_val (p), Double_val (off),
					Double_val (r), Double_val (g), Double_val (b));
  cairo_treat_status (s);
  return Val_unit;
}

CAMLprim value
ml_cairo_pattern_add_color_stop_rgba (value p, value off, value r, value g, value b, value a)
{
  cairo_status_t s;
  s = cairo_pattern_add_color_stop_rgba (cairo_pattern_t_val (p), Double_val (off),
					 Double_val (r), Double_val (g), Double_val (b), 
					 Double_val (a));
  cairo_treat_status (s);
  return Val_unit;
}
wML_bc6(cairo_pattern_add_color_stop_rgba)

CAMLprim value
ml_cairo_pattern_set_matrix (value p, value m)
{
  cairo_status_t s;
#ifdef ARCH_ALIGN_DOUBLE
  cairo_matrix_t mat;
  ml_convert_cairo_matrix_in (m, &mat);
  s = cairo_pattern_set_matrix (cairo_pattern_t_val (p), &mat);
#else
  s = cairo_pattern_set_matrix (cairo_pattern_t_val (p), cairo_matrix_t_val (m));
#endif
  cairo_treat_status (s);
  return Val_unit;
}

CAMLprim value
ml_cairo_pattern_get_matrix (value p)
{
  cairo_status_t s;
#ifdef ARCH_ALIGN_DOUBLE
  cairo_matrix_t mat;
  s = cairo_pattern_get_matrix (cairo_pattern_t_val (p), &mat);
  cairo_treat_status (s);
  return ml_convert_cairo_matrix_out (m, &mat);
#else
  CAMLparam1(p);
  value m = caml_alloc_small (6 * Double_wosize, Double_array_tag);
  s = cairo_pattern_get_matrix (cairo_pattern_t_val (p), cairo_matrix_t_val (m));
  cairo_treat_status (s);
  CAMLreturn (m);
#endif
}

#define cairo_extend_t_val(v) ((cairo_extend_t) Int_val(v))
#define Val_cairo_extend_t(v) Val_int(v)

CAMLprim value
ml_cairo_pattern_set_extend (value p, value e)
{
  cairo_status_t s;
  s = cairo_pattern_set_extend (cairo_pattern_t_val (p), cairo_extend_t_val (e));
  cairo_treat_status (s);
  return Val_unit;
}

wML_1(cairo_pattern_get_extend, cairo_pattern_t_val, Val_cairo_extend_t)

#define cairo_filter_t_val(v) ((cairo_filter_t) Int_val(v))
#define Val_cairo_filter_t(v) Val_int(v)

CAMLprim value
ml_cairo_pattern_set_filter (value p, value e)
{
  cairo_status_t s;
  s = cairo_pattern_set_filter (cairo_pattern_t_val (p), cairo_filter_t_val (e));
  cairo_treat_status (s);
  return Val_unit;
}

wML_1(cairo_pattern_get_filter, cairo_pattern_t_val, Val_cairo_filter_t)
