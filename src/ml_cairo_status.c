/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include <cairo.h>
#include <caml/callback.h>
#include <caml/fail.h>

#include "ml_cairo.h"
#include "ml_cairo_status.h"

void
cairo_treat_status (cairo_status_t status)
{
  static value *cairo_exn;

  if (status != CAIRO_STATUS_SUCCESS)
    {
      if (cairo_exn == NULL)
	{
	  cairo_exn = caml_named_value ("cairo_status_exn");
	  if (cairo_exn == NULL)
	    failwith ("cairo exception");
	}
      raise_with_arg (*cairo_exn, Val_int (status - 1));
    }
}

void
check_cairo_status (value cr)
{
  struct ml_cairo *ml_c = Data_custom_val (cr);
  if (!ml_c->suspend_exn)
    cairo_treat_status (cairo_status (ml_c->cr));
}
