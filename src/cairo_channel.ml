(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

type t
external of_out_channel : out_channel -> t = "ml_FILE_of_channel"
external of_file_descr : Unix.file_descr -> t = "ml_FILE_of_file_descr"
external close : t -> unit = "ml_fclose"

let open_out fname =
  let oc = Pervasives.open_out fname in
  let c = of_out_channel oc in
  close_out oc ;
  c
