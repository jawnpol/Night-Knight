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

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp zakaryW.cpp zacharyK.cpp brandonB.cpp johnpaulC.cpp 
	g++ $(CFLAGS) asteroids.cpp zakaryW.cpp zacharyK.cpp brandonB.cpp johnpaulC.cpp  \
		log.cpp timers.cpp /usr/lib/x86_64-linux-gnu/libopenal.so \
    	/usr/lib/x86_64-linux-gnu/libalut.so libggfonts.a -Wall $(LFLAGS) -o asteroids

clean:
	rm -f asteroids
	rm -f *.o

