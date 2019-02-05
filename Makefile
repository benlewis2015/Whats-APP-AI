PATH := /usr/um/gcc-6.2.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-6.2.0/lib64
LD_RUN_PATH := /usr/um/gcc-6.2.0/lib64

EXECUTABLE  = converse
DEBUG       = $(EXECUTABLE)_debug

CXX         = g++

TESTSOURCES = $(wildcard test*.cpp)

TESTS       = $(TESTSOURCES:%.cpp=%)

SOURCES     = $(wildcard *.cpp)
SOURCES     := $(filter-out $(TESTSOURCES), $(SOURCES))

OBJECTS     = $(SOURCES:%.cpp=%.o)

PROJECTFILE = $(or $(wildcard project*.cpp), $(EXECUTABLE).cpp)

PERF_FILE = perf.data*

CXXFLAGS = -std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic -O3

# make debug - will compile "all" with $(CXXFLAGS) and the -g flag
#              also defines DEBUG, so "#ifdef DEBUG /*...*/ #endif" works
debug: EXECUTABLE := $(DEBUG)
debug: CXXFLAGS += -g3 -DDEBUG
debug: clean $(EXECUTABLE)

# make profile - will compile "all" with $(CXXFLAGS) and the -pg flag
profile: CXXFLAGS += -pg
profile: clean $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
ifeq ($(EXECUTABLE), executable)
	@echo Edit EXECUTABLE variable in Makefile.
	@echo Using default a.out.
	$(CXX) $(CXXFLAGS) $(OBJECTS)
else
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)
endif

# rule for creating objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

# make clean - remove .o files, executables, tarball
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(DEBUG) $(TESTS) \
      $(PARTIAL_SUBMITFILE) $(FULL_SUBMITFILE) $(PERF_FILE)
	rm -Rf *.dSYM

help:
	@echo "$$MAKEFILE_HELP"

Responder.o: Responder.cpp Responder.h
