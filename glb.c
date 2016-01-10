#include <math.h>
#include <GLUT/glut.h>
#include <pthread.h>
#include <stdio.h>

void SetDrawColor(float red_weight, float green_weight, float blue_weight)
{
  glColor3f(red_weight, green_weight, blue_weight);
}

void Draw1(int i)
{
  float red_weight = (float)i / 360.0; // TODO What is C Operator Precedence?
  SetDrawColor(red_weight, 1.0, 1.0);

  const int TreatEachPairOfVerticesAsAnIndependentLine = GL_LINES;
  glBegin(TreatEachPairOfVerticesAsAnIndependentLine);
  glVertex2d(cos(i/57.25779),sin(i/57.25779));
  glVertex2d(cos((i+90)/57.25779),sin((i+90)/57.25779));
  glEnd();
}

void Draw2(int i)
{
  float green_weight = (float)i / 360.0; // TODO What is C Operator Precedence?
  SetDrawColor(1.0, green_weight, 1.0);

  const int TreatEachPairOfVerticesAsAnIndependentLine = GL_LINES;
  glBegin(TreatEachPairOfVerticesAsAnIndependentLine);
  glVertex2d(cos(i/57.25779),sin(i/57.25779));
  glVertex2d(cos((i*2)/57.25779),sin((i+90)/57.25779));
  glEnd();
}

void DrawTriangle()
{
  const int TreatEachTripletOfVerticesAsAnIndependentTriangle = GL_TRIANGLES;
  glBegin(TreatEachTripletOfVerticesAsAnIndependentTriangle);
  glVertex2d(0.5, 1.0);
  glVertex2d(0.3, 0.5);
  glVertex2d(1.0, 0.5);
  glEnd();
}

void DisplayCallback()
{
  int current_degree;
  double angle;
  glClear(GL_COLOR_BUFFER_BIT);
  const int InitialDegree = 0;
  const int LastDegree = 359;
  const int DegreeIncrement = 4;
  for (current_degree = InitialDegree; current_degree <= LastDegree; current_degree += DegreeIncrement)
  {
    Draw1(current_degree);
    Draw2(current_degree);
  }

  DrawTriangle();
  glLoadIdentity();
  glutSwapBuffers();
}

void InitializeGlut()
{
  const int UseDoubleBufferedWindowFlag = GLUT_DOUBLE;
  const int UseDepthBufferFlag = GLUT_DEPTH; 

  // Initialize library and negotiate session with window system, with no arguments
  int zero = 0;
  int* zero_pointer = &zero;
  glutInit(zero_pointer, NULL);

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
  glutDisplayFunc(DisplayCallback);
}

void RunGlut()
{
  InitializeGlut();
  InitializeAndCreateWindow();
  glutMainLoop();
}

void *ThreadRunCallback(void* thread_id_pointer)
{
   const int GlutThreadId = 1;
   long thread_id = (long)thread_id_pointer;
   if(thread_id == GlutThreadId)
   {
     RunGlut();
   }
   else
   {
     printf("Hello from thread %ld\n", thread_id);
   }

   pthread_exit(NULL);
}

int main(int argc, char** argv)
{
  const int NumberOfThreads = 2;
  pthread_t threads[NumberOfThreads];
  for (long current_thread_id = 1; current_thread_id <= NumberOfThreads; current_thread_id++)
  {
    pthread_t current_thread = threads[current_thread_id];
    const pthread_attr_t* ThreadAttributes = NULL;
    void* start_routine = ThreadRunCallback;
    pthread_t* current_thread_pointer = &current_thread;
    void* current_thread_id_pointer = (void*)current_thread_id;
    pthread_create(current_thread_pointer, ThreadAttributes, start_routine, current_thread_id_pointer);
  }

  printf("Hello from the main thread!\n");
  pthread_exit(NULL);
  return 0;
}
