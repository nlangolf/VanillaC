all: glb.out mcast.out

glb.out: game.c logger.c logger.h game_logger.c game_logger.h network.h network.c
	gcc -g -o glb.out game.c logger.c game_logger.c network.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lGL -lGLU -lm -lobjc -lstdc++ -Wno-deprecated-declarations

mcast.out: mcast.c network.c network.h
	gcc mcast.c network.c -o mcast.out

clean:
	rm glb.out
	rm -rf glb.out.dSYM
	rm mcast.out
