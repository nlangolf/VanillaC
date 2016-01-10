// TODO: Pull out networking code

#include <GLUT/glut.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "game_logger.h"

float camera_angle = 0.0f;
float red = 1.0f, blue = 1.0f, green = 1.0f;

// vector representing camera's direction
float lx = 0.0f, lz = -1.0f;

// position of camera
float x = 0.0f, z = 5.0f;

//
// Network
//

// TODO: Verify thread is created property
// TODO: Implement

#include <unistd.h>

void NetworkProcedure()
{
  while(1)
  {
    //LogFormat("nettt");
    sleep(1);
  }
}

void RunNetwork()
{
  NetworkProcedure();
  pthread_exit(NULL);
}

void BuildAndStartNetwork()
{
  pthread_t network_thread;
  const pthread_attr_t* NetworkThreadAttributes = NULL;
  void* network_routine_pointer = RunNetwork;
  int threadStatus = pthread_create(&network_thread, NetworkThreadAttributes, network_routine_pointer, NULL);
}
//
// End Network
//

void DrawGround()
{
  glColor3f(0.9f, 0.9f, 0.9f);
  glBegin(GL_QUADS);
   glVertex3f(-100.0f, 0.0f, -100.0f);
   glVertex3f(-100.0f, 0.0f,  100.0f);
   glVertex3f( 100.0f, 0.0f,  100.0f);
   glVertex3f( 100.0f, 0.0f, -100.0f);
  glEnd();
}

void DrawSnowMan()
{
  glColor3f(red, green, blue);

  // Draw Body
  glTranslatef(0.0f ,0.75f, 0.0f);
  glutSolidSphere(0.75f,20,20);

  // Draw Head
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.25f,20,20);

  // Draw Eyes
  glPushMatrix();
  glColor3f(0.0f,0.0f,0.0f);
  glTranslatef(0.05f, 0.10f, 0.18f);
  glutSolidSphere(0.05f,10,10);
  glTranslatef(-0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.05f,10,10);
  glPopMatrix();

  // Draw Nose
  glColor3f(1.0f, 0.5f , 0.5f);
  glutSolidCone(0.08f,0.5f,10,2);
}

void Render()
{
  // Clear color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glLoadIdentity();

  // Set camera
  gluLookAt(	x,    1.0f, z,
		x+lx, 1.0f, z+lz,
		0.0f, 1.0f, 0.0f);
  DrawGround();

  for (int i = -3; i < 3; i++)
  {
    for (int j = -3; j < 3; j++)
    {
      glPushMatrix();
      glTranslatef(i*10.0, 0.0, j*10.0);
      DrawSnowMan();
      glPopMatrix();
    }
  }

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
  //LogFormat("Angle is %f", angle);
}

void KeyboardCallback(unsigned char keyCode, int x, int y)
{
  const int EscapeKeyCode = 27;
  if (keyCode == EscapeKeyCode)
  {
    exit(0);
  }
}

void SpecialKeyboardCallback(int key, int x, int y)
{
  const float CameraUpDownSensitivity = 0.5f;
  const float CameraAngleSensitivity = 0.01f;
  switch(key)
  {
    case GLUT_KEY_F1 :
      red = 1.0;
      green = 0.0;
      blue = 0.0;
      LogFormat("f1 pressed");
      break;
    case GLUT_KEY_F2 :
      red = 0.0;
      green = 1.0;
      blue = 0.0;
      LogFormat("f2 pressed");
      break;
    case GLUT_KEY_F3 :
      red = 0.0;
      green = 0.0;
      blue = 1.0;
      LogFormat("f3 pressed");
      break;
    case GLUT_KEY_LEFT :
      camera_angle -= CameraAngleSensitivity;
      lx = sin(camera_angle);
      lz = -cos(camera_angle);
      break;
    case GLUT_KEY_RIGHT :
      camera_angle += CameraAngleSensitivity;
      lx = sin(camera_angle);
      lz = -cos(camera_angle);
      break;
    case GLUT_KEY_UP :
      x += lx * CameraUpDownSensitivity;
      z += lz * CameraUpDownSensitivity;
      break;
    case GLUT_KEY_DOWN :
      x -= lx * CameraUpDownSensitivity;
      z -= lz * CameraUpDownSensitivity;
      break;
  }

  LogFormat("x:%f z:%f lx:%f lz:%f", x, z, lx, lz);
}

void RunGlut()
{
  // init GLUT and create window
  int zero = 0;
  glutInit(&zero, NULL);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(320, 320);
  glutCreateWindow("Allo");

  // register callbacks
  glutDisplayFunc(DisplayCallback);
  glutReshapeFunc(ReshapeCallback);
  glutIdleFunc(IdleCallback);
  glutKeyboardFunc(KeyboardCallback);
  glutSpecialFunc(SpecialKeyboardCallback);

  // enter GLUT event processing loop
  glutMainLoop();
}

int main(int argc, char **argv)
{
  BuildAndStartLogger();
  BuildAndStartNetwork();
  RunGlut();
  pthread_exit(NULL);
  return 1;
}
