
type gtkcairo = [Gtk.widget | `gtkcairo]

external gtkcairo_init : unit -> unit = "ml_cairo_gtkcairo_init"
let init = gtkcairo_init ()

external get_cairo : [>`gtkcairo] Gtk.obj -> Cairo.t = "ml_cairo_gtkcairo_get_cairo"

let redraw_sig = {
  GtkSignal.name       = "redraw" ;
  GtkSignal.classe     = `gtkcairo ;
  GtkSignal.marshaller = 
  let m = GtkSignal.marshal1 Gobject.Data.pointer "GtkCairo::redraw" in
  fun cb ->
    m (function 
      | None -> raise Gpointer.Null 
      | Some cairo -> cb (Obj.magic cairo : Cairo.t)) }

class cairo_signals obj = 
  object (self)
    inherit GObj.widget_signals_impl obj
    method redraw = self#connect redraw_sig
  end

class cairo obj = 
  object
    inherit GObj.widget (obj : [> gtkcairo] Gtk.obj)

    method connect = new cairo_signals obj
    method event = new GObj.event_ops obj
    method cairo = get_cairo obj
    method queue_draw =
      GtkBase.Widget.queue_draw obj
  end

let cairo =
  GtkBase.Widget.size_params []
    ~cont:(fun pl ?packing ?show () ->
      GObj.pack_return 
	(new cairo (GtkObject.make "GtkCairo" pl))
	~packing ~show)
