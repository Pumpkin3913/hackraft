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
CXXFLAGS = $(DEBUG) $(ERROR) -std=c++17
LDLIBS = -lstdc++ -lpthread -llua
$(RM) = rm -f

SRCS = artifact.cpp aspect.cpp character.cpp gauge.cpp inventory.cpp log.cpp luawrapper.cpp main.cpp name.cpp place.cpp player.cpp script.cpp server.cpp tag.cpp uuid.cpp zone.cpp
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

