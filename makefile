glb.out: glb.c
	gcc -o glb.out glb.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lGL -lGLU -lm -lobjc -lstdc++ -Wno-deprecated-declarations

clean:
	rm glb.out
