OBJS	= main.cpp.o\
	  platformHelper.cpp.o \
	  ParseSAMP.cpp.o \
	  ApplyRules.cpp.o \
	  ArguementHandler/ArgHandle.cpp.o\
	  ArguementHandler/Arguement.cpp.o\
	  SimpleCppTextFileHandler/file.cpp.o \
	  SimpleCppTextFileHandler/fileManipulation.cpp.o \
	  SimpleCppTextFileHandler/getExecutablePath.cpp.o

OUT	= SAM
CXX	= g++
CC      = gcc
BUILD_CXX_FLAGS	 = -Wall -std=c++17 -g
BULID_CC_FLAGS   =
LINK_OPTS	 =

all: $(OBJS)
	$(CXX) $(OBJS) -o $(OUT) $(LINK_OPTS)


%.cpp.o: %.cpp
	$(CXX) $< $(BUILD_CXX_FLAGS) $(LINK_OPTS) -g -c -o $@

clean:
	rm -f $(OBJS) $(OUT)

install:
	cp $(OUT) /usr/local/bin/$(out)
