struct ml_cairo {
  cairo_t *cr;
  int suspend_exn;
};
#define cairo_t_val(v) (((struct ml_cairo *) Data_custom_val(v))->cr)
value Val_cairo_t (cairo_t *);

#define cairo_format_t_val(v) ((cairo_format_t) Int_val(v))
#define Val_cairo_format_t(v) Val_int(v)

#define cairo_surface_t_val(v) ((cairo_surface_t *)Pointer_val(v))
value Val_cairo_surface_t (cairo_surface_t *);

#define cairo_font_t_val(v) ((cairo_font_t *)Pointer_val(v))
value Val_cairo_font_t (cairo_font_t *);

#define cairo_matrix_t_val(v) ((cairo_matrix_t *)Pointer_val(v))
value Val_cairo_matrix_t (cairo_matrix_t *);
