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

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp zakaryW.cpp zacharyK.cpp brandonB.cpp johnpaulC.cpp sound.cpp
	g++ $(CFLAGS) asteroids.cpp zakaryW.cpp zacharyK.cpp brandonB.cpp johnpaulC.cpp log.cpp timers.cpp sound.cpp \
    	/usr/lib/x86_64-linux-gnu/libopenal.so \
    	/usr/lib/x86_64-linux-gnu/libalut.so libggfonts.a -Wall $(LFLAGS) -o asteroids

clean:
	rm -f asteroids
	rm -f *.o

