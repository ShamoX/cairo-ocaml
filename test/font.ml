
let out_file name =
  let oc = open_out name in
  let channel = Cairo_channel.of_out_channel oc in
  close_out oc ;
  channel

let pi = 4. *. atan 1.

let main font_arg =
  let ft = Cairo_ft.init_freetype () in
  let font, clean_up =
    if Sys.file_exists font_arg
    then
      let face = Cairo_ft.new_face ft font_arg in
      let font = Cairo_ft.font_create_for_ft_face face in
      (font, (fun () -> Cairo_ft.done_face face))
    else
      let pattern = Cairo_ft.fc_name_parse font_arg in
      let font = Cairo_ft.font_create ft pattern in
      (font, ignore)
  in

  let cr = Cairo.create () in
  let file = out_file "test_font.png" in
  Cairo.set_target_png ~cr ~file Cairo.FORMAT_ARGB32 ~width:200 ~height:200 ;

  Cairo.set_font ~cr ~font ;

  Cairo.scale_font cr 20. ;
  Cairo.move_to cr 10. 10. ;
  Cairo.rotate cr (pi /. 2.) ;
  Cairo.show_text cr "Hello World !" ;

  Cairo.finalise_target ~cr ;
  Cairo_channel.close file ;

  clean_up () ;
  Cairo_ft.done_freetype ft

let _ =
  if Array.length Sys.argv < 2 then exit 1 ;
  main Sys.argv.(1)



