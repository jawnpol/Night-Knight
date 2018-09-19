#
# Makefile for asteroids game
#
# Enter one of the following
#
# make
# make all
# make asteroids
# make clean
#
CFLAGS = -I ./include
#LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm
LFLAGS = -lrt -lX11 -lGL

all: nightKnight

nightKnight: nightKnight.cpp log.cpp timers.cpp
	g++ $(CFLAGS) nightKnight.cpp log.cpp timers.cpp \
	libggfonts.a -Wall $(LFLAGS) -o nightKnight

clean:
	rm -f nightKnight
	rm -f *.o

