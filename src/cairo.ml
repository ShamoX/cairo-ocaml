type status =
    INVALID_RESTORE
  | INVALID_POP_GROUP
  | NO_CURRENT_POINT
  | INVALID_MATRIX
  | NO_TARGET_SURFACE
  | NULL_POINTER
exception Error of status
let init = Callback.register "cairo_status_exn" (Error NULL_POINTER)
type format =
    FORMAT_ARGB32
  | FORMAT_RGB24
  | FORMAT_A8
  | FORMAT_A1
type image =
  { data : Obj.t; format : format; width : int; height : int; stride : int }
type point = { mutable x : float ; mutable y : float }
type t
type surface
type matrix
external create : unit -> t = "ml_cairo_create"
external destroy : cr:t -> unit = "ml_cairo_destroy"
external save : cr:t -> unit = "ml_cairo_save"
external restore : cr:t -> unit = "ml_cairo_restore"
external copy : dest:t -> src:t -> unit = "ml_cairo_copy"
external set_target_surface :
  cr:t -> surface:surface -> unit = "ml_cairo_set_target_surface"
external set_target_image :
  cr:t -> image -> unit = "ml_cairo_set_target_image"
external set_target_ps :
  cr:t -> file:Cairo_channel.t -> width_inches:float -> height_inches:float ->
    x_pixels_per_inch:float -> y_pixels_per_inch:float ->
    unit = "ml_cairo_set_target_ps_bc" "ml_cairo_set_target_ps"
type operator =
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
external set_operator : cr:t -> op:operator -> unit = "ml_cairo_set_operator"
external set_rgb_color :
  cr:t -> red:float -> green:float -> blue:float ->
    unit = "ml_cairo_set_rgb_color"
external set_alpha : cr:t -> alpha:float -> unit = "ml_cairo_set_alpha"
external set_pattern :
  cr:t -> pattern:surface -> unit = "ml_cairo_set_pattern"
external set_tolerance :
  cr:t -> tolerance:float -> unit = "ml_cairo_set_tolerance"
type fill_rule =
    FILL_RULE_WINDING
  | FILL_RULE_EVEN_ODD
external set_fill_rule :
  cr:t -> fill_rule:fill_rule -> unit = "ml_cairo_set_fill_rule"
external set_line_width :
  cr:t -> width:float -> unit = "ml_cairo_set_line_width"
type line_cap =
    LINE_CAP_BUTT
  | LINE_CAP_ROUND
  | LINE_CAP_SQUARE
external set_line_cap :
  cr:t -> line_cap:line_cap -> unit = "ml_cairo_set_line_cap"
type line_join =
    LINE_JOIN_MITER
  | LINE_JOIN_ROUND
  | LINE_JOIN_BEVEL
external set_line_join :
  cr:t -> line_join:line_join -> unit = "ml_cairo_set_line_join"
external set_dash :
  cr:t -> dashes:float array -> offset:float -> unit = "ml_cairo_set_dash"
external set_miter_limit :
  cr:t -> limit:float -> unit = "ml_cairo_set_miter_limit"
external translate :
  cr:t -> tx:float -> ty:float -> unit = "ml_cairo_translate"
external scale : cr:t -> sx:float -> sy:float -> unit = "ml_cairo_scale"
external rotate : cr:t -> angle:float -> unit = "ml_cairo_rotate"
external concat_matrix :
  cr:t -> matrix:matrix -> unit = "ml_cairo_concat_matrix"
external set_matrix : cr:t -> matrix:matrix -> unit = "ml_cairo_set_matrix"
external default_matrix : cr:t -> unit = "ml_cairo_default_matrix"
external identity_matrix : cr:t -> unit = "ml_cairo_identity_matrix"
external transform_point :
  cr:t -> point -> unit = "ml_cairo_transform_point"
external transform_distance :
  cr:t -> point -> unit = "ml_cairo_transform_distance"
external inverse_transform_point :
  cr:t -> point -> unit = "ml_cairo_inverse_transform_point"
external inverse_transform_distance :
  cr:t -> point -> unit = "ml_cairo_inverse_transform_distance"
