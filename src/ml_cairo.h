#define cairo_t_val(v) ((cairo_t *)Pointer_val(v))

static inline cairo_format_t
cairo_format_t_val(value _v)
{
  const cairo_format_t _conv_tab[] =
    { CAIRO_FORMAT_ARGB32, CAIRO_FORMAT_RGB24, 
      CAIRO_FORMAT_A8, CAIRO_FORMAT_A1, };
  return _conv_tab[Int_val(_v)];
}

static inline value
Val_cairo_format_t(cairo_format_t fmt)
{
  switch (fmt) {
  case CAIRO_FORMAT_ARGB32:
    return Val_int(0);
  case CAIRO_FORMAT_RGB24:
    return Val_int(1);
  case CAIRO_FORMAT_A8:
    return Val_int(2);
  case CAIRO_FORMAT_A1:
  default:
    return Val_int(3);
  }
}

value Val_cairo_surface_t (cairo_surface_t *);
value Val_cairo_t (cairo_t *);
