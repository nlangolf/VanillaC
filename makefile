all: glb.out receiver.out sender.out multi_receiver.out

glb.out: game.c logger.c logger.h game_logger.c game_logger.h network.h network.c
	gcc -g -o glb.out game.c logger.c game_logger.c network.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lGL -lGLU -lm -lobjc -lstdc++ -Wno-deprecated-declarations

receiver.out: receiver.c network.c network.h
	gcc receiver.c network.c -o receiver.out

sender.out: sender.c network.c network.h
	gcc sender.c network.c -o sender.out

multi_receiver.out: multi_receiver.c
	gcc multi_receiver.c -o multi_receiver.out

clean:
	rm glb.out
	rm -rf glb.out.dSYM
	rm receiver.out
	rm sender.out
	rm multi_receiver.out
