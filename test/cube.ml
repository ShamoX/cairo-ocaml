let animate_rotate_step = 0.05
let animate_frame_delay = 40
let rotate_max = 8. *. atan 1.
let box_size = 100
let margin   = 20

let setup_transform cr =
  cr#default_matrix ;
  let off = float (box_size + margin) in
  cr#translate off off

let expose_cb c _ =
  let cr = c#cairo in
  cr#rectangle 0. 0. (float box_size) (float box_size) ;
  cr#set_rgb_color 1. 1. 1. ;
  cr#fill ;
  false

let setup packing =
  let size = 2 * box_size + margin in
  let c = Ocairo_gtkcairo.cairo ~width:size ~height:size ~packing () in
  setup_transform c#cairo ;
  c#event#connect#after#expose (expose_cb c) ;
  c

let slider_changed range c () =
  let cr = c#cairo in
  setup_transform cr ;
  cr#rotate range#adjustment#value ;
  c#queue_draw

let animate_step range () =
  let nv = range#adjustment#value +. animate_rotate_step in
  range#adjustment#set_value (mod_float nv rotate_max) ;
  true

let animate_toggled button range =
  let timeout = ref None in
  fun () ->
    match !timeout with
    | None when button#active ->
	let id = 
	  Glib.Timeout.add animate_frame_delay
	    (animate_step range) in
	timeout := Some id
    | Some id when not button#active ->
	Glib.Timeout.remove id
    | _ -> ()



let main =
  let w = GWindow.window ~title:"GtkCairo Demo" () in
  w#connect#destroy GMain.quit ;
  
  let b = GPack.vbox ~spacing:6 ~border_width:12 
      ~packing:w#add () in

  let f = GBin.frame ~shadow_type:`IN ~packing:b#pack () in

  let cairo = setup f#add in

  let slider = GRange.scale `HORIZONTAL 
      ~draw_value:false ~packing:b#pack () in
  slider#adjustment#set_bounds 
    ~lower:0. ~upper:rotate_max
    ~step_incr:animate_rotate_step () ;
  slider#connect#value_changed 
    (slider_changed slider cairo) ;

  let button = GButton.check_button ~label:"Animate" 
      ~packing:b#pack () in
  button#connect#toggled (animate_toggled button slider) ;

  w#show () ;
  GMain.main ()
