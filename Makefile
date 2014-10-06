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

SRCS = main.cpp place.cpp player.cpp screen.cpp server.cpp tile.cpp error.cpp luawrapper.cpp
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

