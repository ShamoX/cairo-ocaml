#include <gtkcairo.h>

#include <caml/mlvalues.h>

#include "wrappers.h"
#include "ml_gobject.h"

#include "ml_cairo.h"

CAMLprim value
ml_cairo_gtkcairo_init (value unit)
{
  GType t = gtk_cairo_get_type ();
  return Val_GType (t);
}

#define GtkCairo_val(v)  check_cast(GTK_CAIRO, v)

CAMLprim value
ml_cairo_gtkcairo_get_cairo (value w)
{
  GtkCairo *c = GtkCairo_val (w);
  cairo_t *cr = gtk_cairo_get_cairo (c);
  cairo_reference (cr);
  return Val_cairo_t (cr);
}
