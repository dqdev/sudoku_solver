.PHONY: default clean

CC=gcc
CXX=g++
CXXFLAGS= -std=c++11

ifeq ($(BUILD),OPT)
	CXXFLAGS+= -O3
endif

ifeq ($(BUILD),DEBUG)
    CXXFLAGS+= -g
endif

ifneq ($(NOWARN),1)
	CXXFLAGS+= -Wall
endif

INCLUDES+= -Isrc

LIBS=
#LIBS= -lboost_regex\
#      -lboost_date_time

SRCDIR=src
OBJDIR=objects

SOURCE=$(shell ls $(SRCDIR)/*.cpp)
OBJ= $(subst $(SRCDIR)/,$(OBJDIR)/,$(subst .cpp,.o,$(SOURCE)))

default: main.tsk

main.tsk: $(OBJ) 
	$(CXX) -o main.tsk $(OBJ) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) -o $@ $<

clean:
	rm -f $(OBJDIR)/*.o main.tsk

