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

nightKnight: nightKnight.cpp log.cpp timers.cpp zakaryW.cpp zacharyK.cpp brandonB.cpp johnpaulC.cpp 
	g++ $(CFLAGS) nightKnight.cpp zakaryW.cpp zacharyK.cpp brandonB.cpp johnpaulC.cpp  \
		log.cpp timers.cpp /usr/lib/x86_64-linux-gnu/libopenal.so \
    		/usr/lib/x86_64-linux-gnu/libalut.so libggfonts.a -Wall -Wextra -Wredundant-decls \
		-Winvalid-pch -Wformat=2 -Wmissing-format-attribute \
		-Wcast-align -Wswitch-enum -Wswitch-default \
	       	-Wformat-nonliteral -Wodr -flto -O3 $(LFLAGS) -o nightKnight

clean:
	rm -f nightKnight 
	rm -f *.o

