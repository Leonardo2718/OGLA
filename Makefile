# Ccompiler, tools, and options
CXX			= g++
CXXFLAGS	= -Wall -std=c++14
AR			= ar

# prerequisite files
HEADERS		= include/ogla/ogla.hpp
SOURCES		= src/ogla.cpp
OBJS		= $(subst src,bin,$(subst .cpp,.o,$(SOURCES)))


# RULE USED FOR DEBUGING THIS MAKE FILE
#all: ogla.a
#	$(info header files: ${HEADERS})
#	$(info source files: ${SOURCES})
#	$(info object files: ${OBJS})
#	$(info archives: $(ARCHIVES))


# Make rules

ogla.a: $(OBJS)
	$(AR) rvs "$@" $(OBJS)

bin/%.o: src/%.cpp $(HEADERS) Makefile
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@"

clean:
	rm $(OBJS)
