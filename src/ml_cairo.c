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

Make_Val_final_pointer(cairo_t, Ignore, cairo_destroy, 50)
#define cairo_t_val(v) ((cairo_t *)Pointer_val(v))
Make_Val_final_pointer(cairo_surface_t, Ignore, cairo_surface_destroy, 20)
#define cairo_surface_t_val(v) ((cairo_surface_t *)Pointer_val(v))
Make_Val_final_pointer(cairo_matrix_t, Ignore, cairo_matrix_destroy, 20)
#define cairo_matrix_t_val(v) ((cairo_matrix_t *)Pointer_val(v))
ML_1(cairo_create, Ignore, Val_cairo_t)

CAMLprim value
ml_cairo_reference(value v_cr)
{
  cairo_reference(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}
ML_1(cairo_destroy, cairo_t_val, Unit)

CAMLprim value
ml_cairo_save(value v_cr)
{
  cairo_save(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_restore(value v_cr)
{
  cairo_restore(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}
ML_2(cairo_copy, cairo_t_val, cairo_t_val, Unit)

CAMLprim value
ml_cairo_set_target_surface(value v_cr, value v_surface)
{
  cairo_set_target_surface(cairo_t_val(v_cr), cairo_surface_t_val(v_surface));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_target_image(value cr, value img)
{
  cairo_set_target_image(cairo_t_val(cr),
			 Bp_val(Field(img, 0)),
			 cairo_format_t_val(Field(img, 1)),
			 Int_val(Field(img, 2)),
			 Int_val(Field(img, 3)), Int_val(Field(img, 4)));
  check_cairo_status(cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_target_ps(value v_cr, value v_file, value v_width_inches,
		       value v_height_inches, value v_x_pixels_per_inch,
		       value v_y_pixels_per_inch)
{
  cairo_set_target_ps(cairo_t_val(v_cr), FILE_val(v_file),
		      Double_val(v_width_inches), Double_val(v_height_inches),
		      Double_val(v_x_pixels_per_inch),
		      Double_val(v_y_pixels_per_inch));
  check_cairo_status(v_cr);
  return Val_unit;
}

static inline cairo_operator_t
cairo_operator_t_val(value _v)
{
  const cairo_operator_t _conv_tab[] =
    { CAIRO_OPERATOR_CLEAR, CAIRO_OPERATOR_SRC, CAIRO_OPERATOR_DST,
CAIRO_OPERATOR_OVER, CAIRO_OPERATOR_OVER_REVERSE, CAIRO_OPERATOR_IN, CAIRO_OPERATOR_IN_REVERSE,
CAIRO_OPERATOR_OUT, CAIRO_OPERATOR_OUT_REVERSE, CAIRO_OPERATOR_ATOP, CAIRO_OPERATOR_ATOP_REVERSE,
CAIRO_OPERATOR_XOR, CAIRO_OPERATOR_ADD, CAIRO_OPERATOR_SATURATE, };
  return _conv_tab[Int_val(_v)];
}

static inline value
Val_cairo_operator_t(cairo_operator_t _s)
{
  switch (_s)
    {
    case CAIRO_OPERATOR_CLEAR:
      return Val_int(0);
    case CAIRO_OPERATOR_SRC:
      return Val_int(1);
    case CAIRO_OPERATOR_DST:
      return Val_int(2);
    case CAIRO_OPERATOR_OVER:
      return Val_int(3);
    case CAIRO_OPERATOR_OVER_REVERSE:
      return Val_int(4);
    case CAIRO_OPERATOR_IN:
      return Val_int(5);
    case CAIRO_OPERATOR_IN_REVERSE:
      return Val_int(6);
    case CAIRO_OPERATOR_OUT:
      return Val_int(7);
    case CAIRO_OPERATOR_OUT_REVERSE:
      return Val_int(8);
    case CAIRO_OPERATOR_ATOP:
      return Val_int(9);
    case CAIRO_OPERATOR_ATOP_REVERSE:
      return Val_int(10);
    case CAIRO_OPERATOR_XOR:
      return Val_int(11);
    case CAIRO_OPERATOR_ADD:
      return Val_int(12);
    case CAIRO_OPERATOR_SATURATE:
      return Val_int(13);
    default:
      failwith("cairo_operator_t: bad enum value");
    }
}

CAMLprim value
ml_cairo_set_operator(value v_cr, value v_op)
{
  cairo_set_operator(cairo_t_val(v_cr), cairo_operator_t_val(v_op));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_rgb_color(value v_cr, value v_red, value v_green, value v_blue)
{
  cairo_set_rgb_color(cairo_t_val(v_cr), Double_val(v_red),
		      Double_val(v_green), Double_val(v_blue));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_alpha(value v_cr, value v_alpha)
{
  cairo_set_alpha(cairo_t_val(v_cr), Double_val(v_alpha));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_pattern(value v_cr, value v_pattern)
{
  cairo_set_pattern(cairo_t_val(v_cr), cairo_surface_t_val(v_pattern));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_tolerance(value v_cr, value v_tolerance)
{
  cairo_set_tolerance(cairo_t_val(v_cr), Double_val(v_tolerance));
  check_cairo_status(v_cr);
  return Val_unit;
}

static inline cairo_fill_rule_t
cairo_fill_rule_t_val(value _v)
{
  const cairo_fill_rule_t _conv_tab[] =
    { CAIRO_FILL_RULE_WINDING, CAIRO_FILL_RULE_EVEN_ODD, };
  return _conv_tab[Int_val(_v)];
}

static inline value
Val_cairo_fill_rule_t(cairo_fill_rule_t _s)
{
  switch (_s)
    {
    case CAIRO_FILL_RULE_WINDING:
      return Val_int(0);
    case CAIRO_FILL_RULE_EVEN_ODD:
      return Val_int(1);
    default:
      failwith("cairo_fill_rule_t: bad enum value");
    }
}

CAMLprim value
ml_cairo_set_fill_rule(value v_cr, value v_fill_rule)
{
  cairo_set_fill_rule(cairo_t_val(v_cr), cairo_fill_rule_t_val(v_fill_rule));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_line_width(value v_cr, value v_width)
{
  cairo_set_line_width(cairo_t_val(v_cr), Double_val(v_width));
  check_cairo_status(v_cr);
  return Val_unit;
}

static inline cairo_line_cap_t
cairo_line_cap_t_val(value _v)
{
  const cairo_line_cap_t _conv_tab[] =
    { CAIRO_LINE_CAP_BUTT, CAIRO_LINE_CAP_ROUND, CAIRO_LINE_CAP_SQUARE, };
  return _conv_tab[Int_val(_v)];
}

static inline value
Val_cairo_line_cap_t(cairo_line_cap_t _s)
{
  switch (_s)
    {
    case CAIRO_LINE_CAP_BUTT:
      return Val_int(0);
    case CAIRO_LINE_CAP_ROUND:
      return Val_int(1);
    case CAIRO_LINE_CAP_SQUARE:
      return Val_int(2);
    default:
      failwith("cairo_line_cap_t: bad enum value");
    }
}

CAMLprim value
ml_cairo_set_line_cap(value v_cr, value v_line_cap)
{
  cairo_set_line_cap(cairo_t_val(v_cr), cairo_line_cap_t_val(v_line_cap));
  check_cairo_status(v_cr);
  return Val_unit;
}

static inline cairo_line_join_t
cairo_line_join_t_val(value _v)
{
  const cairo_line_join_t _conv_tab[] =
    { CAIRO_LINE_JOIN_MITER, CAIRO_LINE_JOIN_ROUND, CAIRO_LINE_JOIN_BEVEL, };
  return _conv_tab[Int_val(_v)];
}

static inline value
Val_cairo_line_join_t(cairo_line_join_t _s)
{
  switch (_s)
    {
    case CAIRO_LINE_JOIN_MITER:
      return Val_int(0);
    case CAIRO_LINE_JOIN_ROUND:
      return Val_int(1);
    case CAIRO_LINE_JOIN_BEVEL:
      return Val_int(2);
    default:
      failwith("cairo_line_join_t: bad enum value");
    }
}

CAMLprim value
ml_cairo_set_line_join(value v_cr, value v_line_join)
{
  cairo_set_line_join(cairo_t_val(v_cr), cairo_line_join_t_val(v_line_join));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_dash(value cr, value d, value off)
{
  cairo_set_dash(cairo_t_val(cr), Double_array_val(d),
		 Double_array_length(d), Double_val(off));
  return Val_unit;
}

CAMLprim value
ml_cairo_set_miter_limit(value v_cr, value v_limit)
{
  cairo_set_miter_limit(cairo_t_val(v_cr), Double_val(v_limit));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_translate(value v_cr, value v_tx, value v_ty)
{
  cairo_translate(cairo_t_val(v_cr), Double_val(v_tx), Double_val(v_ty));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_scale(value v_cr, value v_sx, value v_sy)
{
  cairo_scale(cairo_t_val(v_cr), Double_val(v_sx), Double_val(v_sy));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_rotate(value v_cr, value v_angle)
{
  cairo_rotate(cairo_t_val(v_cr), Double_val(v_angle));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_concat_matrix(value v_cr, value v_matrix)
{
  cairo_concat_matrix(cairo_t_val(v_cr), cairo_matrix_t_val(v_matrix));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_set_matrix(value v_cr, value v_matrix)
{
  cairo_set_matrix(cairo_t_val(v_cr), cairo_matrix_t_val(v_matrix));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_default_matrix(value v_cr)
{
  cairo_default_matrix(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_identity_matrix(value v_cr)
{
  cairo_identity_matrix(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_transform_point(value cr, value p)
{
  double x, y;
  x = Double_field(p, 0);
  y = Double_field(p, 1);
  cairo_transform_point(cairo_t_val(cr), &x, &y);
  check_cairo_status(cr);
  Store_double_field(p, 0, x);
  Store_double_field(p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_transform_distance(value cr, value p)
{
  double x, y;
  x = Double_field(p, 0);
  y = Double_field(p, 1);
  cairo_transform_distance(cairo_t_val(cr), &x, &y);
  check_cairo_status(cr);
  Store_double_field(p, 0, x);
  Store_double_field(p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_inverse_transform_point(value cr, value p)
{
  double x = Double_field(p, 0);
  double y = Double_field(p, 1);
  cairo_inverse_transform_point(cairo_t_val(cr), &x, &y);
  check_cairo_status(cr);
  Store_double_field(p, 0, x);
  Store_double_field(p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_inverse_transform_distance(value cr, value p)
{
  double x = Double_field(p, 0);
  double y = Double_field(p, 1);
  cairo_inverse_transform_distance(cairo_t_val(cr), &x, &y);
  check_cairo_status(cr);
  Store_double_field(p, 0, x);
  Store_double_field(p, 1, y);
  return Val_unit;
}


CAMLprim value
ml_cairo_new_path(value v_cr)
{
  cairo_new_path(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_move_to(value v_cr, value v_x, value v_y)
{
  cairo_move_to(cairo_t_val(v_cr), Double_val(v_x), Double_val(v_y));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_line_to(value v_cr, value v_x, value v_y)
{
  cairo_line_to(cairo_t_val(v_cr), Double_val(v_x), Double_val(v_y));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_curve_to(value v_cr, value v_x1, value v_y1, value v_x2, value v_y2,
		  value v_x3, value v_y3)
{
  cairo_curve_to(cairo_t_val(v_cr), Double_val(v_x1), Double_val(v_y1),
		 Double_val(v_x2), Double_val(v_y2), Double_val(v_x3),
		 Double_val(v_y3));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_arc(value v_cr, value v_xc, value v_yc, value v_radius,
	     value v_angle1, value v_angle2)
{
  cairo_arc(cairo_t_val(v_cr), Double_val(v_xc), Double_val(v_yc),
	    Double_val(v_radius), Double_val(v_angle1), Double_val(v_angle2));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_arc_negative(value v_cr, value v_xc, value v_yc, value v_radius,
		      value v_angle1, value v_angle2)
{
  cairo_arc_negative(cairo_t_val(v_cr), Double_val(v_xc), Double_val(v_yc),
		     Double_val(v_radius), Double_val(v_angle1),
		     Double_val(v_angle2));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_rel_move_to(value v_cr, value v_dx, value v_dy)
{
  cairo_rel_move_to(cairo_t_val(v_cr), Double_val(v_dx), Double_val(v_dy));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_rel_line_to(value v_cr, value v_dx, value v_dy)
{
  cairo_rel_line_to(cairo_t_val(v_cr), Double_val(v_dx), Double_val(v_dy));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_rel_curve_to(value v_cr, value v_dx1, value v_dy1, value v_dx2,
		      value v_dy2, value v_dx3, value v_dy3)
{
  cairo_rel_curve_to(cairo_t_val(v_cr), Double_val(v_dx1), Double_val(v_dy1),
		     Double_val(v_dx2), Double_val(v_dy2), Double_val(v_dx3),
		     Double_val(v_dy3));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_rectangle(value v_cr, value v_x, value v_y, value v_width,
		   value v_height)
{
  cairo_rectangle(cairo_t_val(v_cr), Double_val(v_x), Double_val(v_y),
		  Double_val(v_width), Double_val(v_height));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_close_path(value v_cr)
{
  cairo_close_path(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_stroke(value v_cr)
{
  cairo_stroke(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_fill(value v_cr)
{
  cairo_fill(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_copy_page(value v_cr)
{
  cairo_copy_page(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_show_page(value v_cr)
{
  cairo_show_page(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_in_stroke(value v_cr, value v_x, value v_y)
{
  int c_ret;
  c_ret =
    cairo_in_stroke(cairo_t_val(v_cr), Double_val(v_x), Double_val(v_y));
  check_cairo_status(v_cr);
  return Val_int(c_ret);
}

CAMLprim value
ml_cairo_in_fill(value v_cr, value v_x, value v_y)
{
  int c_ret;
  c_ret = cairo_in_fill(cairo_t_val(v_cr), Double_val(v_x), Double_val(v_y));
  check_cairo_status(v_cr);
  return Val_int(c_ret);
}

CAMLprim value
ml_cairo_clip(value v_cr)
{
  cairo_clip(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_unit;
}
Make_Val_final_pointer(cairo_font_t, Ignore, cairo_font_destroy, 20)

#define cairo_font_t_val(v) ((cairo_font_t *)Pointer_val(v))

static void
cairo_glyph_t_val(cairo_glyph_t * _s, value _v)
{
  _s->index = Int_val(Field(_v, 0));
  _s->x = Double_val(Field(_v, 1));
  _s->y = Double_val(Field(_v, 2));
}

static void
cairo_font_extents_t_val(cairo_font_extents_t * _s, value _v)
{
  _s->ascent = Double_field(_v, 0);
  _s->descent = Double_field(_v, 1);
  _s->height = Double_field(_v, 2);
  _s->max_x_advance = Double_field(_v, 3);
  _s->max_y_advance = Double_field(_v, 4);
}

static value
Val_cairo_font_extents_t(cairo_font_extents_t * _s)
{
  value _v;
  _v = alloc_small(5 * Double_wosize, Double_array_tag);
  Store_double_field(_v, 0, _s->ascent);
  Store_double_field(_v, 1, _s->descent);
  Store_double_field(_v, 2, _s->height);
  Store_double_field(_v, 3, _s->max_x_advance);
  Store_double_field(_v, 4, _s->max_y_advance);
  return _v;
}

static inline cairo_font_weight_t
cairo_font_weight_t_val(value _v)
{
  const cairo_font_weight_t _conv_tab[] =
    { CAIRO_FONT_WEIGHT_NORMAL, CAIRO_FONT_WEIGHT_BOLD, };
  return _conv_tab[Int_val(_v)];
}

static inline cairo_font_slant_t
cairo_font_slant_t_val(value _v)
{
  const cairo_font_slant_t _conv_tab[] =
    { CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_SLANT_ITALIC,
CAIRO_FONT_SLANT_OBLIQUE, };
  return _conv_tab[Int_val(_v)];
}

CAMLprim value
ml_cairo_select_font(value v_ct, value v_family, value v_slant,
		     value v_weight)
{
  cairo_select_font(cairo_t_val(v_ct), String_val(v_family),
		    cairo_font_slant_t_val(v_slant),
		    cairo_font_weight_t_val(v_weight));
  check_cairo_status(v_ct);
  return Val_unit;
}

CAMLprim value
ml_cairo_scale_font(value v_cr, value v_scale)
{
  cairo_scale_font(cairo_t_val(v_cr), Double_val(v_scale));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_transform_font(value v_cr, value v_matrix)
{
  cairo_transform_font(cairo_t_val(v_cr), cairo_matrix_t_val(v_matrix));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_show_text(value v_ct, value v_utf8)
{
  cairo_show_text(cairo_t_val(v_ct), String_val(v_utf8));
  check_cairo_status(v_ct);
  return Val_unit;
}

CAMLprim value
ml_cairo_show_glyphs(value v_ct, value v_glyphs, value v_num_glyphs)
{
  cairo_glyph_t c_glyphs;
  cairo_glyph_t_val(&c_glyphs, v_glyphs);
  cairo_show_glyphs(cairo_t_val(v_ct), &c_glyphs, Int_val(v_num_glyphs));
  check_cairo_status(v_ct);
  return Val_unit;
}

CAMLprim value
ml_cairo_current_font(value cr)
{
  cairo_font_t *f = cairo_current_font(cairo_t_val(cr));
  check_cairo_status(cr);
  cairo_font_reference(f);
  return Val_cairo_font_t(f);
}

CAMLprim value
ml_cairo_current_font_extents(value cr)
{
  cairo_font_extents_t e;
  cairo_current_font_extents(cairo_t_val(cr), &e);
  check_cairo_status(cr);
  return Val_cairo_font_extents_t(&e);
}

CAMLprim value
ml_cairo_set_font(value v_ct, value v_font)
{
  cairo_set_font(cairo_t_val(v_ct), cairo_font_t_val(v_font));
  check_cairo_status(v_ct);
  return Val_unit;
}
ML_1(cairo_font_reference, cairo_font_t_val, Unit)

ML_1(cairo_font_destroy, cairo_font_t_val, Unit)
ML_2(cairo_font_set_transform, cairo_font_t_val, cairo_matrix_t_val, Unit)
ML_2(cairo_font_current_transform, cairo_font_t_val, cairo_matrix_t_val, Unit)

CAMLprim value
ml_cairo_show_surface(value v_cr, value v_surface, value v_width,
		      value v_height)
{
  cairo_show_surface(cairo_t_val(v_cr), cairo_surface_t_val(v_surface),
		     Int_val(v_width), Int_val(v_height));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_current_operator(value v_cr)
{
  cairo_operator_t c_ret;
  c_ret = cairo_current_operator(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_cairo_operator_t(c_ret);
}

CAMLprim value
ml_cairo_current_rgb_color(value cr)
{
  CAMLparam1(cr);
  CAMLlocal4(v, vr, vg, vb);
  double r, g, b;
  cairo_current_rgb_color(cairo_t_val(cr), &r, &g, &b);
  check_cairo_status(cr);
  vr = copy_double(r);
  vg = copy_double(g);
  vb = copy_double(b);
  v = alloc_small(3, 0);
  Field(v, 0) = vr;
  Field(v, 1) = vg;
  Field(v, 2) = vb;
  CAMLreturn(v);
}

CAMLprim value
ml_cairo_current_alpha(value v_cr)
{
  double c_ret;
  c_ret = cairo_current_alpha(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return copy_double(c_ret);
}

CAMLprim value
ml_cairo_current_tolerance(value v_cr)
{
  double c_ret;
  c_ret = cairo_current_tolerance(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return copy_double(c_ret);
}

CAMLprim value
ml_cairo_current_point(value cr)
{
  value v;
  double x, y;
  cairo_current_point(cairo_t_val(cr), &x, &y);
  check_cairo_status(cr);
  v = alloc_small(2 * Double_wosize, 0);
  Store_double_field(v, 0, x);
  Store_double_field(v, 1, y);
  return v;
}

CAMLprim value
ml_cairo_current_fill_rule(value v_cr)
{
  cairo_fill_rule_t c_ret;
  c_ret = cairo_current_fill_rule(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_cairo_fill_rule_t(c_ret);
}

CAMLprim value
ml_cairo_current_line_width(value v_cr)
{
  double c_ret;
  c_ret = cairo_current_line_width(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return copy_double(c_ret);
}

CAMLprim value
ml_cairo_current_line_cap(value v_cr)
{
  cairo_line_cap_t c_ret;
  c_ret = cairo_current_line_cap(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_cairo_line_cap_t(c_ret);
}

CAMLprim value
ml_cairo_current_line_join(value v_cr)
{
  cairo_line_join_t c_ret;
  c_ret = cairo_current_line_join(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return Val_cairo_line_join_t(c_ret);
}

CAMLprim value
ml_cairo_current_miter_limit(value v_cr)
{
  double c_ret;
  c_ret = cairo_current_miter_limit(cairo_t_val(v_cr));
  check_cairo_status(v_cr);
  return copy_double(c_ret);
}

CAMLprim value
ml_cairo_current_matrix(value v_cr, value v_matrix)
{
  cairo_current_matrix(cairo_t_val(v_cr), cairo_matrix_t_val(v_matrix));
  check_cairo_status(v_cr);
  return Val_unit;
}

CAMLprim value
ml_cairo_current_target_surface(value cr)
{
  cairo_surface_t *s = cairo_current_target_surface(cairo_t_val(cr));
  check_cairo_status(cr);
  cairo_surface_reference(s);
  return Val_cairo_surface_t(s);
}
ML_1(cairo_status, cairo_t_val, Val_cairo_status_t)

ML_1(cairo_status_string, cairo_t_val, copy_string)
CAMLprim value
ml_cairo_surface_create_for_image(value img)
{
  cairo_surface_t *s;
  s = cairo_surface_create_for_image(Bp_val(Field(img, 0)),
				     cairo_format_t_val(Field(img, 1)),
				     Int_val(Field(img, 2)),
				     Int_val(Field(img, 3)),
				     Int_val(Field(img, 4)));
  return Val_cairo_surface_t(s);
}
ML_4(cairo_surface_create_similar, cairo_surface_t_val, cairo_format_t_val, Int_val, Int_val, Val_cairo_surface_t)

ML_1(cairo_surface_reference, cairo_surface_t_val, Unit)
ML_1(cairo_surface_destroy, cairo_surface_t_val, Unit)
ML_2(cairo_surface_set_repeat, cairo_surface_t_val, Int_val, Val_cairo_status_t)
ML_2(cairo_surface_set_matrix, cairo_surface_t_val, cairo_matrix_t_val, Val_cairo_status_t)
ML_2(cairo_surface_get_matrix, cairo_surface_t_val, cairo_matrix_t_val, Val_cairo_status_t)

static inline cairo_filter_t
cairo_filter_t_val(value _v)
{
  const cairo_filter_t _conv_tab[] =
    { CAIRO_FILTER_FAST, CAIRO_FILTER_GOOD, CAIRO_FILTER_BEST,
CAIRO_FILTER_NEAREST, CAIRO_FILTER_BILINEAR, };
  return _conv_tab[Int_val(_v)];
}
ML_2(cairo_surface_set_filter, cairo_surface_t_val, cairo_filter_t_val, Val_cairo_status_t)

ML_3(cairo_image_surface_create, cairo_format_t_val, Int_val, Int_val, Val_cairo_surface_t)
CAMLprim value
ml_cairo_image_surface_create_for_data(value img)
{
  cairo_surface_t *s;
  s = cairo_image_surface_create_for_data(Bp_val(Field(img, 0)),
					  cairo_format_t_val(Field(img, 1)),
					  Int_val(Field(img, 2)),
					  Int_val(Field(img, 3)),
					  Int_val(Field(img, 4)));
  return Val_cairo_surface_t(s);
}
ML_5(cairo_ps_surface_create, FILE_val, Double_val, Double_val, Double_val, Double_val, Val_cairo_surface_t)

ML_1(cairo_matrix_create, Ignore, Val_cairo_matrix_t)
ML_1(cairo_matrix_destroy, cairo_matrix_t_val, Unit)
ML_2(cairo_matrix_copy, cairo_matrix_t_val, cairo_matrix_t_val, Val_cairo_status_t)
ML_1(cairo_matrix_set_identity, cairo_matrix_t_val, Val_cairo_status_t)
CAMLprim value
ml_cairo_matrix_set_affine(value m, value aff)
{
  if (Double_array_length(aff) != 6)
    invalid_argument("not a matrix");

  cairo_matrix_set_affine(cairo_matrix_t_val(m),
			  Double_field(aff, 0),
			  Double_field(aff, 1),
			  Double_field(aff, 2),
			  Double_field(aff, 3),
			  Double_field(aff, 4), Double_field(aff, 5));
  return Val_unit;
}

CAMLprim value
ml_cairo_matrix_get_affine(value m)
{
  CAMLparam1(m);
  CAMLlocal1(v);
  double a, b, c, d, tx, ty;
  cairo_matrix_get_affine(cairo_matrix_t_val(m), &a, &b, &c, &d, &tx, &ty);
  v = alloc_small(6 * Double_wosize, 0);
  Store_double_field(v, 0, a);
  Store_double_field(v, 1, b);
  Store_double_field(v, 2, c);
  Store_double_field(v, 3, d);
  Store_double_field(v, 4, tx);
  Store_double_field(v, 5, ty);
  CAMLreturn(v);
}
ML_3(cairo_matrix_translate, cairo_matrix_t_val, Double_val, Double_val, Val_cairo_status_t)

ML_3(cairo_matrix_scale, cairo_matrix_t_val, Double_val, Double_val, Val_cairo_status_t)
ML_2(cairo_matrix_rotate, cairo_matrix_t_val, Double_val, Val_cairo_status_t)
ML_1(cairo_matrix_invert, cairo_matrix_t_val, Val_cairo_status_t)
ML_3(cairo_matrix_multiply, cairo_matrix_t_val, cairo_matrix_t_val, cairo_matrix_t_val, Val_cairo_status_t)
CAMLprim value
ml_cairo_matrix_transform_distance(value m, value p)
{
  double x, y;
  x = Double_field(p, 0);
  y = Double_field(p, 1);
  cairo_matrix_transform_distance(cairo_matrix_t_val(m), &x, &y);
  Store_double_field(p, 0, x);
  Store_double_field(p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_matrix_transform_point(value m, value p)
{
  double x, y;
  x = Double_field(p, 0);
  y = Double_field(p, 1);
  cairo_matrix_transform_point(cairo_matrix_t_val(m), &x, &y);
  Store_double_field(p, 0, x);
  Store_double_field(p, 1, y);
  return Val_unit;
}

CAMLprim value
ml_cairo_finalise_target_ps(value cr)
{
  cairo_set_target_surface(cairo_t_val(cr), NULL);
  return Val_unit;
}

CAMLprim value
ml_cairo_ps_surface_finalise(value s)
{
  cairo_surface_t *surf = cairo_surface_t_val(s);
  cairo_surface_destroy(surf);
  cairo_surface_t_val(s) = NULL;
  return Val_unit;
}
