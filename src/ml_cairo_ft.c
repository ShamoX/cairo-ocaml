/**************************************************************************/
/*  cairo-ocaml -- Objective Caml bindings for Cairo                      */
/*  Copyright © 2004-2005 Olivier Andrieu                                 */
/*                                                                        */
/*  This code is free software and is licensed under the terms of the     */
/*  GNU Lesser General Public License version 2.1 (the "LGPL").           */
/**************************************************************************/

#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/custom.h>
#include <caml/callback.h>

#include "ml_cairo_wrappers.h"

#include <cairo.h>
#ifdef CAIRO_HAS_FT_FONT
# include <cairo-ft.h>
#endif

#include "ml_cairo.h"
#include "ml_cairo_status.h"

#ifdef CAIRO_HAS_FT_FONT

/* minimal Freetype interface */
static void
ml_raise_FT_Error (FT_Error err)
{
  static value *caml_exn;
  if (err == FT_Err_Ok)
    return;

  if (caml_exn == NULL)
    {
      caml_exn = caml_named_value ("FT_exn");
      if (caml_exn == NULL)
	failwith ("freetype error");
    }

  raise_with_arg (*caml_exn, Val_int (err));
}

#define FT_Library_val(v) (FT_Library)Pointer_val(v)

CAMLprim value
ml_FT_Init_FreeType (value unit)
{
  FT_Library lib;
  ml_raise_FT_Error (FT_Init_FreeType (&lib));
  return Val_ptr (lib);
}

CAMLprim value
ml_FT_Done_FreeType (value lib)
{
  ml_raise_FT_Error (FT_Done_FreeType (FT_Library_val (lib)));
  return Val_unit;
}

#define FT_Face_val(v) (FT_Face)Pointer_val(v)

CAMLprim value
ml_FT_New_Face (value lib, value o_index, value path)
{
  FT_Face face;
  FT_Long index = Option_val(o_index, Long_val, 0);
  ml_raise_FT_Error (FT_New_Face (FT_Library_val (lib),
				  String_val (path),
				  index, &face));
  return Val_ptr (face);
}

CAMLprim value
ml_FT_Done_Face (value face)
{
  ml_raise_FT_Error (FT_Done_Face (FT_Face_val (face)));
  return Val_unit;
}

/* minimal Fontconfig interface */
Make_Val_final_pointer (FcPattern, Id, FcPatternDestroy, 10)
#define FcPattern_val(v) (FcPattern *)Pointer_val(v)

ML_1 (FcNameParse, String_val, Val_FcPattern)

CAMLprim value
ml_FcNameUnparse (value patt)
{
  FcChar8 *s;
  value r;
  s = FcNameUnparse (FcPattern_val (patt));
  if (s == NULL)
    failwith ("FcNameUnparse");
  r = copy_string (s);
  free (s);
  return r;
}

/* cairo Fontconfig/Freetype font backend */
ML_2 (cairo_ft_font_create, FcPattern_val, cairo_matrix_t_val, Val_cairo_font_t)
ML_3 (cairo_ft_font_create_for_ft_face, FT_Face_val, Int_val, cairo_matrix_t_val, Val_cairo_font_t)
ML_1 (cairo_ft_font_lock_face, cairo_font_t_val, Val_ptr)
ML_1 (cairo_ft_font_unlock_face, cairo_font_t_val, Unit)
CAMLprim value
ml_cairo_ft_font_get_pattern (value font)
{
  FcPattern *p;
  p = cairo_ft_font_get_pattern (cairo_font_t_val (font));
  if (p == NULL)
    failwith ("cairo_ft_font_get_pattern: NULL pointer");
  FcPatternReference (p);
  return Val_FcPattern (p);
}

#else

Unsupported (ml_FT_Init_FreeType)
Unsupported (ml_FT_Done_FreeType)
Unsupported (ml_FT_New_Face)
Unsupported (ml_FT_Done_Face)
Unsupported (ml_FcNameParse)
Unsupported (ml_FcNameUnparse)
Unsupported (ml_cairo_ft_font_create)
Unsupported (ml_cairo_ft_font_create_for_ft_face)
Unsupported (ml_cairo_ft_font_lock_face)
Unsupported (ml_cairo_ft_font_unlock_face)
Unsupported (ml_cairo_ft_font_get_pattern)

#endif /* CAIRO_HAS_FT_FONT */
