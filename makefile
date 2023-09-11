# Makefile for compiling txt2root with CERN ROOT

# Compiler
CXX = g++

# Source file
SRC = t2r_src.cxx

# Output executable name
TARGET = txt2root

# Compiler flags
CXXFLAGS = -std=c++11

# ROOT libraries and include directory
ROOTLIBS = $(shell root-config --libs)
ROOTINC = $(shell root-config --cflags)

# Build target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(ROOTINC) -o $(TARGET) $(SRC) $(ROOTLIBS)

# Clean rule
clean:
	rm -f $(TARGET)

# Default target
all: $(TARGET)

.PHONY: all clean

