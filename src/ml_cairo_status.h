void  cairo_treat_status(cairo_status_t);

#define Val_cairo_status_t(s)  (cairo_treat_status(s), Val_unit)
#define check_cairo_status(cr) cairo_treat_status(cairo_status(cairo_t_val(cr)))
#define report_null_pointer    cairo_treat_status(CAIRO_STATUS_NULL_POINTER)
