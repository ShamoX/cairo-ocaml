type gtkcairo = [Gtk.widget | `gtkcairo]
val init : unit

external get_cairo : [> `gtkcairo ] Gtk.obj -> Cairo.t
  = "ml_cairo_gtkcairo_get_cairo"

class cairo :
  ([> gtkcairo ] as 'a) Gtk.obj ->
  object
    inherit GObj.widget_full
    val obj : 'a Gtk.obj
    method event : GObj.event_ops
    method cairo : Ocairo.cairo
    method queue_draw : unit
  end

val cairo :
  ?width:int ->
  ?height:int ->
  ?packing:(GObj.widget -> unit) ->
  ?show:bool ->
  unit -> cairo
