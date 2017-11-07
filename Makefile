#
# Makefile
#
#  Created on: Nov 4, 2017
#      Author: richard

#
# Project files
#

EXE  = PhilipsHueMonitor
EXE_T  = PhilipsHueMonitorTest

SRCS = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
SRCS := $(filter-out %_Test.cpp, $(SRCS))

SRCS_T = $(wildcard src/*_Test.cpp) $(wildcard src/**/*_Test.cpp)

OBJS = $(SRCS:.cpp=.o)
OBJS_T = $(OBJS) $(SRCS_T:.cpp=.o)
OBJS_T := $(filter-out src/PhilipsHueMonitor.o,$(OBJS_T))

#
# Flags
#
CXX	 = g++
CXXFLAGS = -g -Wall -Werror -Wextra -std=c++14 -Weffc++ -fmessage-length=0
LINKER_FLAGS = -lboost_system -lpthread -lssl -lcrypto
TSTLINKER_FLAGS = $(LINKER_FLAGS) -l:gtest_main.a -l:gmock_main.a

all:	$(EXE)
test:	$(EXE_T)

$(EXE):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $^ $(LINKER_FLAGS)

$(EXE_T):	$(OBJS_T)
	$(CXX) $(CXXFLAGS) -o $(EXE_T) $^ $(TSTLINKER_FLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<


clean:
	rm -f $(OBJS) $(OBJS_T) $(EXE) $(EXE_T)
