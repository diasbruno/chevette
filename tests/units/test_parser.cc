#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <cctype>   /* isdigit */
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <lisp/parser.h>

using namespace lisp;

TEST_CASE("parse an empty file.", "[parser]") {
  parser p;
  std::string s = "";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse empty ().", "[parser]") {
  parser p;
  std::string s = "()";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse literal.", "[parser]") {
  parser p;
  std::string s = "1";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse literals without wrapper '()'.", "[parser]") {
  parser p;
  std::string s = "1 2";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse list with single element.", "[parser]") {
  parser p;
  std::string s = "(1)";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse list with more than one element.", "[parser]") {
  parser p;
  std::string s = "(1 2 3)";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse a list where car is a list.", "[parser]") {
  parser p;
  std::string s = "((1 2) 1)";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse a list where both car and cdr are lists.", "[parser]") {
  parser p;
  std::string s = "((1 2) (3 4))";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse list inside of lists.", "[parser]") {
  parser p;
  std::string s = "((1 2) (3 4)) ((1 2) (3 4))";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse a symbols.", "[parser]") {
  parser p;
  std::string s = "(de a (+ -1 1))";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}

TEST_CASE("parse a big list.", "[parser]") {
  parser p;
  std::string s = "add ((1 add 2) (3 4)) ((1 2) (3 4))";
  std::ostringstream ss;
  to_string(ss, p(s));
  REQUIRE(ss.str() == s);
}
