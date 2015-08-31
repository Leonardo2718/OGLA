# compiler, tools, and options
CXX			= g++
CXXFLAGS	= -Wall -std=c++14 -iquote../include

# prerequisite files
HEADERS		= ../include/ogla/ogla.hpp
ARCHIVES	= ../ogla.a /lib/libboost_unit_test_framework.a

# make rules

all: test_firsttoken test_analyze

test_%: test_%.cpp $(HEADERS) $(ARCHIVES) Makefile
	$(CXX) $(CXXFLAGS) "$<" $(ARCHIVES) -o "$@"

%_test: %_test.cpp $(HEADERS) $(ARCHIVES) Makefile
	$(CXX) $(CXXFLAGS) "$<" $(ARCHIVES) -o "$@"
