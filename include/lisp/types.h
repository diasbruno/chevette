#pragma once

#define with_numl(X) ((X)->obj.num.l)
#define with_numd(X) ((X)->obj.num.d)
#define with_str(X) ((X)->obj.str)
#define with_car(X) ((X)->obj.cons.car)
#define with_cdr(X) ((X)->obj.cons.cdr)

namespace lisp
{
  enum { kNone, kLNum, kDNum, kStr, kSym, kCons, kQuote };

  struct obj_t
  {
    int type;
    union {
      union { long l; double d; } num;
      const char* str;
      struct { struct obj_t* car; struct obj_t* cdr; } cons;
    } obj;
  };

  static obj_t* nil = NULL;
  static obj_t* t = NULL;
  static obj_t* f = NULL;

  inline obj_t* alloc(int type)
  {
    obj_t* o = (obj_t*)malloc(sizeof(obj_t));
    o->type = type;
    if (type == kStr || type == kSym) {
      o->obj.str = NULL;
    }
    if (type == kCons) {
      o->obj.cons.car = nil;
      o->obj.cons.cdr = nil;
    }
    return o;
  }

  inline obj_t* car(obj_t* cons) { return cons->obj.cons.car; }
  inline obj_t* cdr(obj_t* cons) { return cons->obj.cons.cdr; }

  struct func
  {
    typedef obj_t*(*fn)(obj_t*);
    func(fn fn) : _fn(fn) {}
    func() : _fn(NULL) {}
    func(func const& fn) : _fn(fn._fn) {}
    func const& operator= (func const& rhs) {
      _fn = rhs._fn;
      return *this;
    }
    obj_t* operator()(obj_t* args) {
      return _fn(args);
    }
    fn _fn;
  };
};
