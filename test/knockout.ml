(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

let rect_path cr x y width height =
  Cairo.new_path cr ;
  Cairo.move_to cr x y ;
  Cairo.rel_line_to cr 0. height ;
  Cairo.rel_line_to cr width 0. ;
  Cairo.rel_line_to cr 0. (~-. height) ;
  Cairo.rel_line_to cr (~-. width)  0. ;
  Cairo.close_path cr



let pi = 4. *. atan 1.
let oval_path cr xc yc xr yr =
  Cairo.new_path cr ;
  Cairo.move_to cr (xc +. xr) yc ;
  
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

    Cairo.curve_to ~cr ~x1 ~y1 ~x2 ~y2 ~x3 ~y3
  done ;
  Cairo.close_path cr




let check_size = 32
let fill_checks c width height =
  Cairo.save c ; begin
    let check = Cairo.surface_create_similar
	(Cairo.current_target_surface c)
	Cairo.FORMAT_RGB24 (2 * check_size) (2 * check_size) in
    Cairo.surface_set_repeat check true ;

    Cairo.save c ; begin
      let f_size = float check_size in
      Cairo.set_target_surface c check ;
      Cairo.set_operator c Cairo.OPERATOR_SRC ;
      Cairo.set_rgb_color c 0.4 0.4 0.4 ;
      rect_path c 0. 0. (2. *. f_size) (2. *. f_size) ;

      Cairo.set_rgb_color c 0.7 0.7 0.7 ;
      rect_path c 0. 0. f_size f_size ;
      Cairo.fill c ;
      rect_path c f_size f_size f_size f_size ;
      Cairo.fill c end ;
    Cairo.restore c ;

    Cairo.set_pattern c (Cairo.pattern_create_for_surface check) ;
    rect_path c 0. 0. (float width) (float height) ;
    Cairo.fill c end ;
  Cairo.restore c



let draw_3circles c xc yc radius =
  let subradius = radius *. (2. /. 3. -. 0.1) in

  List.iter (fun ((r, g, b), off) ->
    Cairo.set_rgb_color c r g b ;
    oval_path c 
      (xc +. radius /. 3. *. cos (pi *. (0.5 +. off)))
      (yc -. radius /. 3. *. sin (pi *. (0.5 +. off)))
      subradius subradius ;
    Cairo.fill c)
    [ (1., 0., 0.), 0. ;
      (0., 1., 0.), 2./.3. ;
      (0., 0., 1.), 4./.3. ; ]


let expose c d_area ev =
  let { Gtk.width = width ; 
	Gtk.height = height } = d_area#misc#allocation in
  let drawable = d_area#misc#window in

  let radius = 0.5 *. float (min width height) -. 10. in
  let xc = float width /. 2. in
  let yc = float height /. 2. in

  Cairo_lablgtk.set_target_drawable c drawable ;
  let sur = Cairo.current_target_surface c in

  let overlay = Cairo.surface_create_similar sur Cairo.FORMAT_ARGB32 width height in
  let punch = Cairo.surface_create_similar sur Cairo.FORMAT_A8 width height in
  let circles = Cairo.surface_create_similar sur Cairo.FORMAT_ARGB32 width height in

  fill_checks c width height ;

  Cairo.save c ; begin
    Cairo.set_target_surface c overlay ;
    Cairo.set_rgb_color c 0. 0. 0. ;
    oval_path c xc yc radius radius ;
    Cairo.fill c ;
    Cairo.save c ; begin
      Cairo.set_target_surface c punch ;
      draw_3circles c xc yc radius end ;
    Cairo.restore c ;
    Cairo.set_operator c Cairo.OPERATOR_OUT_REVERSE ;
    Cairo.show_surface c punch width height ;
    Cairo.save c ; begin
      Cairo.set_target_surface c circles ;
      Cairo.set_alpha c 0.5 ;
      Cairo.set_operator c Cairo.OPERATOR_OVER ;
      draw_3circles c xc yc radius end ;
    Cairo.restore c ;
    Cairo.set_operator c Cairo.OPERATOR_ADD ;
    Cairo.show_surface c circles width height end ;
  Cairo.restore c ;
  Cairo.show_surface c overlay width height ;
  true


let main () =
  let w = GWindow.window ~title:"Knockout Groups" ~width:400 ~height:400 () in
  w#connect#destroy GMain.quit ;

  let c = Cairo.create () in

  let d = GMisc.drawing_area ~packing:w#add () in
  d#event#connect#expose (expose c d) ;

  w#show () ;
  GMain.main ()

let _ = 
  if not !Sys.interactive
  then main ()
