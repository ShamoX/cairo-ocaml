(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

type gtkcairo = [Gtk.widget | `gtkcairo]

external get_cairo : [> `gtkcairo ] Gtk.obj -> Cairo.t
  = "ml_cairo_gtkcairo_get_cairo"

class cairo_signals :
  [> gtkcairo ] Gtk.obj ->
  object
    inherit GObj.widget_signals
    method paint : callback:(Cairo.t -> unit) -> GtkSignal.id
  end

class cairo :
  ([> gtkcairo ] as 'a) Gtk.obj ->
  object
    inherit GObj.widget
    val obj : 'a Gtk.obj
    method connect : cairo_signals
    method event : GObj.event_ops
    method cairo : Cairo.t
    method queue_draw : unit
  end

val cairo :
  ?width:int ->
  ?height:int ->
  ?packing:(GObj.widget -> unit) ->
  ?show:bool ->
  unit -> cairo
