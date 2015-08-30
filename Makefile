# Ccompiler, tools, and options
CXX			= g++
CXXFLAGS	= -Wall -std=c++14 -iquoteinclude/ogla
AR			= ar

# prerequisite files
HEADERS		= include/ogla/token.hpp include/ogla/grammar.hpp include/ogla/lexers.hpp
SOURCES		= src/token.cpp src/grammar.cpp src/lexers.cpp
OBJS		= $(subst src,bin,$(subst .cpp,.o,$(SOURCES)))


# RULE USED FOR DEBUGING THIS MAKE FILE
#all: ogla.a
#	$(info header files: ${HEADERS})
#	$(info source files: ${SOURCES})
#	$(info object files: ${OBJS})
#	$(info archives: $(ARCHIVES))


# Make rules

ogla.a: $(OBJS)
	$(AR) rcvs "$@" $(OBJS)

bin/%.o: src/%.cpp $(HEADERS) Makefile
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@"

clean:
	rm $(OBJS)
