void cairo_treat_status (cairo_status_t);
void check_cairo_status (value cr);

#define Val_cairo_status_t(s)  (cairo_treat_status(s), Val_unit)
#define report_null_pointer    cairo_treat_status(CAIRO_STATUS_NULL_POINTER)
