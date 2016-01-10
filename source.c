#include <GLUT/glut.h>

void Render(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
   glVertex3f(-0.5,-0.5,0.0);
   glVertex3f(0.5,0.0,0.0);
   glVertex3f(0.0,0.5,0.0);
  glEnd();
  glutSwapBuffers();
}

void DisplayCallback()
{
  Render();
}

int main(int argc, char **argv)
{
  // init GLUT and create Window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(320,320);
  glutCreateWindow("Lighthouse3D - GLUT Tutorial");

  // register callbacks
  glutDisplayFunc(DisplayCallback);

  // enter GLUT event processing cycle
  glutMainLoop();

  return 1;
}
