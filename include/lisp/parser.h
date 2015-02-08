#pragma once

#include <stdlib.h>
#include <memory>
#include <string>
#include <lisp/types.h>

namespace lisp
{
  void print(std::ostream& os, obj_t* o, int sc = 0)
  {
    if (o == nil) {
      return;
    }

    if (o->type == kCons)
    {
      if (sc > 0) os << "(";
      for (obj_t* r = o; r != nil && car(r) != nil; r = cdr(r)) {
          print(os, car(r), sc + 1);
        if(cdr(r) != nil)
          os << " ";
      }
      if (sc > 0) os << ")";
    }

    if (o->type == kLNum) { os << with_numl(o); }
    if (o->type == kDNum) { os << with_numd(o); }
    if (o->type == kSym ) { os << with_str(o); }
  }

  void to_string(std::ostringstream& buf, obj_t* o, int sc = 0)
  {
    if (o == nil) {
      return;
    }

    if (o->type == kCons)
    {
      if (sc > 0) buf << "(";
      for (obj_t* r = o; r != nil && car(r) != nil; r = cdr(r)) {
          to_string(buf, car(r), sc + 1);
        if(cdr(r) != nil)
          buf << " ";
      }
      if (sc > 0) buf << ")";
    }

    if (o->type == kLNum || o->type == kDNum)
    {
      if (o->type == kLNum)
        buf << with_numl(o);
      else
        buf << with_numd(o);
    }
    if (o->type == kSym ) { buf << with_str(o); }
  }

  inline bool not_parens(char c) { return c != '(' && c != ')'; }
  inline bool is_num(char t) { return isdigit(static_cast<unsigned char>(t)) != 0; }

  struct parser
  {
    parser() : prog(nil) {}
    obj_t* operator()(std::string const& file)
    {
      if (file.size() == 0) return prog;

      prog = alloc(kCons);

      with_car(prog) = alloc(kNone);
      with_cdr(prog) = nil;

      const char* f = parse(file.c_str(), prog);

      if (*f != '\0') {
        std::cout << std::endl << *f << " failed to parse." << std::endl;
      }

      return prog;
    }
  private:
    const char* parse(const char* file, obj_t* o)
    {
      const char* f = file;

      if (*f == '\0')  return f;

      obj_t* head = nil;
      obj_t* tail = nil;

      while (*f == ' ') { ++f; }

      if (*f == '\0') return f;

      head = with_car(o);

      if (!is_num(*f) && *f != ' ' && not_parens(*f)) {
        f = sym(f, head);
      }

      const char* n = f;
      if (is_num(*n) || (*f == '-' && is_num(*++n))) {
        f = num(f, head);
      }

      if (*f == '(') {
        obj_t* inner = head;
        head->type = kCons;
        with_car(inner) = alloc(kNone);
        with_cdr(inner) = nil;
        f = parse(++f, inner);
      }
      if (*f == ')') return ++f;
      if (*f != ')' && *f != '\0') {
        tail = alloc(kCons);
        with_car(tail) = alloc(kNone);
        with_cdr(tail) = nil;
        with_cdr(o) = tail;
        o = tail;
      }

      return parse(f, o);
    }

    inline const char* sym(const char* f, obj_t* s)
    {
      const char* y = f;
      while (*y != ' ' || !not_parens(*y)) { ++y; }

      s->type = kSym;

      std::string str(f, y);
      char* cpy = new char[str.size()+1];
      std::copy(str.begin(), str.end(), cpy);
      cpy[str.size()] = '\0';

      with_str(s) = cpy;

      return y;
    }
    inline const char* num(const char* f, obj_t* s)
    {
      int t = kLNum;
      const char* y = f;

      while (*y != ' ' && !not_parens(*y)) {
        if (*y == '.') { t = kDNum; }
        ++y;
      }

      s->type = t;

      if (t == kLNum) {
        with_numl(s) = std::strtol(f, (char**)&y, 10);
      } else {
        with_numd(s) = std::strtod(f, (char**)&y);
      }

      return y;
    }

    obj_t* prog;
  };
};
