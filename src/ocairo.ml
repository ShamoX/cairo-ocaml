
type status = Cairo.status =
    INVALID_RESTORE
  | INVALID_POP_GROUP
  | NO_CURRENT_POINT
  | INVALID_MATRIX
  | NO_TARGET_SURFACE
  | NULL_POINTER
exception Error = Cairo.Error

type point = Cairo.point =
    { mutable x : float ; mutable y : float }

type operator = Cairo.operator =
    OPERATOR_CLEAR
  | OPERATOR_SRC
  | OPERATOR_DST
  | OPERATOR_OVER
  | OPERATOR_OVER_REVERSE
  | OPERATOR_IN
  | OPERATOR_IN_REVERSE
  | OPERATOR_OUT
  | OPERATOR_OUT_REVERSE
  | OPERATOR_ATOP
  | OPERATOR_ATOP_REVERSE
  | OPERATOR_XOR
  | OPERATOR_ADD
  | OPERATOR_SATURATE
type fill_rule = Cairo.fill_rule = FILL_RULE_WINDING | FILL_RULE_EVEN_ODD
type line_cap = Cairo.line_cap = LINE_CAP_BUTT | LINE_CAP_ROUND | LINE_CAP_SQUARE
type line_join = Cairo.line_join = LINE_JOIN_MITER | LINE_JOIN_ROUND | LINE_JOIN_BEVEL

type glyph = Cairo.glyph = { index : int; glyph_x : float; glyph_y : float; }
type text_extents = Cairo.text_extents = { 
    x_bearing   : float ;
    y_bearing   : float ;
    text_width  : float ;
    text_height : float ;
    x_advance   : float ;
    y_advance   : float }
type font_extents = Cairo.font_extents = {
  ascent        : float;
  descent       : float;
  font_height   : float;
  max_x_advance : float;
  max_y_advance : float;
}
type font_weight = Cairo.font_weight = FONT_WEIGHT_NORMAL | FONT_WEIGHT_BOLD
type font_slant = Cairo.font_slant = 
    FONT_SLANT_NORMAL 
  | FONT_SLANT_ITALIC 
  | FONT_SLANT_OBLIQUE

type format = Cairo.format =
    FORMAT_ARGB32 
  | FORMAT_RGB24 
  | FORMAT_A8 
  | FORMAT_A1
type filter = Cairo.filter =
    FILTER_FAST
  | FILTER_GOOD
  | FILTER_BEST
  | FILTER_NEAREST
  | FILTER_BILINEAR

