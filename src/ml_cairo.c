#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/custom.h>

#include "ml_cairo_wrappers.h"

#include <cairo.h>
#include "ml_cairo_channel.h"
#include "ml_cairo_status.h"
#include "ml_cairo.h"

static void
ml_final_cairo_t (value val)
{
  cairo_t *cr = cairo_t_val (val);
  if (cr != NULL)
    cairo_destroy (cr);
}

static struct custom_operations ml_custom_cairo_t = {
  "cairo_t/001", ml_final_cairo_t, ml_pointer_compare,
  ml_pointer_hash, custom_serialize_default, custom_deserialize_default
};

value
Val_cairo_t (cairo_t * p)
{
  value ret;
  struct ml_cairo *ml_c;
  if (p == NULL)
    report_null_pointer;
  ret = alloc_custom (&ml_custom_cairo_t, sizeof (struct ml_cairo), 20, 1000);
  ml_c = Data_custom_val (ret);
  ml_c->cr = p;
  ml_c->suspend_exn = 0;
  return ret;
}

Make_Val_final_pointer(cairo_surface_t, Ignore, cairo_surface_destroy, 20)
#define cairo_surface_t_val(v) ((cairo_surface_t *)Pointer_val(v))

Make_Val_final_pointer(cairo_matrix_t, Ignore, cairo_matrix_destroy, 100)
#define cairo_matrix_t_val(v) ((cairo_matrix_t *)Pointer_val(v))

Make_Val_final_pointer(cairo_pattern_t, Ignore, cairo_pattern_destroy, 20)
#define cairo_pattern_t_val(v) ((cairo_pattern_t *)Pointer_val(v))

CAMLprim value
ml_cairo_suspend_exn (value v_cr)
{
  struct ml_cairo *ml_c = Data_custom_val (v_cr);
  ml_c->suspend_exn = 1;
  return Val_unit;
}

CAMLprim value
ml_cairo_resume_exn (value v_cr)
{
  struct ml_cairo *ml_c = Data_custom_val (v_cr);
  ml_c->suspend_exn = 0;
  cairo_treat_status (cairo_status (ml_c->cr));
  return Val_unit;
}

CAMLprim value
ml_cairo_get_suspend_exn (value v_cr)
{
  struct ml_cairo *ml_c = Data_custom_val (v_cr);
  return Val_bool (ml_c->suspend_exn);
}

ML_0(cairo_create, Val_cairo_t)

ML_1(cairo_destroy, cairo_t_val, Unit)

