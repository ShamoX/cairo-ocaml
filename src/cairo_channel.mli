(**************************************************************************)
(*  cairo-ocaml -- Objective Caml bindings for Cairo                      *)
(*  Copyright © 2004-2005 Olivier Andrieu                                 *)
(*                                                                        *)
(*  This code is free software and is licensed under the terms of the     *)
(*  GNU Lesser General Public License version 2.1 (the "LGPL").           *)
(**************************************************************************)

(** Support module for file-based backends (PostScript, PDF and PNG) *)

type t

val open_out : string -> t
external of_out_channel : out_channel -> t = "ml_FILE_of_channel"
external of_file_descr : Unix.file_descr -> t = "ml_FILE_of_file_descr"
external close : t -> unit = "ml_fclose"
