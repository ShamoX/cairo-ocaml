
type gtkcairo = [Gtk.widget | `gtkcairo]

external gtkcairo_init : unit -> unit = "ml_cairo_gtkcairo_init"
let init = gtkcairo_init ()

external get_cairo : [>`gtkcairo] Gtk.obj -> Cairo.t = "ml_cairo_gtkcairo_get_cairo"

class cairo obj = 
  object
    inherit GObj.widget_full (obj : [> gtkcairo] Gtk.obj)

    method event = new GObj.event_ops obj
    method cairo = new Ocairo.cairo (get_cairo obj)
    method queue_draw =
      GtkBase.Widget.queue_draw obj
  end

let cairo =
  GtkBase.Widget.size_params []
    ~cont:(fun pl ?packing ?show () ->
      GObj.pack_return 
	(new cairo (GtkObject.make "GtkCairo" pl))
	~packing ~show)
