# compiler, tools, and options
CXX			= g++
#CXXFLAGS	= -Wall -std=c++14 -llibboost_unit_test_framework
CXXFLAGS	= -Wall -std=c++14

# prerequisite files
HEADERS		= ../include/ogla/ogla.hpp
#ARCHIVES	= ../ogla.a
ARCHIVES	= ../ogla.a /lib/libboost_unit_test_framework.a

# make rules

all: test_lexer test_firsttoken

test_%: test_%.cpp $(HEADERS) $(ARCHIVES) Makefile
	$(CXX) $(CXXFLAGS) "$<" $(ARCHIVES)  -o "$@"
#/lib/libboost_unit_test_framework.a
