
(** Cairo Graphics *)

(** {3 Error reporting} *)

type status =
    NO_MEMORY
  | INVALID_RESTORE
  | INVALID_POP_GROUP
  | NO_CURRENT_POINT
  | INVALID_MATRIX
  | NO_TARGET_SURFACE
  | NULL_POINTER
exception Error of status
val init : unit

(** {3 Types} *)

type point = { mutable x : float ; mutable y : float }

type t
type surface
type matrix
type pattern
type image

type format = 
  | FORMAT_ARGB32 
  | FORMAT_RGB24 
  | FORMAT_A8 
  | FORMAT_A1

(** {3 Core API} *)

val create : unit -> t
external save    : cr:t -> unit = "ml_cairo_save"
external restore : cr:t -> unit = "ml_cairo_restore"
external _copy    : dest:t -> src:t -> unit = "ml_cairo_copy"
val copy : t -> t

external status : t -> status option = "ml_cairo_status"
external status_string : t -> string = "ml_cairo_status_string"

external suspend_exn : t -> unit = "ml_cairo_suspend_exn"
(** The functions operating on cairo values normally raise an [Error] exception 
    immediately if the operation fails. Calling [suspend_exn] will prevent this 
    automatic exception-raising. *)
external resume_exn  : t -> unit = "ml_cairo_resume_exn"
(** Switch back to exception-raising mode. If the cairo object has an error status, 
    an exception is raised right away. *)
external get_suspend_exn : t -> bool = "ml_cairo_get_suspend_exn"
(** Check the current exception-raising mode. *)

(** {4 Target functions} *)

external set_target_surface : cr:t -> surface:surface -> unit = "ml_cairo_set_target_surface"
external set_target_image   : cr:t -> image -> unit = "ml_cairo_set_target_image"
external set_target_ps :
  cr:t ->
  file:Cairo_channel.t ->
  width_inches:float ->
  height_inches:float ->
  x_pixels_per_inch:float -> y_pixels_per_inch:float -> unit
  = "ml_cairo_set_target_ps_bc" "ml_cairo_set_target_ps"
external set_target_pdf :
  cr:t ->
  file:Cairo_channel.t ->
  width_inches:float ->
  height_inches:float ->
  x_pixels_per_inch:float -> y_pixels_per_inch:float -> unit
  = "ml_cairo_set_target_pdf_bc" "ml_cairo_set_target_pdf"
external set_target_png :
  cr:t -> file:Cairo_channel.t -> format -> width:int -> height:int -> unit = "ml_cairo_set_target_png"
external finalise_target : cr:t -> unit = "ml_cairo_finalise_target"

(** {4 Renderer state} *)

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
  cr:t -> red:float -> green:float -> blue:float -> unit
  = "ml_cairo_set_rgb_color"
external set_pattern : cr:t -> pattern:pattern -> unit
  = "ml_cairo_set_pattern"
external set_alpha : cr:t -> alpha:float -> unit = "ml_cairo_set_alpha"
external set_tolerance : cr:t -> tolerance:float -> unit
  = "ml_cairo_set_tolerance"
type fill_rule = FILL_RULE_WINDING | FILL_RULE_EVEN_ODD
external set_fill_rule : cr:t -> fill_rule:fill_rule -> unit
  = "ml_cairo_set_fill_rule"
external set_line_width : cr:t -> width:float -> unit
  = "ml_cairo_set_line_width"
type line_cap = LINE_CAP_BUTT | LINE_CAP_ROUND | LINE_CAP_SQUARE
external set_line_cap : cr:t -> line_cap:line_cap -> unit
  = "ml_cairo_set_line_cap"
type line_join = LINE_JOIN_MITER | LINE_JOIN_ROUND | LINE_JOIN_BEVEL
external set_line_join : cr:t -> line_join:line_join -> unit
  = "ml_cairo_set_line_join"
external set_dash : cr:t -> dashes:float array -> offset:float -> unit
  = "ml_cairo_set_dash"
external set_miter_limit : cr:t -> limit:float -> unit
  = "ml_cairo_set_miter_limit"

(** {4 Transformations} *)

external translate : cr:t -> tx:float -> ty:float -> unit
  = "ml_cairo_translate"
