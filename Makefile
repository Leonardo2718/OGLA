# Compiler, tools, and options
CC			= gcc
CXX			= g++
CFLAGS		= -Wall
CXXFLAGS	= $(CFLAGS) -std=c++14
AR			= ar

# Files
HEADERS		= src/ogla.hpp
SOURCES		= src/ogla.cpp
OBJS		= $(subst src,obj,$(subst .cpp,.o,$(SOURCES)))
#OBJS		= $(subst .cpp,.o,$(SOURCES))
#ARCHIVES	= ogla.a

# Make rules

ogla.a: $(OBJS)
	$(AR) rvs "$@" $(OBJS)

#all: $(ARCHIVES) $(OBJS)
#all: $(ARCHIVES)
#	$(info source files: ${SOURCES})
#	$(info object files: ${OBJS})
#	$(info archives: $(ARCHIVES))
#$(ARCHIVES): $(OBJS) Makefile

#$(OBJS): $(SOURCES) $(HEADERS) Makefile

obj/%.o: src/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o "$@" "$<"

#%.a: $(OBJS)
#	$(AR) rvs "$@" $(OBJS)

lexer_test: examples/lexer_test.cpp src/ogla.hpp ogla.a Makefile
	$(CXX) $(CXXFLAGS) -o "$@" examples/lexer_test.cpp ogla.a

clean:
	rm $(OBJS)
