#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/custom.h>

#include <svg-cairo.h>

#define report_null_pointer failwith("null pointer")
#include "ml_cairo_wrappers.h"
#include "ml_cairo.h"

static value
ml_svg_cairo_status (svg_cairo_status_t s)
{
  static value *exn;
  if (s == SVG_CAIRO_STATUS_SUCCESS)
    return Val_unit;
  if (exn == NULL)
    {
      exn = caml_named_value ("svg_cairo_status_exn");
      if (exn == NULL)
	failwith ("Svg_cairo exception not registered");
    }
  raise_with_arg (*exn, Val_int (s - 1));
}

Make_Val_final_pointer (svg_cairo_t, Id, svg_cairo_destroy, 100)
#define svg_cairo_t_val(v) (svg_cairo_t *)Pointer_val(v)

CAMLprim value
ml_svg_cairo_create (value unit)
{
  svg_cairo_status_t status;
  svg_cairo_t *s;
  status = svg_cairo_create (&s);
  ml_svg_cairo_status (status);
  return Val_svg_cairo_t (s);
}

ML_2 (svg_cairo_parse, svg_cairo_t_val, String_val, ml_svg_cairo_status)

CAMLprim value
ml_svg_cairo_parse_buffer (value s, value b)
{
  return ml_svg_cairo_status (svg_cairo_parse_buffer (svg_cairo_t_val (s),
						      String_val (b),
						      string_length (b)));
}

ML_1 (svg_cairo_parse_chunk_begin, svg_cairo_t_val, ml_svg_cairo_status)
CAMLprim value
ml_svg_cairo_parse_chunk (value s, value b)
{
  return ml_svg_cairo_status (svg_cairo_parse_chunk (svg_cairo_t_val (s),
						     String_val (b),
						     string_length (b)));
}
ML_1 (svg_cairo_parse_chunk_end, svg_cairo_t_val, ml_svg_cairo_status)

ML_2 (svg_cairo_render, svg_cairo_t_val, cairo_t_val, ml_svg_cairo_status)
ML_3 (svg_cairo_set_viewport_dimension, svg_cairo_t_val, Unsigned_int_val, Unsigned_int_val, ml_svg_cairo_status)

CAMLprim value
ml_svg_cairo_get_size (value s)
{
  int w, h;
  value r;
  svg_cairo_get_size (svg_cairo_t_val (s), &w, &h);
  r = alloc_small (2, 0);
  Field (r, 0) = Val_int (w);
  Field (r, 1) = Val_int (h);
  return r;
}