external scale : cr:t -> sx:float -> sy:float -> unit = "ml_cairo_scale"
external rotate : cr:t -> angle:float -> unit = "ml_cairo_rotate"
external concat_matrix : cr:t -> matrix:matrix -> unit
  = "ml_cairo_concat_matrix"
external set_matrix : cr:t -> matrix:matrix -> unit = "ml_cairo_set_matrix"
external default_matrix : cr:t -> unit = "ml_cairo_default_matrix"
external identity_matrix : cr:t -> unit = "ml_cairo_identity_matrix"

external transform_point : cr:t -> point -> unit
  = "ml_cairo_transform_point"
external transform_distance : cr:t -> point -> unit
  = "ml_cairo_transform_distance"
external inverse_transform_point : cr:t -> point -> unit
  = "ml_cairo_inverse_transform_point"
external inverse_transform_distance : cr:t -> point -> unit
  = "ml_cairo_inverse_transform_distance"

(** {4 Paths} *)

external new_path : cr:t -> unit = "ml_cairo_new_path"
external move_to : cr:t -> x:float -> y:float -> unit = "ml_cairo_move_to"
val move_to_point : cr:t -> point -> unit
external line_to : cr:t -> x:float -> y:float -> unit = "ml_cairo_line_to"
val line_to_point : cr:t -> point -> unit
external curve_to :
  cr:t ->
  x1:float ->
  y1:float -> x2:float -> y2:float -> x3:float -> y3:float -> unit
  = "ml_cairo_curve_to_bc" "ml_cairo_curve_to"
val curve_to_point : cr:t -> point -> point -> point -> unit
external arc :
  cr:t ->
  xc:float ->
  yc:float -> radius:float -> angle1:float -> angle2:float -> unit
  = "ml_cairo_arc_bc" "ml_cairo_arc"
external arc_negative :
  cr:t ->
  xc:float ->
  yc:float -> radius:float -> angle1:float -> angle2:float -> unit
  = "ml_cairo_arc_negative_bc" "ml_cairo_arc_negative"
external rel_move_to : cr:t -> dx:float -> dy:float -> unit
  = "ml_cairo_rel_move_to"
external rel_line_to : cr:t -> dx:float -> dy:float -> unit
  = "ml_cairo_rel_line_to"
external rel_curve_to :
  cr:t ->
  dx1:float ->
  dy1:float -> dx2:float -> dy2:float -> dx3:float -> dy3:float -> unit
  = "ml_cairo_rel_curve_to_bc" "ml_cairo_rel_curve_to"
external rectangle :
  cr:t -> x:float -> y:float -> width:float -> height:float -> unit
  = "ml_cairo_rectangle"
external close_path : cr:t -> unit = "ml_cairo_close_path"
external stroke : cr:t -> unit = "ml_cairo_stroke"
external fill : cr:t -> unit = "ml_cairo_fill"

external in_stroke : cr:t -> x:float -> y:float -> bool = "ml_cairo_in_stroke"
external in_fill : cr:t -> x:float -> y:float -> bool = "ml_cairo_in_fill"

external stroke_extents : cr:t -> float * float * float * float = "ml_cairo_stroke_extents"
external fill_extents : cr:t -> float * float * float * float = "ml_cairo_fill_extents"

