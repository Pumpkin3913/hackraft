# Pumpkin's

# (re)define default variables.
CC  = clang
CXX = clang
#CC = g++
#CC = gcc
#CXX = g++
CFLAGS   = -Wall -pedantic -g
CXXFLAGS = -Wall -pedantic -g -std=c++11
LDLIBS = -lpthread -lstdc++
$(RM) = rm -f

SRCS = main.cpp place.cpp player.cpp screen.cpp server.cpp tile.cpp error.cpp
OBJS = $(SRCS:.cpp=.o)

all : server

server : $(OBJS)

# server : $(OBJS)
#	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# %.o : %.cpp
#	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
# And how to use $(LIBS) ?

main.o : main.cpp server.h screen.h place.h tile.h player.h aspect.h

place.o : place.cpp place.h tile.h

player.o : player.cpp player.h aspect.h screen.h
# player.o : player.cpp player.h aspect.h screen.h object.h

screen.o : screen.cpp screen.h place.h player.h error.h

server.o : server.cpp server.h screen.h tile.h error.h

tile.o : tile.cpp tile.h aspect.h

error.o : error.cpp error.h

.PHONY : clean clear

clean :
	$(RM) *.o

clear : clean
	$(RM) server

