#define Pointer_val(val) ((void*)Field(val,1))
#define Store_pointer(val, p) (Field(val, 1)=Val_bp(p))

int ml_pointer_compare (value, value);
long ml_pointer_hash (value);

#define Make_Val_final_pointer(type, init, final, adv) \
static void ml_final_##type (value val) \
{ if (Field(val,1)) final ((type*)Field(val,1)); } \
static struct custom_operations ml_custom_##type = \
{ #type "/001", ml_final_##type, ml_pointer_compare, \
  ml_pointer_hash, custom_serialize_default, custom_deserialize_default };\
value Val_##type (type *p) \
{ value ret; if (!p) report_null_pointer; \
  ret = alloc_custom (&ml_custom_##type, sizeof(value), adv, 1000); \
  p = init(p); \
  Field(ret,1) = Val_bp (p); return ret; }

static inline value Val_ptr(void *p)
{
  value v;
  v = alloc_small(2, Abstract_tag);
  Field(v, 1) = (value) p;
  return v;
}

#ifndef ARCH_ALIGN_DOUBLE
#define Double_array_val(v)    ((double *)(v))
#endif
#define Double_array_length(v) (Wosize_val(v) / Double_wosize)

#define Option_val(v,conv,def) (Is_long(v) ? def : conv(Field((v),0)))
#define StringOption_val(v) Option_val(v, String_val, NULL)

#define Ignore(x)
#define Unit(x) ((x), Val_unit)
#define Id(x) (x)

#define Unsupported(fun) \
CAMLprim value fun() { failwith("Unsupported backend"); return Val_unit; }

#define ML_0(cname, conv) \
CAMLprim value ml_##cname (value unit) { return conv (cname ()); }
#define ML_1(cname, conv1, conv) \
CAMLprim value ml_##cname (value arg1) { return conv (cname (conv1 (arg1))); }
#define ML_1_post(cname, conv1, conv, t, post) \
CAMLprim value ml_##cname (value arg1) \
{ t ret = cname (conv1(arg1)); post; return conv(ret); }
#define ML_2(cname, conv1, conv2, conv) \
CAMLprim value ml_##cname (value arg1, value arg2) \
{ return conv (cname (conv1(arg1), conv2(arg2))); }
#define ML_2_post(cname, conv1, conv2, t, conv) \
CAMLprim value ml_##cname (value arg1, value arg2) \
{ t ret = cname (conv1(arg1), conv2(arg2)); post; return conv(ret); }
#define ML_3(cname, conv1, conv2, conv3, conv) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3) \
{ return conv (cname (conv1(arg1), conv2(arg2), conv3(arg3))); }
#define ML_3_post(cname, conv1, conv2, conv3, conv, t, post) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3) \
{ t ret = cname (conv1(arg1), conv2(arg2), conv3(arg3)); post; return conv(t); }
#define ML_4(cname, conv1, conv2, conv3, conv4, conv) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3, value arg4) \
{ return conv (cname (conv1(arg1), conv2(arg2), conv3(arg3), conv4(arg4))); }
#define ML_4_post(cname, conv1, conv2, conv3, conv4, conv, t, post) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3, value arg4) \
{ t ret = cname (conv1(arg1), conv2(arg2), conv3(arg3), conv4(arg4)); post; return conv(ret); }
#define ML_5(cname, conv1, conv2, conv3, conv4, conv5, conv) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3, value arg4, \
                           value arg5) \
{ return conv (cname (conv1(arg1), conv2(arg2), conv3(arg3), conv4(arg4), \
		      conv5(arg5))); }
#define ML_5_post(cname, conv1, conv2, conv3, conv4, conv5, conv, t, post) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3, value arg4, \
                           value arg5) \
{ t ret = cname (conv1(arg1), conv2(arg2), conv3(arg3), conv4(arg4), conv5(arg5)); \
  post; return conv(ret); }
#define ML_6(cname, conv1, conv2, conv3, conv4, conv5, conv6, conv) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3, value arg4, \
                           value arg5, value arg6) \
{ return conv (cname (conv1(arg1), conv2(arg2), conv3(arg3), conv4(arg4), \
		      conv5(arg5), conv6(arg6))); }
#define ML_6_post(cname, conv1, conv2, conv3, conv4, conv5, conv6, conv, t, post) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3, value arg4, \
                           value arg5, value arg6) \
{ t ret = cname (conv1(arg1), conv2(arg2), conv3(arg3), conv4(arg4), \
		 conv5(arg5), conv6(arg6)); post; return conv(ret); }
#define ML_7(cname, conv1, conv2, conv3, conv4, conv5, conv6, conv7, conv) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3, value arg4, \
                           value arg5, value arg6, value arg7) \
{ return conv (cname (conv1(arg1), conv2(arg2), conv3(arg3), conv4(arg4), \
		      conv5(arg5), conv6(arg6), conv7(arg7))); }
#define ML_7_post(cname, conv1, conv2, conv3, conv4, conv5, conv6, conv7, conv, t, post) \
CAMLprim value ml_##cname (value arg1, value arg2, value arg3, value arg4, \
                           value arg5, value arg6, value arg7) \
{ t ret = cname (conv1(arg1), conv2(arg2), conv3(arg3), conv4(arg4), \
		      conv5(arg5), conv6(arg6), conv7(arg7)); post; return conv(ret); }


#define ML_bc6(cname) \
CAMLprim value ml_##cname##_bc (value *argv, int argn) \
{ return ml_##cname(argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]); }
#define ML_bc7(cname) \
CAMLprim value ml_##cname##_bc (value *argv, int argn) \
{ return ml_##cname(argv[0],argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]); }
