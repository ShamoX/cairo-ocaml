static inline cairo_format_t
cairo_format_t_val(value _v)
{
  const cairo_format_t _conv_tab[] =
    { CAIRO_FORMAT_ARGB32, CAIRO_FORMAT_RGB24, 
      CAIRO_FORMAT_A8, CAIRO_FORMAT_A1, };
  return _conv_tab[Int_val(_v)];
}

value Val_cairo_surface_t (cairo_surface_t *);
value Val_cairo_t (cairo_t *);
