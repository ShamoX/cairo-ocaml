type image = {
    data   : Obj.t ;
    format : Cairo.format ;
    width  : int ;
    height : int ;
    stride : int ;
  }

external conv : image -> Cairo.image = "%identity"

open Bigarray

external bigarray_kind_float : ('a, 'b, c_layout) Array2.t -> bool
  = "bigarray_kind_float"
external bigarray_byte_size  : ('a, 'b, c_layout) Array2.t -> int
  = "ml_bigarray_byte_size"

external bigarray_data : ('a, 'b, c_layout) Array2.t -> Obj.t = "%field1"

let of_bigarr arr format ~width ~height ~stride =
  if bigarray_kind_float arr
  then invalid_arg "wrong Bigarray kind" ;
  if bigarray_byte_size arr < stride * height
  then invalid_arg "Bigarray too small" ;
  conv { data = bigarray_data arr ;
	 format = format ;
	 width = width ;
	 height = height ;
	 stride = stride }

let of_bigarr_32 ~alpha (arr : (int32, int32_elt, c_layout) Array2.t) =
  let w = Array2.dim2 arr in
  conv { data = bigarray_data arr ;
	 format = if alpha then Cairo.FORMAT_ARGB32 else Cairo.FORMAT_RGB24 ;
	 width  = w ;
	 height = Array2.dim1 arr ;
	 stride = 4 * w ;
       }

let of_bigarr_24 (arr : (int, int_elt, c_layout) Array2.t) =
  if Sys.word_size <> 32
  then failwith "your ints have 63 bits" ;
  let w = Array2.dim2 arr in
  conv { data = bigarray_data arr ;
	 format = Cairo.FORMAT_RGB24 ;
	 width  = w ;
	 height = Array2.dim1 arr ;
	 stride = 4 * w ;
       }

let of_bigarr_8 (arr : (int, int8_unsigned_elt, c_layout) Array2.t) =
  let w = Array2.dim2 arr in
  conv { data = bigarray_data arr ;
	 format = Cairo.FORMAT_A8 ;
	 width  = w ;
	 height = Array2.dim1 arr ;
	 stride = w ;
       }

let of_bigarr_1 (arr : (int, int8_unsigned_elt, c_layout) Array2.t) =
  let w = Array2.dim2 arr in
  conv { data = bigarray_data arr ;
	 format = Cairo.FORMAT_A1 ;
	 width  = w * 8 ;
	 height = Array2.dim1 arr ;
	 stride = w ;
       }

let output_pixel oc p =
  let r = (p lsr 16) land 0xff in
  output_byte oc r ;
  let g = (p lsr 8) land 0xff in
  output_byte oc g ;
  let b = p land 0xff in
  output_byte oc b 

let write_ppm_int32 oc (arr : (int32, int32_elt, c_layout) Array2.t) =
  let h = Array2.dim1 arr in
  let w = Array2.dim2 arr in
  Printf.fprintf oc "P6 %d %d 255\n" w h ;
  for i=0 to pred h do
    for j=0 to pred w do
      output_pixel oc (Int32.to_int arr.{i, j})
    done
  done ;
  flush oc

let write_ppm_int oc (arr : (int, int_elt, c_layout) Array2.t) =
  let h = Array2.dim1 arr in
  let w = Array2.dim2 arr in
  Printf.fprintf oc "P6 %d %d 255\n" w h ;
  for i=0 to pred h do
    for j=0 to pred w do
      output_pixel oc arr.{i, j}
    done
  done ;
  flush oc