class cairo cairo =
  object
    val c = cairo

    method get_cairo = c

    method save = Cairo.save c
    method restore = Cairo.restore c
    method copy =
      let c' = Cairo.create () in
      Cairo.copy c' c ;
      new cairo c'

    method set_target_surface (s : surface) = Cairo.set_target_surface c s#get_surface
    method target_surface = 
      new surface (Cairo.current_target_surface c)
    method set_target_image   i = Cairo.set_target_image c i
    method set_target_ps chan w h xpi ypi =
      Cairo.set_target_ps c chan w h xpi ypi
    method finalise_target = Cairo.finalise_target

    method current_point = Cairo.current_point c
    method set_operator o = Cairo.set_operator c o
    method operator = Cairo.current_operator c
    method set_rgb_color r g b = Cairo.set_rgb_color c r g b
    method rgb_color = Cairo.current_rgb_color c
    method set_alpha a = Cairo.set_alpha c a
    method alpha = Cairo.current_alpha c
    method set_pattern (s : surface) = Cairo.set_pattern c s#get_surface
    method set_tolerance t = Cairo.set_tolerance c t
    method tolerance = Cairo.current_tolerance c
    method set_fill_rule v = Cairo.set_fill_rule c v
    method fill_rule = Cairo.current_fill_rule c
    method set_line_width v = Cairo.set_line_width c v
    method line_width = Cairo.current_line_width c
    method set_line_cap v = Cairo.set_line_cap c v
    method line_cap = Cairo.current_line_cap c
    method set_line_join v = Cairo.set_line_join c v
    method line_join = Cairo.current_line_join c
    method set_dash d o = Cairo.set_dash c d o
    method set_miter_limit v = Cairo.set_miter_limit c v
    method miter_limit = Cairo.current_miter_limit c

    method translate tx ty = Cairo.translate c tx ty
    method scale sx sy = Cairo.scale c sx sy
    method rotate a = Cairo.rotate c a
    method concat_matrix (mat : matrix) = Cairo.concat_matrix c mat#get_matrix
    method set_matrix (mat : matrix) = Cairo.set_matrix c mat#get_matrix
    method matrix = 
      let m = new matrix in
      Cairo.current_matrix c m#get_matrix ;
      m
    method default_matrix = Cairo.default_matrix c
    method identity_matrix = Cairo.identity_matrix c

    method transform_point co = Cairo.transform_point c co
    method transform_distance co = Cairo.transform_distance c co
    method inverse_transform_point co = Cairo.inverse_transform_point c co
    method inverse_transform_distance co = Cairo.inverse_transform_distance c co

    method new_path = Cairo.new_path c
    method move_to x y = Cairo.move_to c x y
    method move_to_point p = Cairo.move_to_point c p
    method line_to x y = Cairo.line_to c x y
    method curve_to x1 y1 x2 y2 x3 y3 = Cairo.curve_to c x1 y1 x2 y2 x3 y3
    method arc xc yc rad a1 a2 = Cairo.arc c xc yc rad a1 a2
    method arc_negative xc yc rad a1 a2 = Cairo.arc_negative c xc yc rad a1 a2
    method rel_move_to dx dy = Cairo.rel_move_to c dx dy
    method rel_line_to dx dy = Cairo.rel_line_to c dx dy
    method rel_curve_to dx1 dy1 dx2 dy2 dx3 dy3 = Cairo.rel_curve_to c dx1 dy1 dx2 dy2 dx3 dy3
    method rectangle x y w h = Cairo.rectangle c x y w h
    method close_path = Cairo.close_path c
    method stroke = Cairo.stroke c
    method fill = Cairo.fill c
    method current_path : 'a 'b. ('a -> ([> Cairo.basic_path] as 'b) -> 'a) -> 'a -> 'a =
      Cairo.fold_current_path c
    method current_path_flat : 'a 'b. ('a -> ([> Cairo.flat_path] as 'b) -> 'a) -> 'a -> 'a =
      Cairo.fold_current_path_flat c

    method in_stroke x y = Cairo.in_stroke c x y
    method in_fill x y = Cairo.in_fill c x y

    method show_surface (s : surface) w h = Cairo.show_surface c s#get_surface w h
    method copy_page = Cairo.copy_page c
    method show_page = Cairo.show_page c
    method clip = Cairo.clip c

    method select_font f sl w = Cairo.select_font c f sl w
    method scale_font s = Cairo.scale_font c s
    method transform_font (mat : matrix) = Cairo.transform_font c mat#get_matrix
    method show_text str = Cairo.show_text c str
    method show_glyphs gl = Cairo.show_glyphs c gl
    method font_extents = Cairo.current_font_extents c
    method text_extents s = Cairo.text_extents c s
    method glyph_extents gl = Cairo.glyph_extents c gl
    method text_path s = Cairo.text_path c s
    method glyph_path gl = Cairo.glyph_path c gl
  end

and matrix = 
    object (self)
      val m = Cairo.matrix_create ()
      method get_matrix = m

      method copy =
	let m' = new matrix in
	Cairo.matrix_copy m'#get_matrix m ; m'

      method set_identity = Cairo.matrix_set_identity m
      method set_affine aff = Cairo.matrix_set_affine m aff
      method get_affine = Cairo.matrix_get_affine m
      method translate tx ty = 
	let m' = self#copy in
	Cairo.matrix_translate m'#get_matrix tx ty ; m'
      method scale sx sy = 
	let m' = self#copy in
	Cairo.matrix_scale m'#get_matrix sx sy ; m'
      method rotate a = 
	let m' = self#copy in
	Cairo.matrix_rotate m'#get_matrix a ; m'
      method invert = 
	let m' = self#copy in
	Cairo.matrix_invert m'#get_matrix ; m'
      method multiply (b : matrix) =
	let r = new matrix in
	Cairo.matrix_multiply r#get_matrix m b#get_matrix ; r
      method transform_distance co = Cairo.matrix_transform_distance m co
      method transform_point co = Cairo.matrix_transform_point m co
    end

and surface surf = 
    object
      val s = surf
      method get_surface = s

      method create_similar f w h =
	new surface (Cairo.surface_create_similar s f w h)
      method set_repeat r = Cairo.surface_set_repeat s r
      method set_matrix (m : matrix) = Cairo.surface_set_matrix s m#get_matrix
      method get_matrix =
	let m = new matrix in
	Cairo.surface_get_matrix s m#get_matrix ;
	m
      method set_filter f = Cairo.surface_set_filter s f
      method finalise = Cairo.surface_finalise s
    end

let cairo () =
  new cairo (Cairo.create ())

let surface_create_for_image i =
  new surface (Cairo.surface_create_for_image i)

let image_surface_create f w h =
  new surface (Cairo.image_surface_create f w h)
let ps_surface_create chan w h xpi ypi =
  new surface (Cairo.ps_surface_create chan w h xpi ypi)
