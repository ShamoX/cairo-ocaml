type status = Cairo.status =
    INVALID_RESTORE
  | INVALID_POP_GROUP
  | NO_CURRENT_POINT
  | INVALID_MATRIX
  | NO_TARGET_SURFACE
  | NULL_POINTER

exception Error of status

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
type line_cap = Cairo.line_cap =
    LINE_CAP_BUTT
  | LINE_CAP_ROUND
  | LINE_CAP_SQUARE
type line_join = Cairo.line_join =
    LINE_JOIN_MITER
  | LINE_JOIN_ROUND
  | LINE_JOIN_BEVEL
type glyph = Cairo.glyph = { index : int; x : float; y : float; }
type font_extents =
  Cairo.font_extents = {
  ascent : float;
  descent : float;
  height : float;
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

class cairo :
  Cairo.t ->
  object
    val c : Cairo.t
    method alpha : float
    method arc : float -> float -> float -> float -> float -> unit
    method arc_negative : float -> float -> float -> float -> float -> unit
    method clip : unit
    method close_path : unit
    method concat_matrix : matrix -> unit
    method copy : cairo
    method copy_page : unit
    method current_point : point
    method curve_to :
      float -> float -> float -> float -> float -> float -> unit
    method default_matrix : unit
    method fill : unit
    method fill_rule : Cairo.fill_rule
    method finalise_target_ps : cr:Cairo.t -> unit
    method font_extents : Cairo.font_extents
    method get_cairo : Cairo.t
    method identity_matrix : unit
    method in_stroke : float -> float -> bool
    method in_fill : float -> float -> bool
    method inverse_transform_distance : point -> unit
    method inverse_transform_point : point -> unit
    method line_cap : Cairo.line_cap
    method line_join : Cairo.line_join
    method line_to : float -> float -> unit
    method line_width : float
    method matrix : matrix
    method miter_limit : float
    method move_to : float -> float -> unit
    method new_path : unit
    method operator : Cairo.operator
    method rectangle : float -> float -> float -> float -> unit
    method rel_curve_to :
      float -> float -> float -> float -> float -> float -> unit
    method rel_line_to : float -> float -> unit
    method rel_move_to : float -> float -> unit
    method restore : unit
    method rgb_color : float * float * float
    method rotate : float -> unit
    method save : unit
    method scale : float -> float -> unit
    method scale_font : float -> unit
    method select_font :
      string -> Cairo.font_slant -> Cairo.font_weight -> unit
    method set_alpha : float -> unit
    method set_dash : float array -> float -> unit
    method set_fill_rule : Cairo.fill_rule -> unit
    method set_line_cap : Cairo.line_cap -> unit
    method set_line_join : Cairo.line_join -> unit
    method set_line_width : float -> unit
    method set_matrix : matrix -> unit
    method set_miter_limit : float -> unit
    method set_operator : Cairo.operator -> unit
    method set_pattern : surface -> unit
    method set_rgb_color : float -> float -> float -> unit
    method set_target_image : Cairo.image -> unit
    method set_target_ps :
      Cairo_channel.t -> float -> float -> float -> float -> unit
    method set_target_surface : surface -> unit
    method set_tolerance : float -> unit
    method show_glyphs : Cairo.glyph -> int -> unit
    method show_page : unit
    method show_surface : surface -> int -> int -> unit
    method show_text : string -> unit
    method stroke : unit
    method target_surface : surface
    method tolerance : float
    method transform_distance : point -> unit
    method transform_font : matrix -> unit
    method transform_point : point -> unit
    method translate : float -> float -> unit
  end


and matrix :
  object
    val m : Cairo.matrix
    method copy : matrix
    method get_affine : float array
    method get_matrix : Cairo.matrix
    method invert : matrix
    method multiply : matrix -> matrix
    method rotate : float -> matrix
    method scale : float -> float -> matrix
    method set_affine : float array -> unit
    method set_identity : unit
    method transform_distance : point -> unit
    method transform_point : point -> unit
    method translate : float -> float -> matrix
  end

and surface :
  Cairo.surface ->
  object
    val s : Cairo.surface
    method create_similar : Cairo.format -> int -> int -> surface
    method get_matrix : matrix
    method get_surface : Cairo.surface
    method set_filter : Cairo.filter -> unit
    method set_matrix : matrix -> unit
    method set_repeat : bool -> unit
    method ps_finalise : unit
  end

val cairo : unit -> cairo

val surface_create_for_image : Cairo.image -> surface
val image_surface_create : Cairo.format -> int -> int -> surface

val ps_surface_create :
  Cairo_channel.t -> float -> float -> float -> float -> surface
