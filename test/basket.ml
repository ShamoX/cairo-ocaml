
let _ =
  Cairo.init

let draw c =
  Cairo.move_to c 10. 10. ;
  Cairo.line_to c 510. 10. ;
  Cairo.curve_to c 410. 200. 110. 200. 10. 10. ;
  Cairo.close_path c ;
  
  Cairo.save c ;
  Cairo.set_rgb_color c 0.8 0.1 0.1 ;
  Cairo.fill c ;
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
    let oc = open_out "basket.ps" in
    let file = Cairo_channel.of_out_channel oc in
    close_out oc ;
    Cairo.set_target_ps c file x_inches y_inches x_ppi y_ppi ;
    draw c ;
    Cairo.show_page c ;
    Cairo.finalise_target_ps c ;
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
