
class cairo cairo = 
  object
    inherit Ocairo.cairo cairo

    method set_target_drawable d =
      Cairo_lablgtk.set_target_drawable c (d : [`drawable] Gobject.obj)
  end

let cairo ?target () =
  let c = new cairo (Cairo.create ()) in
  match target with
  | None -> c
  | Some d -> c#set_target_drawable (d :> [`drawable] Gobject.obj) ; c

let surface_create_for_drawable d fmt =
  new Ocairo.surface (Cairo_lablgtk.surface_create_for_drawable d fmt)
