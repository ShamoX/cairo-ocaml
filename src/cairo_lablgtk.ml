(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

external image_of_pixbuf : GdkPixbuf.pixbuf -> Cairo.image = "cairo_lablgtk_of_pixbuf"
external shuffle_pixels  : GdkPixbuf.pixbuf -> unit = "cairo_lablgtk_shuffle_pixels"

external surface_create_for_drawable : 
    [> `drawable] Gobject.obj ->
    Cairo.format -> Cairo.surface = "cairo_lablgtk_surface_create_for_drawable"
external set_target_drawable :
    Cairo.t -> [> `drawable] Gobject.obj -> unit 
      = "cairo_lablgtk_set_target_drawable"

let create ?target () =
  let c = Cairo.create () in
  begin match target with
  | None -> ()
  | Some d -> set_target_drawable c d end ;
  c
