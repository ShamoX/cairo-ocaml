(** Minimal support for the Fontconfig/Freetype font interface *)

exception FT_Error of int

type ft_library
type ft_face

val init_freetype : unit -> ft_library
external done_freetype : ft_library -> unit = "ml_FT_Done_FreeType"

external new_face : ft_library -> ?index:int -> string -> ft_face
  = "ml_FT_New_Face"
external done_face : ft_face -> unit = "ml_FT_Done_Face"

type fc_pattern
external fc_name_parse : string -> fc_pattern = "ml_FcNameParse"
external fc_name_unparse : fc_pattern -> string = "ml_FcNameUnparse"

external font_create : ft_library -> fc_pattern -> Cairo.font
  = "ml_cairo_ft_font_create"
external font_create_for_ft_face : ft_face -> Cairo.font
  = "ml_cairo_ft_font_create_for_ft_face"
external font_pattern : Cairo.font -> fc_pattern = "ml_cairo_ft_font_pattern"
