#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/custom.h>

#include "ml_cairo_wrappers.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "caml_io.h"
#include "ml_cairo_channel.h"

static value
ml_FILE_of_fd (int fd)
{
  int new_fd;
  FILE *f;

  new_fd = dup (fd);
  if (new_fd < 0)
    goto fail;
  f = fdopen (new_fd, "w");
  if (!f)
    goto fail;
  return Val_ptr (f);

fail:
  raise_sys_error (copy_string (strerror (errno)));
}

CAMLprim value
ml_FILE_of_channel (value v)
{
  struct channel *c = Channel (v);
  flush (c);
  return ml_FILE_of_fd (c->fd);
}

CAMLprim value
ml_FILE_of_file_descr (value v)
{
#ifndef _WIN32
  return ml_FILE_of_fd (Int_val (v));
#else
  return failwith ("unsupported");
#endif
}

CAMLprim value
ml_fclose (value v)
{
  FILE *f = FILE_val (v);
  if (fclose (f))
    raise_sys_error (copy_string (strerror (errno)));
  return Val_unit;
}
