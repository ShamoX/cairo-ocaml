#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include "ml_cairo_wrappers.h"

int
ml_pointer_compare (value a, value b)
{
  void *p1 = Pointer_val (a);
  void *p2 = Pointer_val (b);
  if (p1 == p2)
    return 0;
  else if (p1 < p2)
    return -1;
  else
    return 1;
}

long
ml_pointer_hash (value a)
{
  void *p = Pointer_val (a);
  return (long) p;
}