(** {3 Misc stuff I don't know how to categorize} *)

external show_surface :
  cr:t -> surface:surface -> width:int -> height:int -> unit
  = "ml_cairo_show_surface"
external copy_page : cr:t -> unit = "ml_cairo_copy_page"
external show_page : cr:t -> unit = "ml_cairo_show_page"
external init_clip : cr:t -> unit = "ml_cairo_init_clip"
external clip : cr:t -> unit = "ml_cairo_clip"

(** {3 Text API} *)

type font
type glyph = { index : int; glyph_x : float; glyph_y : float; }
type text_extents = { 
    x_bearing   : float ;
    y_bearing   : float ;
    text_width  : float ;
    text_height : float ;
    x_advance   : float ;
    y_advance   : float }
type font_extents = {
    ascent        : float;
    descent       : float;
    font_height   : float;
    max_x_advance : float;
    max_y_advance : float;
}
type font_weight = 
  | FONT_WEIGHT_NORMAL 
  | FONT_WEIGHT_BOLD
type font_slant = 
  | FONT_SLANT_NORMAL 
  | FONT_SLANT_ITALIC 
  | FONT_SLANT_OBLIQUE

external select_font :
  cr:t -> family:string -> slant:font_slant -> weight:font_weight -> unit
  = "ml_cairo_select_font"
external scale_font : cr:t -> scale:float -> unit = "ml_cairo_scale_font"
external transform_font : cr:t -> matrix:matrix -> unit
  = "ml_cairo_transform_font"
external show_text : cr:t -> utf8:string -> unit = "ml_cairo_show_text"
external show_glyphs : cr:t -> glyph array -> unit
  = "ml_cairo_show_glyphs"
external current_font : cr:t -> font = "ml_cairo_current_font"
external current_font_extents : cr:t -> font_extents
  = "ml_cairo_current_font_extents"
external set_font : cr:t -> font:font -> unit = "ml_cairo_set_font"
external text_extents : t -> utf8:string -> text_extents = "ml_cairo_text_extents"
external glyph_extents : t -> glyph array -> text_extents = "ml_cairo_glyph_extents"
external text_path : t -> utf8:string -> unit = "ml_cairo_text_path"
external glyph_path : t -> glyph array -> unit = "ml_cairo_glyph_path"

(** {4 Renderer state querying} *)

external current_operator : cr:t -> operator = "ml_cairo_current_operator"
external current_rgb_color : cr:t -> float * float * float
  = "ml_cairo_current_rgb_color"
external current_pattern : cr:t -> pattern = "ml_cairo_current_pattern"
external current_alpha : cr:t -> float = "ml_cairo_current_alpha"
external current_tolerance : cr:t -> float = "ml_cairo_current_tolerance"
external current_point : cr:t -> point = "ml_cairo_current_point"
external current_fill_rule : cr:t -> fill_rule = "ml_cairo_current_fill_rule"
external current_line_width : cr:t -> float = "ml_cairo_current_line_width"
external current_line_cap : cr:t -> line_cap = "ml_cairo_current_line_cap"
external current_line_join : cr:t -> line_join = "ml_cairo_current_line_join"
external current_miter_limit : cr:t -> float = "ml_cairo_current_miter_limit"
external current_matrix : cr:t -> matrix:matrix -> unit
  = "ml_cairo_current_matrix"
external current_target_surface : cr:t -> surface
  = "ml_cairo_current_target_surface"

type flat_path = [
  | `MOVE_TO of point
  | `LINE_TO of point
  | `CLOSE ]
type basic_path = [
  | flat_path
  | `CURVE_TO of point * point * point ]
external fold_current_path      : t -> ('a -> [> basic_path] -> 'a) -> 'a -> 'a = "ml_cairo_current_path"
external fold_current_path_flat : t -> ('a -> [> flat_path] -> 'a) -> 'a -> 'a  = "ml_cairo_current_path_flat"

(** {3 Surface API} *)

external surface_create_for_image : image -> surface
  = "ml_cairo_surface_create_for_image"
external surface_create_similar :
  other:surface -> format:format -> width:int -> height:int -> surface
  = "ml_cairo_surface_create_similar"
external surface_set_repeat : surface:surface -> repeat:bool -> unit
  = "ml_cairo_surface_set_repeat"
external surface_set_matrix : surface:surface -> matrix:matrix -> unit
  = "ml_cairo_surface_set_matrix"
external surface_get_matrix : surface:surface -> matrix:matrix -> unit
  = "ml_cairo_surface_get_matrix"
type filter =
    FILTER_FAST
  | FILTER_GOOD
  | FILTER_BEST
  | FILTER_NEAREST
  | FILTER_BILINEAR
  | FILTER_GAUSSIAN
external surface_set_filter : surface:surface -> filter:filter -> unit
  = "ml_cairo_surface_set_filter"
external surface_get_filter : surface:surface -> filter
  = "ml_cairo_surface_get_filter"
external surface_finalise : surface -> unit = "ml_cairo_surface_finalise"

(** {4 Pattern functions} *)

external pattern_create_for_surface : surface -> pattern = "ml_cairo_pattern_create_for_surface"

external pattern_create_linear : x0:float -> y0:float -> x1:float -> y1:float -> pattern 
  = "ml_cairo_pattern_create_linear"
external pattern_create_radial : 
  cx0:float -> cy0:float -> radius0:float ->
  cx1:float -> cy1:float -> radius1:float -> pattern 
  = "ml_cairo_pattern_create_radial_bc" "ml_cairo_pattern_create_radial"

external pattern_add_color_stop : 
  pattern -> offset:float -> 
  red:float -> green:float -> blue:float -> alpha:float -> unit 
  = "ml_cairo_pattern_add_color_stop_bc" "ml_cairo_pattern_add_color_stop"

external pattern_set_matrix : pattern -> matrix -> unit = "ml_cairo_pattern_set_matrix"
external pattern_get_matrix : pattern -> matrix -> unit = "ml_cairo_pattern_get_matrix"

type extend =
  | EXTEND_NONE
  | EXTEND_REPEAT
  | EXTEND_REFLECT
external pattern_set_extend : pattern -> extend -> unit = "ml_cairo_pattern_set_extend"
external pattern_get_extend : pattern -> extend = "ml_cairo_pattern_get_extend"
external pattern_set_filter : pattern -> filter -> unit = "ml_cairo_pattern_set_filter"
external pattern_get_filter : pattern -> filter = "ml_cairo_pattern_get_filter"

(** {4 Image surface} *)

external image_surface_create :
  format:format -> width:int -> height:int -> surface
  = "ml_cairo_image_surface_create"
external image_surface_create_for_data : image -> surface
  = "ml_image_surface_create_for_data"

(** {4 PS surface} *)

external ps_surface_create :
  file:Cairo_channel.t ->
  width_inches:float ->
  height_inches:float ->
  x_pixels_per_inch:float -> y_pixels_per_inch:float -> surface
  = "ml_cairo_ps_surface_create"

(** {4 PDF surface} *)

external pdf_surface_create :
  file:Cairo_channel.t ->
  width_inches:float ->
  height_inches:float ->
  x_pixels_per_inch:float -> y_pixels_per_inch:float -> surface
  = "ml_cairo_pdf_surface_create"

(** {4 PNG surface} *)

external png_surface_create :
  file:Cairo_channel.t -> format -> width:float -> height:float ->
    surface = "ml_cairo_png_surface_create"


(** {3 Matrix API} *)

external matrix_create : unit -> matrix = "ml_cairo_matrix_create"
external matrix_copy : matrix:matrix -> other:matrix -> unit
  = "ml_cairo_matrix_copy"
external matrix_set_identity : matrix:matrix -> unit
  = "ml_cairo_matrix_set_identity"
external matrix_set_affine : matrix:matrix -> float array -> unit
  = "ml_cairo_matrix_set_affine"
external matrix_get_affine : matrix:matrix -> float array
  = "ml_cairo_matrix_get_affine"

external matrix_translate : matrix:matrix -> tx:float -> ty:float -> unit
  = "ml_cairo_matrix_translate"
external matrix_scale : matrix:matrix -> sx:float -> sy:float -> unit
  = "ml_cairo_matrix_scale"
external matrix_rotate : matrix:matrix -> radians:float -> unit
  = "ml_cairo_matrix_rotate"
external matrix_invert : matrix:matrix -> unit = "ml_cairo_matrix_invert"
external matrix_multiply : result:matrix -> a:matrix -> b:matrix -> unit
  = "ml_cairo_matrix_multiply"

external matrix_transform_distance : matrix:matrix -> point -> unit
  = "ml_cairo_matrix_transform_distance"
external matrix_transform_point : matrix:matrix -> point -> unit
  = "ml_cairo_matrix_transform_point"

(** {3 Font API}

   Mostly unusable ATM. It needs other libraries (freetype2/fontconfig). 
*)

external font_set_transform : font:font -> matrix:matrix -> unit
  = "ml_cairo_font_set_transform"
external font_current_transform : font:font -> matrix:matrix -> unit
  = "ml_cairo_font_current_transform"

