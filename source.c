#include <GLUT/glut.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

const int LogLineMaxLength = 50;
const int MaxPendingLogLines = 500;
float angle = 0.0f;

struct LogLine
{
  char string[LogLineMaxLength];
};
struct LogLine logLines[MaxPendingLogLines];
int currentWriteLogLineIndex = 0;
int currentReadLogLineIndex = 0;

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
  angle += 0.1f;
  Render();
  snprintf(logLines[currentWriteLogLineIndex].string, LogLineMaxLength, "Angle is %f", angle);
  currentWriteLogLineIndex++;
  if(currentWriteLogLineIndex >= MaxPendingLogLines - 1)
  {
    currentWriteLogLineIndex = 0;
    printf("re: %d\n", currentWriteLogLineIndex);
  }
}

void KeyboardCallback(unsigned char keyCode, int x, int y)
{
  const int EscapeKeyCode = 27;
  if (keyCode == EscapeKeyCode)
  {
    exit(0);
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

  // enter GLUT event processing loop
  glutMainLoop();
}

void RunLogger()
{
  while(1)
  {
    char* logLine = logLines[currentReadLogLineIndex].string;
    printf("%s\n", logLine);
    currentReadLogLineIndex++;
    sleep(1);
  }
  pthread_exit(NULL);
}

void BuildAndStartLogger()
{
  pthread_t log_thread;
  pthread_t* log_thread_pointer = &log_thread;
  const pthread_attr_t* LogThreadAttributes;
  void* log_routine_pointer = RunLogger;
  pthread_create(log_thread_pointer, LogThreadAttributes, log_routine_pointer, NULL);
}

int main(int argc, char **argv)
{
  BuildAndStartLogger();
  RunGlut();
  pthread_exit(NULL);
  return 1;
}
