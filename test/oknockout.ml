
let rect_path cr x y width height =
  cr#new_path ;
  cr#move_to x y ;
  cr#rel_line_to 0. height ;
  cr#rel_line_to width 0. ;
  cr#rel_line_to 0. (~-. height) ;
  cr#rel_line_to (~-. width)  0. ;
  cr#close_path



let pi = 4. *. atan 1.
let oval_path cr xc yc xr yr =
  cr#new_path ;
  cr#move_to (xc +. xr) yc ;
  
  let tangent_mult = 1.65591 /. 3. in
  for i=0 to 3 do
    let angle1 = (float  i      /. 2.) *. pi in
    let angle2 = (float (i + 1) /. 2.) *. pi in
    
    let x0 = xc +. xr *. cos angle1 in
    let y0 = yc -. yr *. sin angle1 in
    let x1 = x0 -. xr *. sin angle1 *. tangent_mult in
    let y1 = y0 -. yr *. cos angle1 *. tangent_mult in
    let x3 = xc +. xr *. cos angle2 in
    let y3 = yc -. yr *. sin angle2 in
    let x2 = x3 +. xr *. sin angle2 *. tangent_mult in
    let y2 = y3 +. yr *. cos angle2 *. tangent_mult in

    cr#curve_to x1 y1 x2 y2 x3 y3
  done ;
  cr#close_path




let check_size = 32
let fill_checks cr width height =
  cr#save ; begin
    let check = cr#target_surface#create_similar 
	Ocairo.FORMAT_RGB24 (2 * check_size) (2 * check_size) in
    check#set_repeat true ;

    cr#save ; begin
      let f_size = float check_size in
      cr#set_target_surface check ;
      cr#set_operator Ocairo.OPERATOR_SRC ;
      cr#set_rgb_color 0.4 0.4 0.4 ;
      rect_path cr 0. 0. (2. *. f_size) (2. *. f_size) ;

      cr#set_rgb_color 0.7 0.7 0.7 ;
      rect_path cr 0. 0. f_size f_size ;
      cr#fill ;
      rect_path cr f_size f_size f_size f_size ;
      cr#fill end ;
    cr#restore ;

    cr#set_pattern check ;
    rect_path cr 0. 0. (float width) (float height) ;
    cr#fill end ;
  cr#restore



let draw_3circles cr xc yc radius =
  let subradius = radius *. (2. /. 3. -. 0.1) in

  List.iter 
    (fun ((r, g, b), off) ->
      cr#set_rgb_color r g b ;
      oval_path cr
	(xc +. radius /. 3. *. cos (pi *. (0.5 +. off)))
	(yc -. radius /. 3. *. sin (pi *. (0.5 +. off)))
	subradius subradius ;
      cr#fill)
    [ (1., 0., 0.), 0. ;
      (0., 1., 0.), 2./.3. ;
      (0., 0., 1.), 4./.3. ; ]


let expose cr d_area ev =
  let { Gtk.width = width ; 
	Gtk.height = height } = d_area#misc#allocation in
  let drawable = d_area#misc#window in

  let radius = 0.5 *. float (min width height) -. 10. in
  let xc = float width /. 2. in
  let yc = float height /. 2. in

  Cairo_lablgtk.set_target_drawable cr#get_cairo drawable ;
  let sur = cr#target_surface in

  let overlay = sur#create_similar Ocairo.FORMAT_ARGB32 width height in
  let punch   = sur#create_similar Ocairo.FORMAT_A8     width height in
  let circles = sur#create_similar Ocairo.FORMAT_ARGB32 width height in

  fill_checks cr width height ;

  cr#save ; begin
    cr#set_target_surface overlay ;
    cr#set_rgb_color 0. 0. 0. ;
    oval_path cr xc yc radius radius ;
    cr#fill ;
    cr#save ; begin
      cr#set_target_surface punch ;
      draw_3circles cr xc yc radius end ;
    cr#restore ;
    cr#set_operator Ocairo.OPERATOR_OUT_REVERSE ;
    cr#show_surface punch width height ;
    cr#save ; begin
      cr#set_target_surface circles ;
      cr#set_alpha 0.5 ;
      cr#set_operator Ocairo.OPERATOR_OVER ;
      draw_3circles cr xc yc radius end ;
    cr#restore ;
    cr#set_operator Ocairo.OPERATOR_ADD ;
    cr#show_surface circles width height end ;
  cr#restore ;
  cr#show_surface overlay width height ;
  true


let main () =
  let w = GWindow.window ~title:"Knockout Groups" ~width:400 ~height:400 () in
  w#connect#destroy GMain.quit ;

  let d = GMisc.drawing_area ~packing:w#add () in
  d#event#connect#expose 
    (expose (Ocairo.cairo ()) d) ;

  w#show () ;
  GMain.main ()

let _ = 
  if not !Sys.interactive
  then main ()
