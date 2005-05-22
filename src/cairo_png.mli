(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

(** PNG reading/writing functions *)

external image_surface_create_from_file : 
  string -> Cairo.image_surface = "ml_cairo_image_surface_create_from_png"

external image_surface_create_from_stream : 
  (string -> unit) -> Cairo.image_surface = "ml_cairo_image_surface_create_from_stream"


external surface_write_to_file : 
  'a Cairo.surface -> string -> unit = "ml_cairo_surface_write_to_png"

external surface_write_to_stream : 
  'a Cairo.surface -> (string -> unit) -> unit = "ml_cairo_surface_write_to_png_stream"
