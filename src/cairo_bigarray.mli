(** Support for the in-memory image backend, via Bigarray *)

open Bigarray

val of_bigarr :
  ('a, 'b, c_layout) Array2.t -> Cairo.format -> 
  width:int -> height:int -> stride:int -> Cairo.image

val of_bigarr_32 : alpha:bool -> (int32, int32_elt, c_layout) Array2.t -> Cairo.image
val of_bigarr_24 : (int, int_elt, c_layout) Array2.t -> Cairo.image
val of_bigarr_8  : (int, int8_unsigned_elt, c_layout) Array2.t -> Cairo.image
val of_bigarr_1  : (int, int8_unsigned_elt, c_layout) Array2.t -> Cairo.image

val write_ppm_int32 : out_channel -> (int32, int32_elt, c_layout) Array2.t -> unit
val write_ppm_int   : out_channel -> (int,   int_elt,   c_layout) Array2.t -> unit
