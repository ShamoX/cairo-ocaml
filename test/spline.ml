
type point = Ocairo.point = 
    { mutable x : float ; 
      mutable y : float }

type spl = {
    mutable pm           : GDraw.pixmap ;
            pt         	 : point array ;
    mutable tolerance  	 : float ;
    mutable line_width 	 : float ;
            line_cap   	 : Ocairo.line_cap ;
    mutable zoom       	 : float ;
    mutable xtrans     	 : float ;
    mutable ytrans     	 : float ;
    mutable click      	 : bool ;
            drag_pt    	 : point ;
    mutable active       : int ;
    mutable width        : int ;
    mutable height       : int ;
  }


let ribbon = 
  [| 110., 20.  ; 310., 300. ;
     10. , 310. ; 210., 20.  |]

let spline_copy arr =
  Array.map
    (fun (x, y) -> { x = x ; y = y })
    arr

let new_pixmap width height =
  let drawable = GDraw.pixmap ~width ~height () in
  drawable#set_foreground `WHITE ;
  drawable#rectangle
    ~x:0 ~y:0 ~width ~height ~filled:true () ;
  drawable

let init_spl () = 
  let width = 400 in
  let height = 400 in
  {
   pm = new_pixmap width height ;
   pt = spline_copy ribbon ;
   tolerance = 0.1 ;
   line_width = 10. ; 
   line_cap = Ocairo.LINE_CAP_ROUND ;
   zoom = 1. ;
   xtrans = 0. ;
   ytrans = 0. ;
   click = false ;
   drag_pt = { x = 0. ; y = 0. } ;
   active = 0 ;
   width = width ;
   height = height ;
 }


let draw_control_line ct a b w =
  ct#save ; begin
    ct#set_rgb_color 0. 0. 1. ;
    ct#set_line_width w ;
    ct#move_to a.x a.y ;
    ct#line_to b.x b.y ;
    ct#stroke end ;
  ct#restore

let two_pi = 8. *. atan 1.

let draw_spline ct spl =
  let drag_pt = { x = spl.drag_pt.x ; y = spl.drag_pt.y } in
  ct#inverse_transform_point drag_pt ;
  ct#save ; begin
    ct#move_to  spl.pt.(0).x spl.pt.(0).y ;
    ct#curve_to 
      spl.pt.(1).x spl.pt.(1).y 
      spl.pt.(2).x spl.pt.(2).y 
      spl.pt.(3).x spl.pt.(3).y ;
    
    if spl.click && ct#in_stroke drag_pt.x drag_pt.y
    then spl.active <- 0xf ;

    ct#stroke ;

    draw_control_line ct spl.pt.(0) spl.pt.(1) (2. /. spl.zoom) ;
    draw_control_line ct spl.pt.(3) spl.pt.(2) (2. /. spl.zoom) ;

    for i=0 to 3 do
      ct#save ; begin
	ct#set_rgb_color 1. 0. 0. ;
	ct#set_alpha 0.5 ;
	ct#new_path ;
	ct#arc 
	  spl.pt.(i).x spl.pt.(i).y
	  (spl.line_width /. 1.25)
	  0. two_pi ;
	if spl.click && ct#in_fill drag_pt.x drag_pt.y
	then begin
	  spl.active <- 1 lsl i ;
	  spl.click <- false
	end ;
	ct#fill end ;
      ct#restore
    done end ;
  ct#restore
	  

let refresh spl =
  let ct = Ocairo_lablgtk.cairo ~target:spl.pm#pixmap () in
  spl.pm#rectangle ~x:0 ~y:0 
    ~width:spl.width ~height:spl.height ~filled:true () ;
  ct#set_rgb_color 0. 0. 0. ;
  ct#set_line_width spl.line_width ;
  ct#set_line_cap spl.line_cap ;
  ct#translate spl.xtrans spl.ytrans ;
  ct#scale spl.zoom spl.zoom ;
  ct#set_tolerance spl.tolerance ;

  try draw_spline ct spl
  with Ocairo.Error _ ->
    prerr_endline "Cairo is unhappy"


let trans_horiz_cb dir spl =
  let delta = float spl.width /. 16. in
  begin match dir with
  | `LEFT  -> spl.xtrans <- spl.xtrans -. delta
  | `RIGHT -> spl.xtrans <- spl.xtrans +. delta
  end ;
  true

let trans_vert_cb dir spl =
  let delta = float spl.height /. 16. in
  begin match dir with
  | `UP   -> spl.ytrans <- spl.ytrans -. delta
  | `DOWN -> spl.ytrans <- spl.ytrans +. delta
  end ;
  true

let zoom_cb dir spl = 
  begin match dir with
  | `OUT -> spl.zoom <- spl.zoom /. 1.1 
  | `IN  -> spl.zoom <- spl.zoom *. 1.1 
  end ; 
  true

let smooth_cb dir spl =
  begin match dir with
  | `INC -> spl.tolerance <- spl.tolerance *. 10.
  | `DEC -> spl.tolerance <- spl.tolerance /. 10.
  end ; 
  true

let line_width_cb dir spl =
  begin match dir with
  | `W -> spl.line_width <- spl.line_width *. 2.
  | `N -> spl.line_width <- spl.line_width /. 2.
  end ; 
  true

let print_spline_cb { pt = pt } =
  let pt_f fmt p =
    Format.fprintf fmt "{@[ %.20g,@ %.20g @]}" p.x p.y in
  Format.printf "@[{ %a,@ %a,@ %a,@ %a }@]@." 
    pt_f pt.(0) pt_f pt.(1) pt_f pt.(2) pt_f pt.(3) ;
  false

