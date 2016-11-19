# Pumpkin's

# (re)define default variables.
#CC  = clang
CXX = clang
#CC = g++
#CC = gcc
#CXX = g++
DEBUG    = -DDEBUG -g 
ERROR    = -Wall -Werror -pedantic
#CFLAGS   = $(DEBUG) $(ERROR)
CXXFLAGS = $(DEBUG) $(ERROR) -std=c++14
LDLIBS = -lstdc++ -lpthread -llua
$(RM) = rm -f

SRCS = main.cpp player.cpp screen.cpp server.cpp tile.cpp place.cpp error.cpp luawrapper.cpp gauge.cpp tagged.cpp
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