external new_path : cr:t -> unit = "ml_cairo_new_path"
external move_to : cr:t -> x:float -> y:float -> unit = "ml_cairo_move_to"
let move_to_point ~cr { x = x ; y = y } = move_to ~cr ~x ~y
external line_to : cr:t -> x:float -> y:float -> unit = "ml_cairo_line_to"
external curve_to :
  cr:t -> x1:float -> y1:float -> x2:float -> y2:float -> x3:float ->
    y3:float -> unit = "ml_cairo_curve_to_bc" "ml_cairo_curve_to"
external arc :
  cr:t -> xc:float -> yc:float -> radius:float -> angle1:float ->
    angle2:float -> unit = "ml_cairo_arc_bc" "ml_cairo_arc"
external arc_negative :
  cr:t -> xc:float -> yc:float -> radius:float -> angle1:float ->
    angle2:float -> unit = "ml_cairo_arc_negative_bc" "ml_cairo_arc_negative"
external rel_move_to :
  cr:t -> dx:float -> dy:float -> unit = "ml_cairo_rel_move_to"
external rel_line_to :
  cr:t -> dx:float -> dy:float -> unit = "ml_cairo_rel_line_to"
external rel_curve_to :
  cr:t -> dx1:float -> dy1:float -> dx2:float -> dy2:float -> dx3:float ->
    dy3:float -> unit = "ml_cairo_rel_curve_to_bc" "ml_cairo_rel_curve_to"
external rectangle :
  cr:t -> x:float -> y:float -> width:float -> height:float ->
    unit = "ml_cairo_rectangle"
external close_path : cr:t -> unit = "ml_cairo_close_path"
external stroke : cr:t -> unit = "ml_cairo_stroke"
external fill : cr:t -> unit = "ml_cairo_fill"
external copy_page : cr:t -> unit = "ml_cairo_copy_page"
external show_page : cr:t -> unit = "ml_cairo_show_page"
external in_stroke : cr:t -> x:float -> y:float -> bool = "ml_cairo_in_stroke"
external in_fill : cr:t -> x:float -> y:float -> bool = "ml_cairo_in_fill"
external clip : cr:t -> unit = "ml_cairo_clip"
type font
type glyph = { index : int; glyph_x : float; glyph_y : float; }
type text_extents = 
  { x_bearing   : float ;
    y_bearing   : float ;
    text_width  : float ;
    text_height : float ;
    x_advance   : float ;
    y_advance   : float }
type font_extents =
  { ascent : float;
    descent : float;
    font_height : float;
    max_x_advance : float;
    max_y_advance : float }
type font_weight =
    FONT_WEIGHT_NORMAL
  | FONT_WEIGHT_BOLD
type font_slant =
    FONT_SLANT_NORMAL
  | FONT_SLANT_ITALIC
  | FONT_SLANT_OBLIQUE
external select_font :
  ct:t -> family:string -> slant:font_slant -> weight:font_weight ->
    unit = "ml_cairo_select_font"
external scale_font : cr:t -> scale:float -> unit = "ml_cairo_scale_font"
external transform_font :
  cr:t -> matrix:matrix -> unit = "ml_cairo_transform_font"
external show_text : ct:t -> utf8:string -> unit = "ml_cairo_show_text"
external show_glyphs :
  ct:t -> glyph array -> unit = "ml_cairo_show_glyphs"
external current_font : ct:t -> font = "ml_cairo_current_font"
external current_font_extents :
  ct:t -> font_extents = "ml_cairo_current_font_extents"
external set_font : ct:t -> font:font -> unit = "ml_cairo_set_font"
external text_extents : t -> utf8:string -> text_extents = "ml_cairo_text_extents"
external glyph_extents : t -> glyph array -> text_extents = "ml_cairo_glyph_extents"
external text_path : t -> utf8:string -> unit = "ml_cairo_text_path"
external glyph_path : t -> glyph array -> unit = "ml_cairo_glyph_path"
external font_destroy : font:font -> unit = "ml_cairo_font_destroy"
external font_set_transform :
  font:font -> matrix:matrix -> unit = "ml_cairo_font_set_transform"
external font_current_transform :
  font:font -> matrix:matrix -> unit = "ml_cairo_font_current_transform"
external show_surface :
  cr:t -> surface:surface -> width:int -> height:int ->
    unit = "ml_cairo_show_surface"
external current_operator : cr:t -> operator = "ml_cairo_current_operator"
external current_rgb_color :
  cr:t -> float * float * float = "ml_cairo_current_rgb_color"
