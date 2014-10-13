# Pumpkin's

# (re)define default variables.
CC  = clang
CXX = clang
#CC = g++
#CC = gcc
#CXX = g++
CFLAGS   = -DDEBUG -g -Wall -pedantic -g
CXXFLAGS = -DDEBUG -g -Wall -pedantic -g -std=c++11
LDLIBS = -lstdc++ -lpthread -llua
$(RM) = rm -f

SRCS = main.cpp player.cpp screen.cpp server.cpp tile.cpp place.cpp error.cpp luawrapper.cpp gauge.cpp object.cpp
OBJS = $(SRCS:.cpp=.o)
EXE = server

all : $(EXE)

$(EXE) : $(OBJS)

%.o : %.cpp %.h

.PHONY : clean clear

clean :
	$(RM) *.o

clear : clean
	$(RM) $(EXE)

