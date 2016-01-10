#include <GLUT/glut.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

//
// START LOGGING CODE
//
const int LogLineMaxLength = 50;
const int MaxPendingLogLines = 100;
int logger_lines_lost = 0;
int logger_inactive_cycles = 0;
struct LogLine
{
  char string[LogLineMaxLength];
};
struct LogLine logLines[MaxPendingLogLines];
int currentWriteLogLineIndex = 0;
int currentReadLogLineIndex = 0;

char* GetLine(int index)
{
  return logLines[index].string;
}

int ReadLinesExist()
{
  if (currentReadLogLineIndex == currentWriteLogLineIndex)
  {
    return 0;
  }

  return 1;
}

char* GetNextWriteLine()
{
  int nextWriteLogLineIndex = currentWriteLogLineIndex + 1;
  if(nextWriteLogLineIndex >= MaxPendingLogLines)
  {
    nextWriteLogLineIndex = 0;
  }

  if(nextWriteLogLineIndex == currentReadLogLineIndex)
  {
    logger_lines_lost++;
  }

  char* writeLine = GetLine(currentWriteLogLineIndex);
  currentWriteLogLineIndex = nextWriteLogLineIndex;
  return writeLine;
}

char* GetNextReadLine()
{
  char* readLine = GetLine(currentReadLogLineIndex);
  currentReadLogLineIndex++;
  if(currentReadLogLineIndex >= MaxPendingLogLines)
  {
    currentReadLogLineIndex = 0;
  }

  return readLine;
}

void LogFormat(const char *format, ...)
{
  va_list args;
  char* logLine = GetNextWriteLine();
  va_start(args, format);
  vsnprintf(logLine, LogLineMaxLength, format, args);
  va_end(args);
}

void RunLogger()
{
  while(1)
  {
    if(ReadLinesExist() == 0)
    {
      logger_inactive_cycles++;
      printf("LOG STATS: %d log lines lost; %d inactive cycles", logger_lines_lost, logger_inactive_cycles);
      sleep(1);
    }

    char* logLine = GetNextReadLine();
    printf("%s\n", logLine);
  }

  pthread_exit(NULL);
}

void BuildAndStartLogger()
{
  pthread_t log_thread;
  const pthread_attr_t* LogThreadAttributes = NULL;
  void* log_routine_pointer = RunLogger;
  int threadStatus = pthread_create(&log_thread, LogThreadAttributes, log_routine_pointer, NULL);
}
//
// END LOGGING CODE
//




float angle = 1.0f;

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
  LogFormat("Angle is %f", angle);
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

int main(int argc, char **argv)
{
  BuildAndStartLogger();
  RunGlut();
  pthread_exit(NULL);
  return 1;
}
