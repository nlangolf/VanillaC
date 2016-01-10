#include <GLUT/glut.h>

float angle = 0.0f;

void Render()
{
  // Clear color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glLoadIdentity();

  // Set camera
  gluLookAt(	0.0f, 0.0f, 10.0f,
		0.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f);

  glRotatef(angle, 0.0f, 1.0f, 0.0f);

  glBegin(GL_TRIANGLES);
   glVertex3f(-2.0f, -2.0f, 0.0f);
   glVertex3f( 2.0f,  0.0f, 0.0f);
   glVertex3f( 0.0f,  2.0f, 0.0f);
  glEnd();

  angle += 0.1f;

  // We are done rendering into the back buffer, so let's bring it out front
  glutSwapBuffers();
}

float CalculateRatio(int width, int height)
{
  if (height == 0)
  {
    return width;
  }

  return ((float)width)/height;
}

void ReshapeCallback(int width, int height)
{
  float ratio = CalculateRatio(width, height);

  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);

  // Reset Matrix
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, width, height);

  // Set the correct perspective.
  gluPerspective(45, ratio, 1, 100);

  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
}

void DisplayCallback()
{
  Render();
}

void IdleCallback()
{
  Render();
}

int main(int argc, char **argv)
{
  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(320, 320);
  glutCreateWindow("Allo");

  // register callbacks
  glutDisplayFunc(DisplayCallback);
  glutReshapeFunc(ReshapeCallback);
  glutIdleFunc(IdleCallback);

  // enter GLUT event processing loop
  glutMainLoop();

  return 1;
}
