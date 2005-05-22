(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

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
  | INVALID_STRING
  | INVALID_PATH_DATA
  | READ_ERROR
  | WRITE_ERROR
  | SURFACE_FINISHED
  | SURFACE_TYPE_MISMATCH
  | BAD_NESTING
exception Error of status
val init : unit

(** {3 Types} *)

type t
type -'a surface
type -'a pattern
type font_face

type point = { x : float ; y : float }
type matrix = {
    xx : float ; yx : float ;
    xy : float ; yy : float ;
    x0 : float ; y0 : float 
  }

(** {3 Core API} *)

val create : 'a surface -> t
external save    : t -> unit = "ml_cairo_save"
external restore : t -> unit = "ml_cairo_restore"

external status : t -> status option = "ml_cairo_status"
external status_string : t -> string = "ml_cairo_status_string"

(** {4 Renderer state} *)

type operator =
    OPERATOR_CLEAR

  | OPERATOR_SOURCE
  | OPERATOR_OVER
  | OPERATOR_IN
  | OPERATOR_OUT
  | OPERATOR_ATOP

  | OPERATOR_DEST
  | OPERATOR_DEST_OVER
  | OPERATOR_DEST_IN
  | OPERATOR_DEST_OUT
  | OPERATOR_DEST_ATOP

  | OPERATOR_XOR
  | OPERATOR_ADD
  | OPERATOR_SATURATE

external set_operator : t -> operator -> unit = "ml_cairo_set_operator"

external set_source_rgb  : t -> red:float -> green:float -> blue:float -> unit = "ml_cairo_set_source_rgb"
external set_source_rgba : t -> red:float -> green:float -> blue:float -> alpha:float ->unit = "ml_cairo_set_source_rgba"
external set_source : t -> 'a pattern -> unit = "ml_cairo_set_source"
external set_source_surface : t -> 'a surface -> float -> float -> unit = "ml_cairo_set_source_surface"

external set_tolerance : t -> float -> unit = "ml_cairo_set_tolerance"

type fill_rule =
    FILL_RULE_WINDING
  | FILL_RULE_EVEN_ODD
external set_fill_rule : t -> fill_rule -> unit = "ml_cairo_set_fill_rule"
external set_line_width : t -> float -> unit = "ml_cairo_set_line_width"
type line_cap =
    LINE_CAP_BUTT
  | LINE_CAP_ROUND
  | LINE_CAP_SQUARE
external set_line_cap : t -> line_cap -> unit = "ml_cairo_set_line_cap"
type line_join =
    LINE_JOIN_MITER
  | LINE_JOIN_ROUND
  | LINE_JOIN_BEVEL
external set_line_join : t -> line_join -> unit = "ml_cairo_set_line_join"
external set_dash : t -> float array -> float -> unit = "ml_cairo_set_dash"
external set_miter_limit : t -> float -> unit = "ml_cairo_set_miter_limit"

(** {4 Transformations} *)

external translate : t -> tx:float -> ty:float -> unit = "ml_cairo_translate"
external scale : t -> sx:float -> sy:float -> unit = "ml_cairo_scale"
external rotate : t -> angle:float -> unit = "ml_cairo_rotate"
external transform : t -> matrix -> unit = "ml_cairo_trasnform"
external set_matrix : t -> matrix -> unit = "ml_cairo_set_matrix"
external identity_matrix : t -> unit = "ml_cairo_identity_matrix"

external user_to_device : t -> point -> point = "ml_cairo_user_to_device"
external user_to_device_distance : t -> point -> point = "ml_cairo_user_to_device_distance"
external device_to_user : t -> point -> point = "ml_cairo_device_to_user"
external device_to_user_distance : t -> point -> point = "ml_cairo_device_to_user_distance"

(** {4 Paths} *)

external new_path : t -> unit = "ml_cairo_new_path"
external move_to : t -> x:float -> y:float -> unit = "ml_cairo_move_to"
val move_to_point : t -> point -> unit
external line_to : t -> x:float -> y:float -> unit = "ml_cairo_line_to"
val line_to_point : t -> point -> unit
external curve_to : t -> x1:float -> y1:float -> x2:float -> y2:float -> x3:float -> y3:float -> unit = "ml_cairo_curve_to_bc" "ml_cairo_curve_to"
val curve_to_point : t -> point -> point -> point -> unit
external arc : t -> xc:float -> yc:float -> radius:float -> angle1:float -> angle2:float -> unit = "ml_cairo_arc_bc" "ml_cairo_arc"
external arc_negative : t -> xc:float -> yc:float -> radius:float -> angle1:float -> angle2:float -> unit = "ml_cairo_arc_negative_bc" "ml_cairo_arc_negative"
external rel_move_to : t -> dx:float -> dy:float -> unit = "ml_cairo_rel_move_to"
external rel_line_to : t -> dx:float -> dy:float -> unit = "ml_cairo_rel_line_to"
external rel_curve_to : t -> dx1:float -> dy1:float -> dx2:float -> dy2:float -> dx3:float -> dy3:float -> unit = "ml_cairo_rel_curve_to_bc" "ml_cairo_rel_curve_to"
external rectangle : t -> x:float -> y:float -> width:float -> height:float -> unit = "ml_cairo_rectangle"
external close_path : t -> unit = "ml_cairo_close_path"

external paint : t -> unit = "ml_cairo_paint"
external paint_with_alpha : t -> float -> unit = "ml_cairo_paint_with_alpha"

external mask : t -> 'a pattern -> unit = "ml_cairo_mask"
external mask_surface : t -> 'a surface -> surface_x:float -> surface_y:float -> unit = "ml_cairo_mask_surface"

external stroke : t -> unit = "ml_cairo_stroke"
external stroke_preserve : t -> unit = "ml_cairo_stroke_preserve"
external fill : t -> unit = "ml_cairo_fill"
external fill_preserve : t -> unit = "ml_cairo_fill_preserve"
external copy_page : t -> unit = "ml_cairo_copy_page"
external show_page : t -> unit = "ml_cairo_show_page"

external in_stroke : t -> point -> bool = "ml_cairo_in_stroke"
external in_fill : t -> point -> bool = "ml_cairo_in_fill"

external stroke_extents : t -> float * float * float * float = "ml_cairo_stroke_extents"
external fill_extents : t -> float * float * float * float = "ml_cairo_fill_extents"

external clip : t -> unit = "ml_cairo_clip"
external clip_preserve : t -> unit = "ml_cairo_clip_preserve"
external reset_clip : t -> unit = "ml_cairo_reset_clip"

(** {3 Text API} *)

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

external select_font_face : t -> string -> font_slant -> font_weight -> unit = "ml_cairo_select_font_face"
external set_font_size : t -> float -> unit = "ml_cairo_set_font_size"
external set_font_matrix : t -> matrix -> unit = "ml_cairo_set_font_matrix"
external get_font_matrix : t -> matrix = "ml_cairo_get_font_matrix"
external show_text : t -> string -> unit = "ml_cairo_show_text"
external show_glyphs : t -> glyph array -> unit = "ml_cairo_show_glyphs"
external get_font_face : t -> font_face = "ml_cairo_get_font_face"
external font_extents : t -> font_extents = "ml_cairo_font_extents"
external set_font_face : t -> font_face -> unit = "ml_cairo_set_font_face"
external text_extents : t -> string -> text_extents = "ml_cairo_text_extents"
external glyph_extents : t -> glyph array -> text_extents = "ml_cairo_glyph_extents"
external text_path : t -> string -> unit = "ml_cairo_text_path"
external glyph_path : t -> glyph array -> unit = "ml_cairo_glyph_path"

(** {4 Renderer state querying} *)

external get_operator : t -> operator = "ml_cairo_get_operator"
external get_source : t -> 'a pattern = "ml_cairo_get_source"
external get_tolerance : t -> float = "ml_cairo_get_tolerance"
external get_current_point : t -> point = "ml_cairo_get_current_point"
external get_fill_rule : t -> fill_rule = "ml_cairo_get_fill_rule"
external get_line_width : t -> float = "ml_cairo_get_line_width"
external get_line_cap : t -> line_cap = "ml_cairo_get_line_cap"
external get_line_join : t -> line_join = "ml_cairo_get_line_join"
external get_miter_limit : t -> float = "ml_cairo_get_miter_limit"
external get_matrix : t -> matrix = "ml_cairo_get_matrix"
external get_target : t -> 'a surface = "ml_cairo_get_target"

type flat_path = [
  | `MOVE_TO of point
  | `LINE_TO of point
  | `CLOSE ]
type path = [
  | flat_path
  | `CURVE_TO of point * point * point ]
external fold_path      : t -> ('a -> [> path] -> 'a) -> 'a -> 'a = "ml_cairo_copy_path"
external fold_path_flat : t -> ('a -> [> flat_path] -> 'a) -> 'a -> 'a  = "ml_cairo_copy_path_flat"

val append_path : t -> [< path] -> unit

(** {3 Surface API} *)

type format =
    FORMAT_ARGB32
  | FORMAT_RGB24
  | FORMAT_A8
  | FORMAT_A1

external surface_create_similar : 'a surface -> format -> width:int -> height:int -> 'a surface = "ml_cairo_surface_create_similar"

external surface_finish : 'a surface -> unit = "ml_cairo_surface_finish"

external surface_set_device_offset : 'a surface -> float -> float -> unit = "ml_cairo_surface_set_device_offset"

(** {4 Image surface} *)

type image_surface = [`Any|`Image] surface

external image_surface_create : format -> width:int -> height:int -> image_surface = "ml_cairo_image_surface_create"
external image_surface_get_width  : [>`Image] surface -> int = "ml_cairo_image_surface_get_width"
external image_surface_get_height : [>`Image] surface -> int = "ml_cairo_image_surface_get_height"

(** {4 Patterns} *)

type surface_pattern  = [`Any|`Surface] pattern
type gradient_pattern = [`Any|`Gradient] pattern

type extend = 
    EXTEND_NONE
  | EXTEND_REPEAT
  | EXTEND_REFLECT

type filter =
    FILTER_FAST
  | FILTER_GOOD
  | FILTER_BEST
  | FILTER_NEAREST
  | FILTER_BILINEAR
  | FILTER_GAUSSIAN

(** Patterns functions *)
module Pattern : sig
external create_for_surface : 'a surface -> surface_pattern = "ml_cairo_pattern_create_for_surface"
external create_linear : x0:float -> y0:float -> x1:float -> y1:float -> gradient_pattern = "ml_cairo_pattern_create_linear"
external create_radial : cx0:float -> cy0:float -> radius0:float -> cx1:float -> cy1:float -> radius1:float -> gradient_pattern = "ml_cairo_pattern_create_radial_bc" "ml_cairo_pattern_create_radial"

external add_color_stop_rgb  : [>`Gradient] pattern -> off:float -> red:float -> green:float -> blue:float -> unit = "ml_cairo_pattern_add_color_stop_rgb"
external add_color_stop_rgba : [>`Gradient] pattern -> off:float -> red:float -> green:float -> blue:float -> alpha:float -> unit = "ml_cairo_pattern_add_color_stop_rgba_bc" "ml_cairo_pattern_add_color_stop_rgba"

external set_matrix : 'a pattern -> matrix -> unit = "ml_cairo_pattern_set_matrix"
external get_matrix : 'a pattern -> matrix = "ml_cairo_pattern_get_matrix"

external set_extend : [> `Surface] pattern -> extend -> unit = "ml_cairo_pattern_set_extend"
external get_extend : [> `Surface] pattern -> extend = "ml_cairo_pattern_get_extend"

external set_filter : [> `Surface] pattern -> filter -> unit = "ml_cairo_pattern_set_filter"
external get_filter : [> `Surface] pattern -> filter = "ml_cairo_pattern_get_filter"
end

(** {3 Matrix API} *)

(** Matrix functions *)
module Matrix : sig
val init_identity : matrix
val init_translate : float -> float -> matrix
val init_scale : float -> float -> matrix
val init_rotate : float -> matrix

external translate : matrix -> float -> float -> matrix = "ml_cairo_matrix_translate"
external scale     : matrix -> float -> float -> matrix = "ml_cairo_matrix_scale"
external rotate    : matrix -> float -> matrix = "ml_cairo_matrix_rotate"
external invert    : matrix -> matrix = "ml_cairo_matrix_invert"
external multiply  : matrix -> matrix -> matrix = "ml_cairo_matrix_multiply"

external transform_distance : matrix -> point -> point = "ml_cairo_matrix_transform_distance"
external transform_point    : matrix -> point -> point = "ml_cairo_matrix_transform_point"
end

(** {3 Scaled Fonts API} *)

(** Scaled fonts functions *)
module Scaled_Font : sig
type t

external create : font_face -> matrix -> matrix -> t = "ml_cairo_scaled_font_create"
external font_extents : t -> font_extents = "ml_cairo_scaled_font_extents"
external glyph_extents : t -> glyph array -> text_extents = "ml_cairo_scaled_font_glyph_extents"
end
