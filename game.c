// TODO: Pull out networking code
// TODO: Fix draw order

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

float enemySnowmanX = 0.0f;
float enemySnowmanY = 0.0f;

// key states (zero when nothing is pressed)
float deltaAngle = 0.0f;
float deltaMove = 0.0f;

//
// Network
//

// TODO: Verify thread is created property
// TODO: Implement

#include <unistd.h>
#include <arpa/inet.h>
#include "network.h"

// TODO: Rename 'Receive'
// TODO: Send/Receive snapshot each time
void NetworkProcedure()
{
  int sock = BuildSocket();
  struct sockaddr_in addr = BuildAddress();
  Receive(addr, sock);
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

void DrawSnowman()
{
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

void DrawEnemySnowman()
{
  glColor3f(1.0f, 0, 0);
  DrawSnowman();
}

void computePos()
{
  x += deltaMove * lx * 0.1f;
  z += deltaMove * lz * 0.1f;
}

void computeDir()
{
  camera_angle += deltaAngle;
  lx = sin(camera_angle);
  lz = -cos(camera_angle);
}

void ApplyHeldKeys()
{
  if(deltaMove)
  {
    computePos();
  }
  if(deltaAngle)
  {
    computeDir();
  }
}

void ApplyOtherMovement()
{
  enemySnowmanX += 0.001f;
  enemySnowmanY += 0.001f;
}

void ReceiveCallback(char* network_address, void* message)
{
  LogFormat("%s, %s", network_address, message);
  ApplyOtherMovement();
}

void Render()
{
  ApplyHeldKeys();

  // Clear color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glLoadIdentity();

  // Set camera
  gluLookAt(	x,    1.0f, z,
		x+lx, 1.0f, z+lz,
		0.0f, 1.0f, 0.0f);
  DrawGround();

  glPushMatrix();
  glTranslatef(enemySnowmanX * 10.0, 0.0, enemySnowmanY * 10.0);
  DrawEnemySnowman();
  glPopMatrix();

  for (int i = -3; i < 3; i++)
  {
    for (int j = -3; j < 3; j++)
    {
      glPushMatrix();
      glTranslatef(i*10.0, 0.0, j*10.0);
      glColor3f(1, 1, 1);
      DrawSnowman();
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
}

void KeyboardCallback(unsigned char keyCode, int xx, int yy)
{
  const int EscapeKeyCode = 27;
  if (keyCode == EscapeKeyCode)
  {
    exit(0);
  }
}

void SpecialKeyDownCallback(int key, int xx, int yy)
{
  switch(key)
  {
    case GLUT_KEY_F1 :
      red = 1.0;
      green = 0.0;
      blue = 0.0;
      break;
    case GLUT_KEY_F2 :
      red = 0.0;
      green = 1.0;
      blue = 0.0;
      break;
    case GLUT_KEY_F3 :
      red = 0.0;
      green = 0.0;
      blue = 1.0;
      break;
    case GLUT_KEY_LEFT :
      deltaAngle = -0.01f;
      break;
    case GLUT_KEY_RIGHT :
      deltaAngle = 0.01f;
      break;
    case GLUT_KEY_UP :
      deltaMove = 0.5f;
      break;
    case GLUT_KEY_DOWN :
      deltaMove = -0.5f;
      break;
  }

  LogFormat("down x:%f z:%f lx:%f lz:%f", x, z, lx, lz);
}

void SpecialKeyUpCallback(int key, int xx, int yy)
{
  switch (key)
  {
    case GLUT_KEY_LEFT :
    case GLUT_KEY_RIGHT :
      deltaAngle = 0.0f;
      break;
    case GLUT_KEY_UP :
    case GLUT_KEY_DOWN :
      deltaMove = 0;
      break;
  }
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

  // We don't care about key-repeat because we look at up and down
  glutIgnoreKeyRepeat(1);
  glutSpecialFunc(SpecialKeyDownCallback);
  glutSpecialUpFunc(SpecialKeyUpCallback);

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
