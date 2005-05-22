(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

type surface = [`Any|`PDF] Cairo.surface

external surface_create : 
  string -> 
  width_in_points:float -> 
  height_in_points:float -> surface = "ml_cairo_pdf_surface_create"

external surface_create_for_stream :
  (string -> unit) ->
  width_in_points:float -> 
  height_in_points:float -> surface = "ml_cairo_pdf_surface_create_for_stream"

external set_dpi :
  [> `PDF] Cairo.surface -> x_dpi:float -> y_dpi:float -> unit = "ml_cairo_pdf_surface_set_dpi"
