/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include <caml/bigarray.h>

unsigned long bigarray_byte_size (struct caml_bigarray *);

CAMLprim value
ml_bigarray_byte_size (value b)
{
  return Val_long (bigarray_byte_size (Bigarray_val (b)));
}

CAMLprim value
bigarray_kind_float (value v)
{
  struct caml_bigarray *ba = Bigarray_val (v);

  switch (ba->flags & BIGARRAY_KIND_MASK)
    {
    case BIGARRAY_FLOAT32:
    case BIGARRAY_FLOAT64:
    case BIGARRAY_COMPLEX32:
    case BIGARRAY_COMPLEX64:
      return Val_true;
    default:
      return Val_false;
    }
}
