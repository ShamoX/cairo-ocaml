type status =
    NO_MEMORY
  | IO_ERROR
  | FILE_NOT_FOUND
  | INVALID_VALUE
  | INVALID_CALL
  | PARSE_ERROR
exception Error of status
let init = Callback.register "svg_cairo_status_exn" (Error NO_MEMORY)

type t

external create  : unit -> t = "ml_svg_cairo_create"

external parse        : t -> string -> unit = "ml_svg_cairo_parse"
external parse_string : t -> string -> unit = "ml_svg_cairo_parse_buffer"

external parse_chunk_begin : t -> unit = "ml_svg_cairo_parse_chunk_begin"
external parse_chunk       : t -> string -> unit = "ml_svg_cairo_parse_chunk"
external parse_chunk_end   : t -> unit = "ml_svg_cairo_parse_chunk_end"

external render : t -> Cairo.t -> unit = "ml_svg_cairo_render"

external set_viewport_dimenstion : t -> int -> int -> unit = "ml_svg_cairo_set_viewport_dimension"

external get_size  : t -> int * int = "ml_svg_cairo_get_size"
