#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>  /* Header File For The GLut Library*/

void Draw1(int i)
{
  glColor3f((float)i/360.0,1.0,1.0);
  glBegin(GL_LINES);
  glVertex2d(cos(i/57.25779),sin(i/57.25779));
  glVertex2d(cos((i+90)/57.25779),sin((i+90)/57.25779));
  glEnd();
}

void Draw2(int i)
{
  glColor3f(1.0,(float)i/360.0,1.0);
  glBegin(GL_LINES);
  glVertex2d(cos(i/57.25779),sin(i/57.25779));
  glVertex2d(cos((i*2)/57.25779),sin((i+90)/57.25779));
  glEnd();
}

void display_callback()
{
  int i;
  double angle;
  glClear(GL_COLOR_BUFFER_BIT);
  for (i=0; i<360; i+=4)
  {
    Draw1(i);
    Draw2(i);
  }

  glLoadIdentity();
  glutSwapBuffers();
}

void InitializeGlut(int* argc, char**argv)
{
  const int UseDoubleBufferedWindowFlag = GLUT_DOUBLE;
  const int UseDepthBufferFlag = GLUT_DEPTH; 

  // Initialize library and negotiate session with window system
  glutInit(argc, argv);

  // Set the inital display mode
  glutInitDisplayMode (UseDoubleBufferedWindowFlag | UseDepthBufferFlag); 
}

void InitializeAndCreateWindow()
{
  // Set window attributes
  glutInitWindowSize (300,300);
  glutInitWindowPosition (100, 100);

  // Create a new top-level window
  glutCreateWindow ("Hellro"); 

  // Set display callback
  glutDisplayFunc(display_callback);
}

int main(int argc, char** argv)
{
  InitializeGlut(&argc, argv);
  InitializeAndCreateWindow();
  glutMainLoop();
  return 0;
}