CAMLprim value
ml_cairo_save (value v_cr)
{
  cairo_save (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_restore (value v_cr)
{
  cairo_restore (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}
ML_2(cairo_copy, cairo_t_val, cairo_t_val, Unit)

CAMLprim value
ml_cairo_set_target_surface (value v_cr, value v_surface)
{
  cairo_set_target_surface (cairo_t_val (v_cr),
			    cairo_surface_t_val (v_surface));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_target_image (value cr, value img)
{
  cairo_set_target_image (cairo_t_val (cr),
			  Bp_val (Field (img, 0)),
			  cairo_format_t_val (Field (img, 1)),
			  Int_val (Field (img, 2)),
			  Int_val (Field (img, 3)), Int_val (Field (img, 4)));
  check_cairo_status (cr);
  return Val_unit;
}

#ifdef CAIRO_HAS_PS_SURFACE
CAMLprim value
ml_cairo_set_target_ps (value v_cr, value v_file, value v_width_inches,
			value v_height_inches, value v_x_pixels_per_inch,
			value v_y_pixels_per_inch)
{
  cairo_set_target_ps (cairo_t_val (v_cr), FILE_val (v_file),
		       Double_val (v_width_inches),
		       Double_val (v_height_inches),
		       Double_val (v_x_pixels_per_inch),
		       Double_val (v_y_pixels_per_inch));
  check_cairo_status (v_cr);
  return Val_unit;
}
#else
Unsupported(ml_cairo_set_target_ps)
#endif /* CAIRO_HAS_PS_SURFACE */
ML_bc6(cairo_set_target_ps)

#ifdef CAIRO_HAS_PNG_SURFACE
CAMLprim value
ml_cairo_set_target_png (value v_cr, value v_file, value v_format,
			 value v_width, value v_height)
{
  cairo_set_target_png (cairo_t_val (v_cr), FILE_val (v_file),
			cairo_format_t_val (v_format),
			Int_val (v_width), Int_val (v_height));
  check_cairo_status (v_cr);
  return Val_unit;
}
#else
Unsupported(ml_cairo_set_target_png)
#endif /* CAIRO_HAS_PNG_SURFACE */

#define cairo_operator_t_val(v) ((cairo_operator_t) Int_val(v))
#define Val_cairo_operator_t(v) Val_int(v)

CAMLprim value
ml_cairo_set_operator (value v_cr, value v_op)
{
  cairo_set_operator (cairo_t_val (v_cr), cairo_operator_t_val (v_op));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_rgb_color (value v_cr, value v_red, value v_green, value v_blue)
{
  cairo_set_rgb_color (cairo_t_val (v_cr), Double_val (v_red),
		       Double_val (v_green), Double_val (v_blue));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_alpha (value v_cr, value v_alpha)
{
  cairo_set_alpha (cairo_t_val (v_cr), Double_val (v_alpha));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_pattern (value v_cr, value v_pattern)
{
  cairo_set_pattern (cairo_t_val (v_cr), cairo_pattern_t_val (v_pattern));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_tolerance (value v_cr, value v_tolerance)
{
  cairo_set_tolerance (cairo_t_val (v_cr), Double_val (v_tolerance));
  check_cairo_status (v_cr);
  return Val_unit;
}

#define cairo_fill_rule_t_val(v) ((cairo_fill_rule_t) Int_val(v))
#define Val_cairo_fill_rule_t(v) Val_int(v)

CAMLprim value
ml_cairo_set_fill_rule (value v_cr, value v_fill_rule)
{
  cairo_set_fill_rule (cairo_t_val (v_cr),
		       cairo_fill_rule_t_val (v_fill_rule));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_line_width (value v_cr, value v_width)
{
  cairo_set_line_width (cairo_t_val (v_cr), Double_val (v_width));
  check_cairo_status (v_cr);
  return Val_unit;
}

#define cairo_line_cap_t_val(v) ((cairo_line_cap_t) Int_val(v))
#define Val_cairo_line_cap_t(v) Val_int(v)

CAMLprim value
ml_cairo_set_line_cap (value v_cr, value v_line_cap)
{
  cairo_set_line_cap (cairo_t_val (v_cr), cairo_line_cap_t_val (v_line_cap));
  check_cairo_status (v_cr);
  return Val_unit;
}

#define cairo_line_join_t_val(v) ((cairo_line_join_t) Int_val(v))
#define Val_cairo_line_join_t(v) Val_int(v)

CAMLprim value
ml_cairo_set_line_join (value v_cr, value v_line_join)
{
  cairo_set_line_join (cairo_t_val (v_cr),
		       cairo_line_join_t_val (v_line_join));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_dash (value cr, value d, value off)
{
#ifndef ARCH_ALIGN_DOUBLE
  cairo_set_dash (cairo_t_val (cr), Double_array_val (d),
		  Double_array_length (d), Double_val (off));
#else
  int i, ndash = Double_array_length (d);
  double *dashes = stat_alloc (ndash * sizeof (double));
  for (i = 0; i < ndash, i++)
    dashes[i] = Double_field (d, i);
  cairo_set_dash (cairo_t_val (cr), dashes, ndash, Double_val (off));
  stat_free (dashes);
#endif
  check_cairo_status (cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_miter_limit (value v_cr, value v_limit)
{
  cairo_set_miter_limit (cairo_t_val (v_cr), Double_val (v_limit));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_translate (value v_cr, value v_tx, value v_ty)
{
  cairo_translate (cairo_t_val (v_cr), Double_val (v_tx), Double_val (v_ty));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_scale (value v_cr, value v_sx, value v_sy)
{
  cairo_scale (cairo_t_val (v_cr), Double_val (v_sx), Double_val (v_sy));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_rotate (value v_cr, value v_angle)
{
  cairo_rotate (cairo_t_val (v_cr), Double_val (v_angle));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_concat_matrix (value v_cr, value v_matrix)
{
  cairo_concat_matrix (cairo_t_val (v_cr), cairo_matrix_t_val (v_matrix));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_matrix (value v_cr, value v_matrix)
{
  cairo_set_matrix (cairo_t_val (v_cr), cairo_matrix_t_val (v_matrix));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_default_matrix (value v_cr)
{
  cairo_default_matrix (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_identity_matrix (value v_cr)
{
  cairo_identity_matrix (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_transform_point (value cr, value p)
{
  double x, y;
  x = Double_field (p, 0);
  y = Double_field (p, 1);
  cairo_transform_point (cairo_t_val (cr), &x, &y);
  check_cairo_status (cr);
  Store_double_field (p, 0, x);
  Store_double_field (p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_transform_distance (value cr, value p)
{
  double x, y;
  x = Double_field (p, 0);
  y = Double_field (p, 1);
  cairo_transform_distance (cairo_t_val (cr), &x, &y);
  check_cairo_status (cr);
  Store_double_field (p, 0, x);
  Store_double_field (p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_inverse_transform_point (value cr, value p)
{
  double x = Double_field (p, 0);
  double y = Double_field (p, 1);
  cairo_inverse_transform_point (cairo_t_val (cr), &x, &y);
  check_cairo_status (cr);
  Store_double_field (p, 0, x);
  Store_double_field (p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_inverse_transform_distance (value cr, value p)
{
  double x = Double_field (p, 0);
  double y = Double_field (p, 1);
  cairo_inverse_transform_distance (cairo_t_val (cr), &x, &y);
  check_cairo_status (cr);
  Store_double_field (p, 0, x);
  Store_double_field (p, 1, y);
  return Val_unit;
}


CAMLprim value
ml_cairo_new_path (value v_cr)
{
  cairo_new_path (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_move_to (value v_cr, value v_x, value v_y)
{
  cairo_move_to (cairo_t_val (v_cr), Double_val (v_x), Double_val (v_y));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_line_to (value v_cr, value v_x, value v_y)
{
  cairo_line_to (cairo_t_val (v_cr), Double_val (v_x), Double_val (v_y));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_curve_to (value v_cr, value v_x1, value v_y1, value v_x2, value v_y2,
		   value v_x3, value v_y3)
{
  cairo_curve_to (cairo_t_val (v_cr), Double_val (v_x1), Double_val (v_y1),
		  Double_val (v_x2), Double_val (v_y2), Double_val (v_x3),
		  Double_val (v_y3));
  check_cairo_status (v_cr);
  return Val_unit;
}
ML_bc7(cairo_curve_to)

CAMLprim value
ml_cairo_arc (value v_cr, value v_xc, value v_yc, value v_radius,
	      value v_angle1, value v_angle2)
{
  cairo_arc (cairo_t_val (v_cr), Double_val (v_xc), Double_val (v_yc),
	     Double_val (v_radius), Double_val (v_angle1),
	     Double_val (v_angle2));
  check_cairo_status (v_cr);
  return Val_unit;
}
ML_bc6(cairo_arc)

CAMLprim value
ml_cairo_arc_negative (value v_cr, value v_xc, value v_yc, value v_radius,
		       value v_angle1, value v_angle2)
{
  cairo_arc_negative (cairo_t_val (v_cr), Double_val (v_xc),
		      Double_val (v_yc), Double_val (v_radius),
		      Double_val (v_angle1), Double_val (v_angle2));
  check_cairo_status (v_cr);
  return Val_unit;
}
ML_bc6(cairo_arc_negative)

CAMLprim value
ml_cairo_rel_move_to (value v_cr, value v_dx, value v_dy)
{
  cairo_rel_move_to (cairo_t_val (v_cr), Double_val (v_dx),
		     Double_val (v_dy));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_rel_line_to (value v_cr, value v_dx, value v_dy)
{
  cairo_rel_line_to (cairo_t_val (v_cr), Double_val (v_dx),
		     Double_val (v_dy));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_rel_curve_to (value v_cr, value v_dx1, value v_dy1, value v_dx2,
		       value v_dy2, value v_dx3, value v_dy3)
{
  cairo_rel_curve_to (cairo_t_val (v_cr), Double_val (v_dx1),
		      Double_val (v_dy1), Double_val (v_dx2),
		      Double_val (v_dy2), Double_val (v_dx3),
		      Double_val (v_dy3));
  check_cairo_status (v_cr);
  return Val_unit;
}
ML_bc7(cairo_rel_curve_to)

CAMLprim value
ml_cairo_rectangle (value v_cr, value v_x, value v_y, value v_width,
		    value v_height)
{
  cairo_rectangle (cairo_t_val (v_cr), Double_val (v_x), Double_val (v_y),
		   Double_val (v_width), Double_val (v_height));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_close_path (value v_cr)
{
  cairo_close_path (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_stroke (value v_cr)
{
  cairo_stroke (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_fill (value v_cr)
{
  cairo_fill (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_copy_page (value v_cr)
{
  cairo_copy_page (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_show_page (value v_cr)
{
  cairo_show_page (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_in_stroke (value v_cr, value v_x, value v_y)
{
  int c_ret;
  c_ret =
    cairo_in_stroke (cairo_t_val (v_cr), Double_val (v_x), Double_val (v_y));
  check_cairo_status (v_cr);
  return Val_int (c_ret);
}

CAMLprim value
ml_cairo_in_fill (value v_cr, value v_x, value v_y)
{
  int c_ret;
  c_ret =
    cairo_in_fill (cairo_t_val (v_cr), Double_val (v_x), Double_val (v_y));
  check_cairo_status (v_cr);
  return Val_int (c_ret);
}

CAMLprim value
ml_cairo_stroke_extents (value v_cr)
{
  double x1, y1, x2, y2;
  cairo_stroke_extents (cairo_t_val (v_cr), &x1, &y1, &x2, &y2);
  check_cairo_status (v_cr);
  {
    CAMLparam0 ();
    CAMLlocal1 (t);
    t = alloc_tuple (4);
    Store_field (t, 0, copy_double (x1));
    Store_field (t, 1, copy_double (y1));
    Store_field (t, 2, copy_double (x2));
    Store_field (t, 3, copy_double (y2));
    CAMLreturn (t);
  }
}

CAMLprim value
ml_cairo_fill_extents (value v_cr)
{
  double x1, y1, x2, y2;
  cairo_fill_extents (cairo_t_val (v_cr), &x1, &y1, &x2, &y2);
  check_cairo_status (v_cr);
  {
    CAMLparam0 ();
    CAMLlocal1 (t);
    t = alloc_tuple (4);
    Store_field (t, 0, copy_double (x1));
    Store_field (t, 1, copy_double (y1));
    Store_field (t, 2, copy_double (x2));
    Store_field (t, 3, copy_double (y2));
    CAMLreturn (t);
  }
}

CAMLprim value
ml_cairo_clip (value v_cr)
{
  cairo_clip (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_init_clip (value v_cr)
{
  cairo_init_clip (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_unit;
}

Make_Val_final_pointer(cairo_font_t, Ignore, cairo_font_destroy, 20)
#define cairo_font_t_val(v) ((cairo_font_t *)Pointer_val(v))

static void
cairo_glyph_t_val (cairo_glyph_t * _s, value _v)
{
  _s->index = Int_val (Field (_v, 0));
  _s->x = Double_val (Field (_v, 1));
  _s->y = Double_val (Field (_v, 2));
}

static value
Val_cairo_font_extents_t (cairo_font_extents_t * _s)
{
  value _v;
  _v = alloc_small (5 * Double_wosize, Double_array_tag);
  Store_double_field (_v, 0, _s->ascent);
  Store_double_field (_v, 1, _s->descent);
  Store_double_field (_v, 2, _s->height);
  Store_double_field (_v, 3, _s->max_x_advance);
  Store_double_field (_v, 4, _s->max_y_advance);
  return _v;
}

static value
Val_cairo_text_extents_t (cairo_text_extents_t * _s)
{
  value _v;
  _v = alloc_small (6 * Double_wosize, Double_array_tag);
  Store_double_field (_v, 0, _s->x_bearing);
  Store_double_field (_v, 1, _s->y_bearing);
  Store_double_field (_v, 2, _s->width);
  Store_double_field (_v, 3, _s->height);
  Store_double_field (_v, 4, _s->x_advance);
  Store_double_field (_v, 5, _s->y_advance);
  return _v;
}

#define cairo_font_weight_t_val(v) ((cairo_font_weight_t) Int_val(v))
#define Val_cairo_font_weight_t(v) Val_int(v)

#define cairo_font_slant_t_val(v) ((cairo_font_slant_t) Int_val(v))
#define Val_cairo_font_slant_t(v) Val_int(v)

CAMLprim value
ml_cairo_select_font (value v_ct, value v_family, value v_slant,
		      value v_weight)
{
  cairo_select_font (cairo_t_val (v_ct), String_val (v_family),
		     cairo_font_slant_t_val (v_slant),
		     cairo_font_weight_t_val (v_weight));
  check_cairo_status (v_ct);
  return Val_unit;
}

CAMLprim value
ml_cairo_scale_font (value v_cr, value v_scale)
{
  cairo_scale_font (cairo_t_val (v_cr), Double_val (v_scale));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_transform_font (value v_cr, value v_matrix)
{
  cairo_transform_font (cairo_t_val (v_cr), cairo_matrix_t_val (v_matrix));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_show_text (value v_ct, value v_utf8)
{
  cairo_show_text (cairo_t_val (v_ct), String_val (v_utf8));
  check_cairo_status (v_ct);
  return Val_unit;
}

CAMLprim value
ml_cairo_show_glyphs (value v_ct, value v_glyphs)
{
  size_t num_glyphs = Wosize_val (v_glyphs);
  cairo_glyph_t c_glyphs[num_glyphs];
  unsigned int i;
  for (i = 0; i < num_glyphs; i++)
    cairo_glyph_t_val (&c_glyphs[i], Field (v_glyphs, i));
  cairo_show_glyphs (cairo_t_val (v_ct), c_glyphs, num_glyphs);
  check_cairo_status (v_ct);
  return Val_unit;
}

CAMLprim value
ml_cairo_current_font (value cr)
{
  cairo_font_t *f = cairo_current_font (cairo_t_val (cr));
  check_cairo_status (cr);
  return Val_cairo_font_t (f);
}

CAMLprim value
ml_cairo_current_font_extents (value cr)
{
  cairo_font_extents_t e;
  cairo_current_font_extents (cairo_t_val (cr), &e);
  check_cairo_status (cr);
  return Val_cairo_font_extents_t (&e);
}

CAMLprim value
ml_cairo_set_font (value v_ct, value v_font)
{
  cairo_set_font (cairo_t_val (v_ct), cairo_font_t_val (v_font));
  check_cairo_status (v_ct);
  return Val_unit;
}

CAMLprim value
ml_cairo_text_extents (value v_ct, value v_utf8)
{
  cairo_text_extents_t c_extents;
  cairo_text_extents (cairo_t_val (v_ct), String_val (v_utf8), &c_extents);
  check_cairo_status (v_ct);
  return Val_cairo_text_extents_t (&c_extents);
}

CAMLprim value
ml_cairo_glyph_extents (value v_ct, value v_glyphs)
{
  size_t num_glyphs = Wosize_val (v_glyphs);
  cairo_text_extents_t c_extents;
  cairo_glyph_t c_glyphs[num_glyphs];
  unsigned int i;
  for (i = 0; i < num_glyphs; i++)
    cairo_glyph_t_val (&c_glyphs[i], Field (v_glyphs, i));
  cairo_glyph_extents (cairo_t_val (v_ct), c_glyphs, num_glyphs, &c_extents);
  check_cairo_status (v_ct);
  return Val_cairo_text_extents_t (&c_extents);
}

CAMLprim value
ml_cairo_text_path (value v_ct, value v_utf8)
{
  cairo_text_path (cairo_t_val (v_ct), String_val (v_utf8));
  check_cairo_status (v_ct);
  return Val_unit;
}

CAMLprim value
ml_cairo_glyph_path (value v_ct, value v_glyphs)
{
  size_t num_glyphs = Wosize_val (v_glyphs);
  cairo_glyph_t c_glyphs[num_glyphs];
  unsigned int i;
  for (i = 0; i < num_glyphs; i++)
    cairo_glyph_t_val (&c_glyphs[i], Field (v_glyphs, i));
  cairo_glyph_path (cairo_t_val (v_ct), c_glyphs, num_glyphs);
  check_cairo_status (v_ct);
  return Val_unit;
}

ML_1(cairo_font_destroy, cairo_font_t_val, Unit)

ML_2(cairo_font_set_transform, cairo_font_t_val, cairo_matrix_t_val, Unit)
ML_2(cairo_font_current_transform, cairo_font_t_val, cairo_matrix_t_val, Unit)

CAMLprim value
ml_cairo_show_surface (value v_cr, value v_surface, value v_width,
		       value v_height)
{
  cairo_show_surface (cairo_t_val (v_cr), cairo_surface_t_val (v_surface),
		      Int_val (v_width), Int_val (v_height));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_current_operator (value v_cr)
{
  cairo_operator_t c_ret;
  c_ret = cairo_current_operator (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_cairo_operator_t (c_ret);
}

CAMLprim value
ml_cairo_current_rgb_color (value cr)
{
  CAMLparam1 (cr);
  CAMLlocal4 (v, vr, vg, vb);
  double r, g, b;
  cairo_current_rgb_color (cairo_t_val (cr), &r, &g, &b);
  check_cairo_status (cr);
  vr = copy_double (r);
  vg = copy_double (g);
  vb = copy_double (b);
  v = alloc_small (3, 0);
  Field (v, 0) = vr;
  Field (v, 1) = vg;
  Field (v, 2) = vb;
  CAMLreturn (v);
}

CAMLprim value
ml_cairo_current_pattern (value v_cr)
{
  cairo_pattern_t *c_ret;
  c_ret = cairo_current_pattern (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  cairo_pattern_reference (c_ret);
  return Val_cairo_pattern_t (c_ret);
}

CAMLprim value
ml_cairo_current_alpha (value v_cr)
{
  double c_ret;
  c_ret = cairo_current_alpha (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return copy_double (c_ret);
}

CAMLprim value
ml_cairo_current_tolerance (value v_cr)
{
  double c_ret;
  c_ret = cairo_current_tolerance (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return copy_double (c_ret);
}

CAMLprim value
ml_cairo_current_point (value cr)
{
  value v;
  double x, y;
  cairo_current_point (cairo_t_val (cr), &x, &y);
  check_cairo_status (cr);
  v = alloc_small (2 * Double_wosize, Double_array_tag);
  Store_double_field (v, 0, x);
  Store_double_field (v, 1, y);
  return v;
}

CAMLprim value
ml_cairo_current_fill_rule (value v_cr)
{
  cairo_fill_rule_t c_ret;
  c_ret = cairo_current_fill_rule (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_cairo_fill_rule_t (c_ret);
}

CAMLprim value
ml_cairo_current_line_width (value v_cr)
{
  double c_ret;
  c_ret = cairo_current_line_width (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return copy_double (c_ret);
}

CAMLprim value
ml_cairo_current_line_cap (value v_cr)
{
  cairo_line_cap_t c_ret;
  c_ret = cairo_current_line_cap (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_cairo_line_cap_t (c_ret);
}

CAMLprim value
ml_cairo_current_line_join (value v_cr)
{
  cairo_line_join_t c_ret;
  c_ret = cairo_current_line_join (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return Val_cairo_line_join_t (c_ret);
}

CAMLprim value
ml_cairo_current_miter_limit (value v_cr)
{
  double c_ret;
  c_ret = cairo_current_miter_limit (cairo_t_val (v_cr));
  check_cairo_status (v_cr);
  return copy_double (c_ret);
}

CAMLprim value
ml_cairo_current_matrix (value v_cr, value v_matrix)
{
  cairo_current_matrix (cairo_t_val (v_cr), cairo_matrix_t_val (v_matrix));
  check_cairo_status (v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_current_target_surface (value cr)
{
  cairo_surface_t *s = cairo_current_target_surface (cairo_t_val (cr));
  check_cairo_status (cr);
  cairo_surface_reference (s);
  return Val_cairo_surface_t (s);
}

ML_1(cairo_status, cairo_t_val, Val_cairo_status_t)
ML_1(cairo_status_string, cairo_t_val, copy_string)

CAMLprim value
ml_cairo_surface_create_for_image (value img)
{
  cairo_surface_t *s;
  s = cairo_surface_create_for_image (Bp_val (Field (img, 0)),
				      cairo_format_t_val (Field (img, 1)),
				      Int_val (Field (img, 2)),
				      Int_val (Field (img, 3)),
				      Int_val (Field (img, 4)));
  return Val_cairo_surface_t (s);
}
ML_4(cairo_surface_create_similar, cairo_surface_t_val, cairo_format_t_val, Int_val, Int_val, Val_cairo_surface_t)

ML_1(cairo_surface_destroy, cairo_surface_t_val, Unit)

ML_2(cairo_surface_set_repeat, cairo_surface_t_val, Int_val, Val_cairo_status_t)
ML_2(cairo_surface_set_matrix, cairo_surface_t_val, cairo_matrix_t_val, Val_cairo_status_t)
ML_2(cairo_surface_get_matrix, cairo_surface_t_val, cairo_matrix_t_val, Val_cairo_status_t)

#define cairo_filter_t_val(v) ((cairo_filter_t) Int_val(v))
#define Val_cairo_filter_t(v) Val_int(v)

ML_2(cairo_surface_set_filter, cairo_surface_t_val, cairo_filter_t_val, Val_cairo_status_t)

ML_3(cairo_image_surface_create, cairo_format_t_val, Int_val, Int_val, Val_cairo_surface_t)

CAMLprim value
ml_cairo_image_surface_create_for_data (value img)
{
  cairo_surface_t *s;
  s = cairo_image_surface_create_for_data (Bp_val (Field (img, 0)),
					   cairo_format_t_val (Field
							       (img, 1)),
					   Int_val (Field (img, 2)),
					   Int_val (Field (img, 3)),
					   Int_val (Field (img, 4)));
  return Val_cairo_surface_t (s);
}

#ifdef CAIRO_HAS_PS_SURFACE
ML_5(cairo_ps_surface_create, FILE_val, Double_val, Double_val, Double_val, Double_val, Val_cairo_surface_t)
#else
Unsupported(ml_cairo_ps_surface_create)
#endif /* CAIRO_HAS_PS_SURFACE */

#ifdef CAIRO_HAS_PNG_SURFACE
ML_4(cairo_png_surface_create, FILE_val, cairo_format_t_val, Double_val, Double_val, Val_cairo_surface_t)
#else
Unsupported(ml_cairo_png_surface_create)
#endif /* CAIRO_HAS_PNG_SURFACE */

ML_0(cairo_matrix_create, Val_cairo_matrix_t)
ML_1(cairo_matrix_destroy, cairo_matrix_t_val, Unit)
ML_2(cairo_matrix_copy, cairo_matrix_t_val, cairo_matrix_t_val, Val_cairo_status_t)
ML_1(cairo_matrix_set_identity, cairo_matrix_t_val, Val_cairo_status_t)
CAMLprim value
ml_cairo_matrix_set_affine (value m, value aff)
{
  if (Double_array_length (aff) != 6)
    invalid_argument ("not a matrix");

  cairo_matrix_set_affine (cairo_matrix_t_val (m),
			   Double_field (aff, 0),
			   Double_field (aff, 1),
			   Double_field (aff, 2),
			   Double_field (aff, 3),
			   Double_field (aff, 4), Double_field (aff, 5));
  return Val_unit;
}

CAMLprim value
ml_cairo_matrix_get_affine (value m)
{
  CAMLparam1 (m);
  CAMLlocal1 (v);
  double a, b, c, d, tx, ty;
  cairo_matrix_get_affine (cairo_matrix_t_val (m), &a, &b, &c, &d, &tx, &ty);
  v = alloc_small (6 * Double_wosize, Double_array_tag);
  Store_double_field (v, 0, a);
  Store_double_field (v, 1, b);
  Store_double_field (v, 2, c);
  Store_double_field (v, 3, d);
  Store_double_field (v, 4, tx);
  Store_double_field (v, 5, ty);
  CAMLreturn (v);
}

ML_3(cairo_matrix_translate, cairo_matrix_t_val, Double_val, Double_val, Val_cairo_status_t)
ML_3(cairo_matrix_scale, cairo_matrix_t_val, Double_val, Double_val, Val_cairo_status_t)
ML_2(cairo_matrix_rotate, cairo_matrix_t_val, Double_val, Val_cairo_status_t)
ML_1(cairo_matrix_invert, cairo_matrix_t_val, Val_cairo_status_t)
ML_3(cairo_matrix_multiply, cairo_matrix_t_val, cairo_matrix_t_val, cairo_matrix_t_val, Val_cairo_status_t)

CAMLprim value
ml_cairo_matrix_transform_distance (value m, value p)
{
  double x, y;
  x = Double_field (p, 0);
  y = Double_field (p, 1);
  cairo_matrix_transform_distance (cairo_matrix_t_val (m), &x, &y);
  Store_double_field (p, 0, x);
  Store_double_field (p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_matrix_transform_point (value m, value p)
{
  double x, y;
  x = Double_field (p, 0);
  y = Double_field (p, 1);
  cairo_matrix_transform_point (cairo_matrix_t_val (m), &x, &y);
  Store_double_field (p, 0, x);
  Store_double_field (p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_finalise_target (value cr)
{
  cairo_set_target_surface (cairo_t_val (cr), NULL);
  return Val_unit;
}

CAMLprim value
ml_cairo_surface_finalise (value s)
{
  cairo_surface_t *surf = cairo_surface_t_val (s);
  cairo_surface_destroy (surf);
  Store_pointer (s, NULL);
  return Val_unit;
}

ML_1 (cairo_pattern_create_for_surface, cairo_surface_t_val, Val_cairo_pattern_t)
ML_4 (cairo_pattern_create_linear, Double_val, Double_val, Double_val, Double_val, Val_cairo_pattern_t)
ML_6 (cairo_pattern_create_radial, Double_val, Double_val, Double_val, Double_val, Double_val, Double_val, Val_cairo_pattern_t)
ML_bc6 (cairo_pattern_create_radial)
ML_6 (cairo_pattern_add_color_stop, cairo_pattern_t_val, Double_val, Double_val, Double_val, Double_val, Double_val, Val_cairo_status_t)
ML_bc6 (cairo_pattern_add_color_stop)
ML_2(cairo_pattern_set_matrix, cairo_pattern_t_val, cairo_matrix_t_val, Val_cairo_status_t)
ML_2(cairo_pattern_get_matrix, cairo_pattern_t_val, cairo_matrix_t_val, Val_cairo_status_t)

#define cairo_extend_t_val(v) ((cairo_extend_t) Int_val(v))
#define Val_cairo_extend_t(v) Val_int(v)

ML_2 (cairo_pattern_set_extend, cairo_pattern_t_val, cairo_extend_t_val, Val_cairo_status_t)
ML_1 (cairo_pattern_get_extend, cairo_pattern_t_val, Val_cairo_extend_t)
ML_2 (cairo_pattern_set_filter, cairo_pattern_t_val, cairo_filter_t_val, Val_cairo_status_t)
ML_1 (cairo_pattern_get_filter, cairo_pattern_t_val, Val_cairo_filter_t)
