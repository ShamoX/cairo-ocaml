#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/custom.h>
#include <caml/callback.h>

#include "ml_cairo_wrappers.h"

#include <cairo.h>
#include "ml_cairo.h"
#include "ml_cairo_status.h"


#define CAML_MOVE_TO_TAG  0x95006a53L
#define CAML_LINE_TO_TAG  0x3f23e04dL
#define CAML_CLOSE_TAG    0x8ca29f31L
#define CAML_CURVE_TO_TAG 0x84e3bcd7L

struct caml_fold_info {
  value closure;
  value val;
};

static value make_point(double x, double y)
{
  value p;
  p = alloc_small(2 * Double_wosize, Double_array_tag);
  Store_double_field(p, 0, x);
  Store_double_field(p, 1, y);
  return p;
}

static void
ml_cairo_move_to_cb(void *data, double x, double y)
{
  struct caml_fold_info *p = data;
  CAMLparam0();
  CAMLlocal2(path_elem, point);
  if (! Is_exception_result(p->val)) {
    point = make_point(x, y);
    path_elem = alloc_small(2, 0);
    Field(path_elem, 0) = CAML_MOVE_TO_TAG;
    Field(path_elem, 1) = point;
    p->val = callback2_exn(p->closure, p->val, path_elem);
  }
  CAMLreturn0;
}

static void
ml_cairo_line_to_cb(void *data, double x, double y)
{
  struct caml_fold_info *p = data;
  CAMLparam0();
  CAMLlocal2(path_elem, point);
  if (! Is_exception_result(p->val)) {
    point = make_point(x, y);
    path_elem = alloc_small(2, 0);
    Field(path_elem, 0) = CAML_LINE_TO_TAG;
    Field(path_elem, 1) = point;
    p->val = callback2_exn(p->closure, p->val, path_elem);
  }
  CAMLreturn0;
}

static void
ml_cairo_curve_to_cb(void *data, 
		     double x1, double y1, 
		     double x2, double y2, 
		     double x3, double y3)
{
  struct caml_fold_info *p = data;
  CAMLparam0();
  CAMLlocal5(path_elem, tuple, point1, point2, point3);
  if (! Is_exception_result(p->val)) {
    point1 = make_point(x1, y1);
    point2 = make_point(x2, y2);
    point3 = make_point(x3, y3);
    tuple = alloc_small(3, 0);
    Field(tuple, 0) = point1;
    Field(tuple, 1) = point2;
    Field(tuple, 2) = point3;
    path_elem = alloc_small(2, 0);
    Field(path_elem, 0) = CAML_CURVE_TO_TAG;
    Field(path_elem, 1) = tuple;
    p->val = callback2_exn(p->closure, p->val, path_elem);
  }
  CAMLreturn0;
}

static void
ml_cairo_close_cb(void *data)
{
  struct caml_fold_info *p = data;
  if (! Is_exception_result(p->val)) {
    p->val = callback2_exn(p->closure, p->val, CAML_CLOSE_TAG);
  }
}

CAMLprim value
ml_cairo_current_path(value v_cr, value closure, value init)
{
  CAMLparam1(v_cr);
  CAMLlocalN(v_p, 2);
  struct caml_fold_info *p = (struct caml_fold_info *)v_p;
  p->closure = closure;
  p->val = init;
  cairo_current_path(cairo_t_val(v_cr),
		     ml_cairo_move_to_cb,
		     ml_cairo_line_to_cb,
		     ml_cairo_curve_to_cb,
		     ml_cairo_close_cb, p);
  check_cairo_status(v_cr);
  if (Is_exception_result(p->val)) {
    value ex = Extract_exception(p->val);
    mlraise(ex);
  }
  CAMLreturn(p->val);
}

CAMLprim value
ml_cairo_current_path_flat(value v_cr, value closure, value init)
{
  CAMLparam1(v_cr);
  CAMLlocalN(v_p, 2);
  struct caml_fold_info *p = (struct caml_fold_info *)v_p;
  p->closure = closure;
  p->val = init;
  cairo_current_path_flat(cairo_t_val(v_cr),
			  ml_cairo_move_to_cb,
			  ml_cairo_line_to_cb,
			  ml_cairo_close_cb, p);
  check_cairo_status(v_cr);
  if (Is_exception_result(p->val)) {
    value ex = Extract_exception(p->val);
    mlraise(ex);
  }
  CAMLreturn(p->val);
}
