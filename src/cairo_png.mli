(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

(** PNG reading/writing functions *)

val image_surface_create_from_channel : 
  in_channel -> Cairo.image_surface

val image_surface_create_from_file : 
  string -> Cairo.image_surface

external image_surface_create_from_stream : 
  (string -> unit) -> Cairo.image_surface = "ml_cairo_image_surface_create_from_png_stream"


val surface_write_to_channel : 
  'a Cairo.surface -> out_channel -> unit

val surface_write_to_file : 
  'a Cairo.surface -> string -> unit

external surface_write_to_stream : 
  'a Cairo.surface -> (string -> unit) -> unit = "ml_cairo_surface_write_to_png_stream"
