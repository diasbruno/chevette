CC=clang++

CLFAGS=-std=c++11
INCLUDES=-I./include -I./tests/catch/single_include
TESTS_UNITS_PATH=tests/units

test_parser: 
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_UNITS_PATH)/$@.cc -o $@

tests: test_parser
