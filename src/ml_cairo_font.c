/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include "ml_cairo.h"

wMake_Val_final_pointer(cairo_font_face_t, cairo_font_face_destroy, 0)

wMake_Val_final_pointer(cairo_scaled_font_t, cairo_scaled_font_destroy, 0)

wMake_Val_final_pointer(cairo_font_options_t, cairo_font_options_destroy, 0)
/* XXX: could be using cairo_font_options_equal and cairo_font_options_hash here ... */

/* font_face_reference */
/* font_face_destroy */
/* font_face_get_user_data */
/* font_face_set_user_data */

CAMLprim value
ml_cairo_font_options_create (value unit)
{
  cairo_font_options_t *o = cairo_font_options_create();
  cairo_treat_status (cairo_font_options_status (o));
  return Val_cairo_font_options_t (o);
}

wML_2(cairo_font_options_merge, cairo_font_options_t_val, cairo_font_options_t_val, Unit)
wML_2(cairo_font_options_set_antialias, cairo_font_options_t_val, Long_val, Unit)
wML_1(cairo_font_options_get_antialias, cairo_font_options_t_val, Val_long)
wML_2(cairo_font_options_set_subpixel_order, cairo_font_options_t_val, Long_val, Unit)
wML_1(cairo_font_options_get_subpixel_order, cairo_font_options_t_val, Val_long)
wML_2(cairo_font_options_set_hint_style, cairo_font_options_t_val, Long_val, Unit)
wML_1(cairo_font_options_get_hint_style, cairo_font_options_t_val, Val_long)
wML_2(cairo_font_options_set_hint_metrics, cairo_font_options_t_val, Long_val, Unit)
wML_1(cairo_font_options_get_hint_metrics, cairo_font_options_t_val, Val_long)


CAMLprim value
ml_cairo_scaled_font_create (value f, value fmat, value ctm, value fo)
{
  cairo_scaled_font_t *sf;
#ifndef ARCH_ALIGN_DOUBLE
  sf = cairo_scaled_font_create (cairo_font_face_t_val (f), 
				 cairo_matrix_t_val (fmat), 
				 cairo_matrix_t_val (ctm),
				 cairo_font_options_t_val (fo));
#else
  cairo_matrix_t c_fmat, c_ctm;
  ml_convert_cairo_matrix_in (fmat, &c_fmat);
  ml_convert_cairo_matrix_in (ctm, &c_ctm);
  sf = cairo_scaled_font_create (cairo_font_face_t_val (f), 
				 &c_fmat, &c_ctm,
				 cairo_font_options_t_val (fo));
#endif
  return Val_cairo_scaled_font_t (sf);
}

/* scaled_font_face_reference */
/* scaled_font_face_destroy */

CAMLprim value
ml_cairo_scaled_font_extents (value sf)
{
  cairo_font_extents_t e;
  cairo_scaled_font_extents (cairo_scaled_font_t_val (sf), &e);
  cairo_treat_status (cairo_scaled_font_status (cairo_scaled_font_t_val (sf)));
  return Val_cairo_font_extents (&e);
}

CAMLprim value
ml_cairo_scaled_font_glyph_extents (value sf, value v_glyphs)
{
  int num_glyphs;
  cairo_glyph_t *c_glyphs;
  cairo_text_extents_t c_extents;
  c_glyphs = ml_convert_cairo_glypth_in (v_glyphs, &num_glyphs);
  cairo_scaled_font_glyph_extents (cairo_scaled_font_t_val (sf), 
				   c_glyphs, num_glyphs, &c_extents);
  caml_stat_free (c_glyphs);
  return Val_cairo_text_extents (&c_extents);
}
