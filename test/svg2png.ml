
type args = {
    svg_file : string ;
    png_file : string ;
    scale    : float ;
    width    : int ;
    height   : int ;
  }

let parse_args () =
  let svg_file = ref "" in
  let png_file = ref "" in
  let scale = ref 1. in
  let width = ref (-1) in
  let height = ref (-1) in
  let spec = [
    "-s", Arg.Set_float scale, "scale";
    "-w", Arg.Set_int width, "width";
    "-h", Arg.Set_int height, "height" ] in
  let msg = 
    Printf.sprintf "usage: %s [options] <svg_file> [png_file]"
      (Filename.basename Sys.executable_name) in
  Arg.parse 
    spec
    (fun arg -> 
      if !svg_file = "" then svg_file := arg else
      if !png_file = "" then png_file := arg else
      ())
    msg ;
  if !svg_file = "" then begin
    Arg.usage spec msg ;
    exit 1
  end ;
  if !png_file = "" then
    png_file :=
      begin
	if Filename.check_suffix !svg_file ".svg"
	then Filename.chop_suffix !svg_file ".svg"
	else !svg_file 
      end ^ ".png" ;
  { svg_file = !svg_file ; png_file = !png_file ;
    scale = !scale ; width = !width ; height = !height }

let render_to_png args =
  let cr = Cairo.create () in
  let svgc = Svg_cairo.create () in
  Svg_cairo.parse svgc args.svg_file ;
  let svg_width, svg_height = Svg_cairo.get_size svgc in
  let scale, width, height = 
    if args.width < 0 && args.height < 0 then
      let width  = float svg_width  *. args.scale +. 0.5 in
      let height = float svg_height *. args.scale +. 0.5 in
      args.scale, int_of_float width, int_of_float height 
    else if args.width < 0 then
      let scale = float args.height /. float svg_height in
      let width = float svg_width *. scale +. 0.5 in
      scale, int_of_float width, args.height
    else if args.height < 0 then
      let scale = float args.width /. float svg_width in
      let height = float svg_height *. scale +. 0.5 in
      scale, args.width, int_of_float height
    else 
      let scale = min (float args.height /. float svg_height) (float args.width /. float svg_width) in
      let dx = (float args.width  -. (float svg_width  *. scale +. 0.5)) /. 2. in
      let dy = (float args.height -. (float svg_height *. scale +. 0.5)) /. 2. in
      Cairo.translate cr dx dy ;
      scale, args.width, args.height in
  
  Cairo.scale cr scale scale ;
  let chan = Cairo_channel.open_out args.png_file in
  Cairo.set_target_png cr chan Cairo.FORMAT_ARGB32 width height ;
  Cairo.set_rgb_color cr 1. 1. 1. ;
  Svg_cairo.render svgc cr ;
  Cairo.show_page cr ;
  Cairo_channel.close chan

let _ = 
  render_to_png (parse_args ())