external current_alpha : cr:t -> float = "ml_cairo_current_alpha"
external current_tolerance : cr:t -> float = "ml_cairo_current_tolerance"
external current_point : cr:t -> point = "ml_cairo_current_point"
external current_fill_rule : cr:t -> fill_rule = "ml_cairo_current_fill_rule"
external current_line_width : cr:t -> float = "ml_cairo_current_line_width"
external current_line_cap : cr:t -> line_cap = "ml_cairo_current_line_cap"
external current_line_join : cr:t -> line_join = "ml_cairo_current_line_join"
external current_miter_limit : cr:t -> float = "ml_cairo_current_miter_limit"
external current_matrix :
  cr:t -> matrix:matrix -> unit = "ml_cairo_current_matrix"
external current_target_surface :
  cr:t -> surface = "ml_cairo_current_target_surface"
external surface_create_for_image :
  image -> surface = "ml_cairo_surface_create_for_image"
external surface_create_similar :
  other:surface -> format:format -> width:int -> height:int ->
    surface = "ml_cairo_surface_create_similar"
external surface_destroy :
  surface:surface -> unit = "ml_cairo_surface_destroy"
external surface_set_repeat :
  surface:surface -> repeat:bool -> unit = "ml_cairo_surface_set_repeat"
external surface_set_matrix :
  surface:surface -> matrix:matrix -> unit = "ml_cairo_surface_set_matrix"
external surface_get_matrix :
  surface:surface -> matrix:matrix -> unit = "ml_cairo_surface_get_matrix"
type filter =
    FILTER_FAST
  | FILTER_GOOD
  | FILTER_BEST
  | FILTER_NEAREST
  | FILTER_BILINEAR
external surface_set_filter :
  surface:surface -> filter:filter -> unit = "ml_cairo_surface_set_filter"
external image_surface_create :
  format:format -> width:int -> height:int ->
    surface = "ml_cairo_image_surface_create"
external image_surface_create_for_data :
  image -> surface = "ml_image_surface_create_for_data"
external ps_surface_create :
  file:Cairo_channel.t -> width_inches:float -> height_inches:float ->
    x_pixels_per_inch:float -> y_pixels_per_inch:float ->
    surface = "ml_cairo_ps_surface_create"
external matrix_create : unit -> matrix = "ml_cairo_matrix_create"
external matrix_destroy : matrix:matrix -> unit = "ml_cairo_matrix_destroy"
external matrix_copy :
  matrix:matrix -> other:matrix -> unit = "ml_cairo_matrix_copy"
external matrix_set_identity :
  matrix:matrix -> unit = "ml_cairo_matrix_set_identity"
external matrix_set_affine :
  matrix:matrix -> float array -> unit = "ml_cairo_matrix_set_affine"
external matrix_get_affine :
  matrix:matrix -> float array = "ml_cairo_matrix_get_affine"
external matrix_translate :
  matrix:matrix -> tx:float -> ty:float -> unit = "ml_cairo_matrix_translate"
external matrix_scale :
  matrix:matrix -> sx:float -> sy:float -> unit = "ml_cairo_matrix_scale"
external matrix_rotate :
  matrix:matrix -> radians:float -> unit = "ml_cairo_matrix_rotate"
external matrix_invert : matrix:matrix -> unit = "ml_cairo_matrix_invert"
external matrix_multiply :
  result:matrix -> a:matrix -> b:matrix -> unit = "ml_cairo_matrix_multiply"
external matrix_transform_distance :
  matrix:matrix -> point -> unit = "ml_cairo_matrix_transform_distance"
external matrix_transform_point :
  matrix:matrix -> point -> unit = "ml_cairo_matrix_transform_point"
external finalise_target : cr:t -> unit = "ml_cairo_finalise_target"
external surface_finalise : surface -> unit = "ml_cairo_surface_finalise"

type flat_path = [
  | `MOVE_TO of point
  | `LINE_TO of point
  | `CLOSE ]
type basic_path = [
  | flat_path
  | `CURVE_TO of point * point * point ]
external fold_current_path      : t -> ('a -> [> basic_path] -> 'a) -> 'a -> 'a = "ml_cairo_current_path"
external fold_current_path_flat : t -> ('a -> [> flat_path] -> 'a) -> 'a -> 'a  = "ml_cairo_current_path_flat"
