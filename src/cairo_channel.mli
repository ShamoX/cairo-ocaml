(** Support module for file-based backends (PostScript and PNG) *)

type t
external of_out_channel : out_channel -> t = "ml_FILE_of_channel"
external of_file_descr : Unix.file_descr -> t = "ml_FILE_of_file_descr"
external close : t -> unit = "ml_fclose"
