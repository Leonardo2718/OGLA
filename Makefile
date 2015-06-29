# Compiler, tools, and options
CC			= gcc
CXX			= g++
CFLAGS		= -Wall
CXXFLAGS	= $(CFLAGS) -std=c++14
AR			= ar

# prerequisite files
HEADERS		= include/ogla/ogla.hpp
SOURCES		= src/ogla.cpp
OBJS		= $(subst src,bin,$(subst .cpp,.o,$(SOURCES)))

# Make rules

#all: ogla.a
#	$(info header files: ${HEADERS})
#	$(info source files: ${SOURCES})
#	$(info object files: ${OBJS})
#	$(info archives: $(ARCHIVES))

ogla.a: $(OBJS)
	$(AR) rvs "$@" $(OBJS)

bin/%.o: src/%.cpp $(HEADERS) Makefile
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@"

clean:
	rm $(OBJS)
