/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include "ml_cairo.h"

CAMLprim value
ml_cairo_status (value v_cr)
{
  value v;
  cairo_status_t status = cairo_status (cairo_t_val (v_cr));

  if (status == CAIRO_STATUS_SUCCESS)
    v = Val_unit;
  else
    {
      v = caml_alloc_small (1, 0);
      Field (v, 0) = Val_int (status - 1);
    }
  return v;
}

wML_1(cairo_status_string, cairo_t_val, caml_copy_string)

void
ml_cairo_treat_status (cairo_status_t status)
{
  static value *cairo_exn;

  assert (status != CAIRO_STATUS_SUCCESS);

  if (status == CAIRO_STATUS_NO_MEMORY)
    caml_raise_out_of_memory ();

  if (cairo_exn == NULL)
    {
      cairo_exn = caml_named_value ("cairo_status_exn");
      if (cairo_exn == NULL)
	caml_failwith ("cairo exception");
    }
  caml_raise_with_arg (*cairo_exn, Val_int (status - 1));
}
