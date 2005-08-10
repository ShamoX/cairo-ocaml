(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

type surface = [`Any|`Xlib] Cairo.surface

external image_of_pixbuf : GdkPixbuf.pixbuf -> Cairo.image_surface = "ml_cairo_lablgtk_of_pixbuf"
external shuffle_pixels  : GdkPixbuf.pixbuf -> unit = "ml_cairo_lablgtk_shuffle_pixels"

external surface_create : [> `drawable] Gobject.obj -> surface = "ml_cairo_xlib_surface_create"
external surface_set_size : [> `Xlib] Cairo.surface -> int -> int -> unit = "ml_cairo_xlib_surface_set_size"
external surface_set_drawable :
  [> `Xlib] Cairo.surface -> 
  [> `drawable] Gobject.obj -> 
  int -> int -> unit = "ml_cairo_xlib_surface_set_drawable"
