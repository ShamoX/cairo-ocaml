/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include "ml_cairo.h"

wMake_Val_final_pointer(cairo_surface_t, cairo_surface_destroy, 0)

wML_4(cairo_surface_create_similar, \
     cairo_surface_t_val, cairo_format_t_val, \
     Int_val, Int_val, Val_cairo_surface_t)

/* surface_reference */
/* surface_destroy */

CAMLprim value
ml_cairo_surface_finish (value surf)
{
  cairo_status_t s;
  s = cairo_surface_finish (cairo_surface_t_val (surf));
  cairo_treat_status (s);
  return Val_unit;
}

static void ml_cairo_destroy_user_data (void *data)
{
  caml_remove_global_root (data);
  caml_stat_free (data);
}

void
ml_cairo_surface_set_user_data (cairo_surface_t *surf, const cairo_user_data_key_t *key, value *v)
{
  cairo_status_t s;
  s = cairo_surface_set_user_data (surf,
				   key, v,
				   ml_cairo_destroy_user_data);
  if (s != CAIRO_STATUS_SUCCESS) 
    {
      cairo_surface_destroy (surf);
      ml_cairo_destroy_user_data (v);
      cairo_treat_status (s);
    }
}

/* surface_get_user_data */

wML_3(cairo_surface_set_device_offset, cairo_surface_t_val, Double_val, Double_val, Unit)
