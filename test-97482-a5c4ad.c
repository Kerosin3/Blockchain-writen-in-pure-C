// #include <GLUT/glut.h>
//
// void display(void) {
//
//     //clear white, draw with black
//     glClearColor(50, 0, 50, 0);
//     glColor3d(0, 100, 0);
//
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//     //this draws a square using vertices
//     glBegin(GL_TRIANGLE_STRIP);
//     glVertex2i(0, 0);
//     glVertex2i(0, 128);
//     glVertex2i(128, 128);
//     glVertex2i(128, 0);
//     glEnd();
//
//     glutSwapBuffers();
//
// }
//
// void reshape(int width, int height) {
//
//     glViewport(0, 0, width, height);
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//
//     //set the coordinate system, with the origin in the top left
//     gluOrtho2D(0, width, height, 0);
//     glMatrixMode(GL_MODELVIEW);
//
// }
//
// void idle(void) {
//
//     glutPostRedisplay();
// }
//
// int main(int argc, char **argv) {
//
//     //a basic set up...
//     glutInit(&argc, argv);
//     glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
//     glutInitWindowSize(640, 480);
//
//     //create the window, the argument is the title
//     glutCreateWindow("OpenGL GLUT program");
//
//     //pass the callbacks
//     glutDisplayFunc(display);
//     glutReshapeFunc(reshape);
//     glutIdleFunc(idle);
//
//     glutMainLoop();
//
//     //we never get here because glutMainLoop() is an infinite loop
//     return 0;
//
// }





//==============================================================================

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
// #include <GLUT/GLUT.h>

int WindW, WindH;
int i;
int alpha;

void Reshape(int width, int height) // Reshape function
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // gluOrtho2D(-1, 1, -1, 1);
  glOrtho(-1, 1, -1, 1, 0, 10);
  glMatrixMode(GL_MODELVIEW);

  WindW = width;
  WindH = height;
}



void Draw(void) // Window redraw function
{

  glClear(GL_COLOR_BUFFER_BIT);

  glLineWidth(3);
  glColor3f(0.0f, 0.6f, 0.9f);

  glPushMatrix();
  glRotatef(alpha, 0.0f, 0.0f, 1.0f);
  alpha += 2;
  if (alpha > 359) alpha = 0;
  glBegin(GL_LINES);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    // glVertex2f(0.9f, 0.9f);
  glEnd();
  glPopMatrix();


  glFlush();
  glutSwapBuffers();



   // glClearColor(0.2f, 0.0f, 0.0f, 0.0f);
   // glClearDepth(1.0);
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glBegin(GL_TRIANGLES);
   //     glColor3f(0.0f, 0.0f, 0.0f);
   //     glVertex3f(-1., -1., 1.);
   //     glColor3f(1.0f, 1.0f, 1.0f);
   //     glVertex3f(0., 1., 0.);
   //     glColor3f(1.0f, 0.0f, 1.0f);
   //     glVertex3f(1., -1., 1.);
   //     glColor3f(1.0f, 0.0f, 1.0f);
   //     glVertex3f(1., -1., 1.);
   //     glColor3f(1.0f, 1.0f, 1.0f);
   //     glVertex3f(0., 1., 0.);
   //     glColor3f(1.0f, 0.0f, 0.0f);
   //     glVertex3f(1., -1., -1.);
   //     glColor3f(1.0f, 0.0f, 0.0f);
   //     glVertex3f(1., -1., -1.);
   //     glColor3f(1.0f, 1.0f, 1.0f);
   //     glVertex3f(0., 1., 0.);
   //     glColor3f(0.0f, 0.0f, 1.0f);
   //     glVertex3f(-1., -1., -1.);
   //     glColor3f(0.0f, 0.0f, 1.0f);
   //     glVertex3f(-1., -1., -1.);
   //     glColor3f(1.0f, 1.0f, 1.0f);
   //     glVertex3f(0., 1., 0.);
   //     glColor3f(1.0f, 1.0f, 0.0f);
   //     glVertex3f(-1., -1., 1.);
   // glEnd();
   // glPopMatrix();
   // glEnable(GL_DEPTH_TEST);
   //
   // glFlush();
   // glutSwapBuffers();
}

void Visibility(int state) // Visibility function
{
  if (state == GLUT_NOT_VISIBLE) printf("Window not visible!\n");
  if (state == GLUT_VISIBLE) printf("Window visible!\n");
}

void timf(int value) // Timer function
{
  glutPostRedisplay();  // Redraw windows
  glutTimerFunc(40, timf, 0); // Setup next timer
}

int main(int argc, char *argv[])
{
  WindW = 400;
  WindH = 300;
  alpha = 4;

  glutInit(&argc, argv);
  glutInitWindowSize(WindW, WindH);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  (void)glutCreateWindow("OTUS OpenGL");

  glutReshapeFunc(Reshape); // Set up reshape function
  glutDisplayFunc(Draw);    // Set up redisplay function
  glutTimerFunc(40, timf, 0); // Set up timer for 40ms, about 25 fps
  glutVisibilityFunc(Visibility); // Set up visibility funtion
  glClearColor(1, 1, 1, 0);

  glutMainLoop();

  return 0;
}
