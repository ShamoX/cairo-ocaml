#include <cairo.h>
#include <caml/callback.h>
#include <caml/fail.h>

#include "ml_cairo.h"
#include "ml_cairo_status.h"

void
cairo_treat_status(cairo_status_t s)
{
  static value *cairo_exn;
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
    {
      cairo_exn = caml_named_value("cairo_status_exn");
      if (cairo_exn == NULL)
	failwith("cairo exception");
    }

  raise_with_arg (*cairo_exn, Val_int(status));
}

void
check_cairo_status (value cr)
{
  struct ml_cairo *ml_c = Data_custom_val(cr);
  if (! ml_c->suspend_exn) 
    {
      cairo_status_t status;
      status = cairo_status (ml_c->cr);
      if (status != CAIRO_STATUS_SUCCESS)
	cairo_treat_status (status);
    }
}
