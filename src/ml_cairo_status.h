/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

void cairo_treat_status (cairo_status_t);
void check_cairo_status (value cr);

#define Val_cairo_status_t(s)  (cairo_treat_status(s), Val_unit)
#define report_null_pointer    cairo_treat_status(CAIRO_STATUS_NULL_POINTER)
