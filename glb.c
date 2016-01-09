#include <math.h>
#include <GLUT/glut.h>
#include <pthread.h>

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

void display_callback()
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

void RunGlut(int* argc, char**argv)
{
  InitializeGlut(argc, argv);
  InitializeAndCreateWindow();
  glutMainLoop();
}

int main(int argc, char** argv)
{
  RunGlut(&argc, argv);
  return 0;
}
