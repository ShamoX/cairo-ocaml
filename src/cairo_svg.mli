(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

(** SVG backend *)

type surface = [`Any|`SVG] Cairo.surface

val surface_create_for_channel : 
  out_channel -> 
  width_in_points:float -> 
  height_in_points:float -> surface

external surface_create_for_stream :
  (string -> unit) ->
  width_in_points:float -> 
  height_in_points:float -> surface = "ml_cairo_svg_surface_create_for_stream"

external set_dpi :
  [> `SVG] Cairo.surface -> x_dpi:float -> y_dpi:float -> unit = "ml_cairo_svg_surface_set_dpi"
