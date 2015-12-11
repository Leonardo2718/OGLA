# compiler, tools, and options
CXX			= g++
CXXFLAGS	= -Wall -std=c++14 -iquote../include

# prerequisite files
HEADERS		= ../include/ogla/ogla.hpp ../include/ogla/lexers.hpp
ARCHIVES	= /lib/libboost_unit_test_framework.a

# make rules

all: lexers_test

%_test: %_test.cpp $(HEADERS) $(ARCHIVES) Makefile
	$(CXX) $(CXXFLAGS) "$<" $(ARCHIVES) -o "$@"
