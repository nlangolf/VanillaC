glb.out: source.c
	gcc -o glb.out source.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lGL -lGLU -lm -lobjc -lstdc++ -Wno-deprecated-declarations

clean:
	rm glb.out
