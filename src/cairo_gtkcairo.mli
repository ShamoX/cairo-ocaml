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
