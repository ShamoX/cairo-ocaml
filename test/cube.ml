let animate_rotate_step = 0.05
let animate_frame_delay = 40
let rotate_max = 8. *. atan 1.
let initial_size = 200

let redraw w range cr =
  let { Gtk.width = width ; Gtk.height = height } =
    w#misc#allocation in
  let box_size = float (width + height) /. 6. in

  Cairo.save cr ; begin
    Cairo.default_matrix cr ;
    let off = float width /. 2. in
    Cairo.translate cr off off ;
    Cairo.rotate cr range#adjustment#value ;
    Cairo.rectangle cr (~-. box_size) (~-. box_size) box_size box_size ;
    Cairo.set_rgb_color cr 1. 0. 0. ;
    Cairo.fill cr end ;
  Cairo.restore cr

let slider_changed cr () =
  cr#queue_draw

let animate_step range () =
  let nv = range#adjustment#value +. animate_rotate_step in
  range#adjustment#set_value (mod_float nv rotate_max) ;
  true

let animate_toggled button range =
  let timeout = ref None in
  fun () ->
    match !timeout with
    | None when button#active ->
	timeout := Some (
	  Glib.Timeout.add animate_frame_delay 
	    (animate_step range))
    | Some id when not button#active ->
	Glib.Timeout.remove id ;
	timeout := None
    | _ -> ()



let main =
  let w = GWindow.window ~title:"GtkCairo Demo" () in
  w#connect#destroy GMain.quit ;
  
  let b = GPack.vbox ~spacing:6 ~border_width:12 
      ~packing:w#add () in

  let f = GBin.frame ~shadow_type:`IN 
      ~packing:(b#pack ~expand:true ~fill:true) () in

  let cairo = Cairo_gtkcairo.cairo 
      ~width:initial_size ~height:initial_size 
      ~packing:f#add () in
  let slider = GRange.scale `HORIZONTAL 
      ~draw_value:false ~packing:b#pack () in
  slider#adjustment#set_bounds 
    ~lower:0. ~upper:rotate_max
    ~step_incr:animate_rotate_step () ;
  let button = GButton.check_button ~label:"Animate" 
      ~packing:b#pack () in

  cairo#connect#paint (redraw cairo slider) ;
  slider#connect#value_changed 
    (slider_changed cairo) ;
  button#connect#toggled (animate_toggled button slider) ;

  w#show () ;
  GMain.main ()