module K = GdkKeysyms

let keybindings = [
  K._q,          ("Q",       (fun _ -> GMain.quit () ; false),
		  "Exit the program") ;
  K._Left,  	 ("Left",    trans_horiz_cb `LEFT,
		  "Translate left") ;
  K._Right, 	 ("Right",   trans_horiz_cb `RIGHT,
		  "Translate right" ) ;
  K._Up,         ("Up",      trans_vert_cb `UP,
		  "Translate up" ) ;
  K._Down,       ("Down",    trans_vert_cb `DOWN,
		  "Translate down") ;
  K._Return,     ("Return",  print_spline_cb,
		  "Print current spline coordinates on stdout") ;
  K._plus,       ("plus",    zoom_cb `IN,
		  "Zoom in") ;
  K._minus,      ("minus",   zoom_cb `OUT,
		  "Zoom out") ;
  K._greater,    ("greater", smooth_cb `DEC,
		  "Increase rendering accuracy, (tolerance /= 10)") ;
  K._less,       ("less",    smooth_cb `INC,
		  "Decrease rendering accuracy, (tolerance *= 10)") ;
  K._w,          ("W",       line_width_cb `W,
		  "Widen line width") ;
  K._n,          ("N",       line_width_cb `N,
		  "Narrow line width") ;
]

let grow_pixmap spl =
  spl.pm <- new_pixmap spl.width spl.height ;
  refresh spl

let config_cb spl ev =
  let module EC = GdkEvent.Configure in
  let has_grown = EC.width ev > spl.width || EC.height ev > spl.height in
  spl.width <- EC.width ev ;
  spl.height <- EC.height ev ;
  if has_grown
  then grow_pixmap spl ;
  true

let expose da spl x y width height =
  let gwin = da#misc#window in
  let d = new GDraw.drawable gwin in
  d#put_pixmap ~x ~y ~xsrc:x ~ysrc:y ~width ~height spl.pm#pixmap

let expose_cb da spl ev =
  let area = GdkEvent.Expose.area ev in
  let module GR = Gdk.Rectangle in
  expose da spl (GR.x area) (GR.y area) (GR.width area) (GR.height area) ;
  true

let refresh_and_expose da spl =
  refresh spl ;
  expose da spl 0 0 spl.width spl.height


let key_press_cb da spl ev =
  try 
    let (_, cb, _) =
      List.assoc (GdkEvent.Key.keyval ev) keybindings in
    let need_refresh = cb spl in
    if need_refresh 
    then refresh_and_expose da spl ;
    true
  with Not_found -> false

let button_ev da spl ev =
  match GdkEvent.get_type ev with
  | `BUTTON_PRESS ->
      spl.click <- true ;
      spl.drag_pt.x <- GdkEvent.Button.x ev ;
      spl.drag_pt.y <- GdkEvent.Button.y ev ;
      refresh_and_expose da spl ;
      true
  | `BUTTON_RELEASE -> 
      spl.click  <- false ;
      spl.active <- 0 ;
      refresh_and_expose da spl ;
      true
  | _ -> false

let motion_notify_cb da spl ev =
  let x = GdkEvent.Motion.x ev in
  let y = GdkEvent.Motion.y ev in
  for i=0 to 3 do
    if (1 lsl i) land spl.active != 0
    then begin
      spl.pt.(i).x <- spl.pt.(i).x +. (x -. spl.drag_pt.x) /. spl.zoom ;
      spl.pt.(i).y <- spl.pt.(i).y +. (y -. spl.drag_pt.y) /. spl.zoom
    end
  done ;
  spl.drag_pt.x <- x ;
  spl.drag_pt.y <- y ;
  refresh_and_expose da spl ;
  true


let init spl packing =
  let da = GMisc.drawing_area ~width:spl.width ~height:spl.height ~packing () in
  da#misc#set_can_focus true ;
  begin
    let timeout = ref None in
    da#misc#connect#realize
      (fun () ->
	timeout := Some
	    (Glib.Timeout.add 20 
	       (fun () -> da#misc#pointer ; true))) ;
    da#misc#connect#unrealize
      (fun () ->
	match !timeout with
	| None -> ()
	| Some id ->
	    Glib.Timeout.remove id)
  end ;
  da#event#add [ `KEY_PRESS ; `POINTER_MOTION_HINT ; 
		 `BUTTON_MOTION ;
		 `BUTTON_PRESS ; `BUTTON_RELEASE ] ;
  da#event#connect#expose         (expose_cb da spl) ;
  da#event#connect#configure      (config_cb spl) ;
  da#event#connect#button_press   (button_ev da spl) ;
  da#event#connect#button_release (button_ev da spl) ;
  da#event#connect#motion_notify  (motion_notify_cb da spl) ; 
  da#event#connect#key_press      (key_press_cb da spl) ;
  refresh spl

let show_help kb =
  Format.printf "@[<v>" ;
  List.iter (fun (_, (key, _, descr)) ->
    Format.printf "%10s: %s@ " key descr)
    kb ;
  Format.printf "@."

let main = 
  let w = GWindow.window 
      ~title:"Cairo spline demo" 
      ~allow_grow:true
      ~allow_shrink:true
      () in
  w#connect#destroy GMain.quit ;
  init (init_spl ()) w#add ;
  show_help keybindings ;
  w#show () ;
  GMain.main ()
