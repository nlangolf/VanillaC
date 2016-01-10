all: glb.out mcast.out

glb.out: game.c logger.c logger.h game_logger.c game_logger.h
	gcc -g -o glb.out game.c logger.c game_logger.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lGL -lGLU -lm -lobjc -lstdc++ -Wno-deprecated-declarations

mcast.out: mcast.c
	gcc mcast.c -o mcast.out

clean:
	rm glb.out
	rm -rf glb.out.dSYM
	rm mcast.out
