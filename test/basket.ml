
let _ =
  Cairo.init

let print_path_elem = function
  | `MOVE_TO { Cairo.x = x ; Cairo.y = y } ->
      Format.printf "@ move_to (%f, %f)" x y
  | `LINE_TO { Cairo.x = x ; Cairo.y = y } ->
      Format.printf "@ line_to (%f, %f)" x y
  | `CURVE_TO ({ Cairo.x = x1 ; Cairo.y = y1 },
	       { Cairo.x = x2 ; Cairo.y = y2 },
	       { Cairo.x = x3 ; Cairo.y = y3 }) ->
      Format.printf "@ curve_to (%f, %f, %f, %f, %f, %f)" x1 y1 x2 y2 x3 y3
  | `CLOSE ->
      Format.printf "@ close\n"
let print_path c =
  Format.printf "@[<v 2>current_path:" ;
  let nb = Cairo.fold_current_path c
      (fun nb el -> print_path_elem el ; nb+1) 0 in
  Format.printf "@]%d elements@." nb

let draw ?(print=false) c =
  Cairo.move_to c 10. 10. ;
  Cairo.line_to c 510. 10. ;
  Cairo.curve_to c 410. 200. 110. 200. 10. 10. ;
  Cairo.close_path c ;
  
  if print then print_path c ;
  
  Cairo.save c ; begin
    Cairo.set_rgb_color c 0.8 0.1 0.1 ;
    Cairo.fill c end ;
  Cairo.restore c ;

  Cairo.set_line_width c 6. ;
  Cairo.set_rgb_color c 0. 0. 0. ;
  Cairo.stroke c



let width = 520.
let height = 170.
let x_inches = width /. 96.
let y_inches = height /. 96.
let x_ppi = 300.
let y_ppi = 300.

let main = 
  let c = Cairo.create () in

  prerr_endline "PS" ;
  begin
    let file = Cairo_channel.open_out "basket.ps" in
    Cairo.set_target_ps c file x_inches y_inches x_ppi y_ppi ;
    draw ~print:true c ;
    Cairo.show_page c ;
    Cairo.finalise_target c ;
    Cairo_channel.close file
  end ;

  prerr_endline "PDF" ;
  begin
    let file = Cairo_channel.open_out "basket.pdf" in
    Cairo.set_target_pdf c file x_inches y_inches x_ppi y_ppi ;
    draw c ;
    Cairo.show_page c ;
    Cairo.finalise_target c ;
    Cairo_channel.close file
  end ;

  prerr_endline "Bigarray and PPM" ;
  begin
    let arr = Bigarray.Array2.create Bigarray.int Bigarray.c_layout
	(int_of_float height) (int_of_float width) in
    Bigarray.Array2.fill arr 0xffffff ;
    let img = Cairo_bigarray.of_bigarr_24 arr in
    Cairo.set_target_image c img ;
    draw c ;
    let oc = open_out "basket.ppm" in
    Cairo_bigarray.write_ppm_int oc arr ;
    close_out oc
  end ;

  prerr_endline "GdkPixbuf and PNG" ;
  begin
    let pb = GdkPixbuf.create
	~width:(int_of_float width)
	~height:(int_of_float height) ~bits:8 ~has_alpha:true () in
    GdkPixbuf.fill pb (Int32.of_string "0xffffffff") ;
    let img = Cairo_lablgtk.image_of_pixbuf pb in
    Cairo.set_target_image c img ;
    draw c ;
    Cairo_lablgtk.shuffle_pixels pb ;
    GdkPixbuf.save ~filename:"basket.png" ~typ:"png" pb
  end
