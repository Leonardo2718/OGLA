# Compiler, tools, and options
CXX			= g++
CXXFLAGS	= -Wall -std=c++14

HEADERS		= ../include/ogla/ogla.hpp
ARCHIVES	= ../ogla.a

# make rules

all: test_lexer

test_%: test_%.cpp $(HEADERS) $(ARCHIVES) Makefile
	$(CXX) $(CXXFLAGS) "$<" $(ARCHIVES) -o "$@"
