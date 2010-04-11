(* This is a direct translation of the example Pango-Cairo program
 * which can be found on this page:
 * http://developer.gnome.org/doc/API/2.0/pango/pango-Cairo-Rendering.html
 * $Id$
 * By Richard W.M. Jones <rich at annexia.org> or <rich at merjis.com>
 *)

let pi = 4. *. atan 1. (* Annoying omission from the stdlib. *)

let radius = 150
let n_words = 10
let font = "Sans Bold 27"

let radius_f = float radius
let n_words_f = float n_words
let scale_f = float Pango.scale

let draw_text cr =
  Cairo.translate cr radius_f radius_f;
  let layout = Pango_cairo.create_layout cr in
  Pango.Layout.set_text layout "Text";
  let desc = Pango.Font.from_string font in
  Pango.Layout.set_font_description layout desc;

  (* Draw the layout n_words times in a circle. *)
  for i = 0 to n_words-1; do
    let angle = (360. *. float i) /. n_words_f in
    Cairo.save cr; begin
	(* Gradient from red at angle == 60 to blue at angle == 240 *)
	let red = (1. +. cos ((angle -. 60.) *. pi /. 180.)) /. 2. in
	Cairo.set_source_rgb cr red 0. (1.0 -. red);

	Cairo.rotate cr (angle *. pi /. 180.);

	(* Inform Pango to re-layout the text with the new transformation *)
	Pango_cairo.update_layout cr layout;

	let width, height = Pango.Layout.get_size layout in
	Cairo.move_to cr ~-.((float width/.scale_f) /. 2.) ~-.radius_f;
	Pango_cairo.show_layout cr layout;

      end; Cairo.restore cr
  done

let () =
  if Array.length Sys.argv <> 2 then
    failwith "Usage: pangocairo OUTPUT_FILENAME.png";

  let filename = Sys.argv.(1) in
  let surface =
    Cairo.image_surface_create
      Cairo.FORMAT_ARGB32 (2 * radius) (2 * radius) in
  let cr = Cairo.create surface in

  Cairo.set_source_rgb cr 1. 1. 1.;
  Cairo.paint cr;
  draw_text cr;

  Cairo_png.surface_write_to_file surface filename
