class cairo : Cairo.t ->
  object
    inherit Ocairo.cairo
    method set_target_drawable : [`drawable] Gobject.obj -> unit 
  end

val cairo : ?target:([> `drawable] Gobject.obj) -> unit -> cairo

val surface_create_for_drawable : 
    [> `drawable] Gobject.obj -> Cairo.format -> Ocairo.surface
