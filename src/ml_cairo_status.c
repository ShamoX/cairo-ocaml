#include <cairo.h>
#include <caml/callback.h>
#include <caml/fail.h>

void
cairo_treat_status(cairo_status_t s)
{
  static value *cairo_exn = NULL;
  int status;

  switch (s) {
    case CAIRO_STATUS_SUCCESS:
      return;
    case CAIRO_STATUS_NO_MEMORY:
      raise_out_of_memory();
    case CAIRO_STATUS_INVALID_RESTORE:
      status = 0; break;
    case CAIRO_STATUS_INVALID_POP_GROUP:
      status = 1; break;
    case CAIRO_STATUS_NO_CURRENT_POINT:
      status = 2; break;
    case CAIRO_STATUS_INVALID_MATRIX:
      status = 3; break;
    case CAIRO_STATUS_NO_TARGET_SURFACE:
      status = 4; break;
    case CAIRO_STATUS_NULL_POINTER:
      status = 5; break;
  }
  
  if (cairo_exn == NULL)
    cairo_exn = caml_named_value("cairo_status_exn");

  if (cairo_exn)
    raise_with_arg(*cairo_exn, Val_int(status));
  else
    failwith("cairo exception");
}
