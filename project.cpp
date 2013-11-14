#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "ReadMidi.h"

#define MAXVAL 127

#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glut.h>
#  include <GL/glext.h>
#endif
using namespace std;


//Globals
static GLsizei width, height;
static float warp = 0.2; //The amount of warpage in a given visualization
static float up = 0.0; 
static int isAnimate = 0;
static int animateInterval = 50; //Time interval between frames.
static float pointSize = 3.0;

read_midi inputMidi;


vector < unsigned char > keyvals; 
vector < float > xvector; 
vector < float > yvector;

  
void animate(int value){
  
  

  /*if (isAnimate){
    xvector.push_back(0.0);
    yvector.push_back(0.0);
    for(int j = 0; j < xvector.size() - 1; j++){
      xvector[j] = xvector[j] + warp;
      if (yvector[j] > 0.0){
	yvector[j] -= 0.5;
      }
      if (yvector[j] < 0.0)
	yvector[j] = 0.0;

      if (up >= 0.0){
	yvector[j] += up;
	up -= up/yvector.size();
      }
      
      }

    if (sides > 25)
    sides = 4;
    else
    sides++;
    
    }*/

  keyvals = inputMidi.get_array();
  
  glutTimerFunc(animateInterval, animate, 1);
  glutPostRedisplay();
}

void drawMe(void){
  
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  //glLoadIdentity();
  //glTranslatef(0.0, 0.0, -25.0);
  glColor3f(0.0, 1.0, 0.0);
  
  if (isAnimate) 
    for (int i = 24; i < keyvals.size(); i++){
      
      glutSolidTorus(3.0, 15.0, 5, 5); 
    }
    
  else 
    {
   
      glBegin(GL_LINES);
      glColor3f(0.0,1.0,0.0);
      for (int i = 24; i < keyvals.size(); i++){
	//glColor3f(0.0, 0.0, 1.0 * (i-24)/(float)keyvals.size());
	glVertex3f((i - 24) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, 0.0);
	glVertex3f((i - 23) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, 0.0);
      }
      glEnd();
  
      glBegin(GL_QUADS);
      //glColor3f(1.0, 0.0, 1.0);
      for (int i = 24; i < keyvals.size(); i++){
	//glColor3f(0.0, 0.0, 1.0 * (i-24)/(float)keyvals.size());
	glVertex3f((i - 24) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, 0.0);
	glVertex3f((i - 24) * width / (keyvals.size()-24), height/2 - height/2 * ((float)keyvals[i])/127.0, 0.0);
	glVertex3f((i - 23) * width / (keyvals.size()-24), height/2 - height/2 * ((float)keyvals[i])/127.0, 0.0);
	glVertex3f((i - 23) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, 0.0);	
	
      }
      glEnd();

    }
  
  glutSwapBuffers();

}


void setup(void){
  glClearColor(1.0, 1.0 ,1.0, 0.0);
  inputMidi.init(1);
  //glEnable(GL_DEPTH_TEST);
}

void keyInput(unsigned char key, int x, int y)
{
  switch(key)
    {
    case 27:
      exit(0);
      break;
    case 'u':
      up+=1.0;
      break;
    case ' ':
      if(isAnimate)
	isAnimate = 0;
      else 
	isAnimate = 1;
      break;
    default:
      break;
    }

}

void disclaimer(void)
{
  cout << "This is a work in progress...\n";
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   //glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);
   
   width = w;
   height = h;
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main (int argc, char** argv)
{
  disclaimer();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100,100);
  glutCreateWindow("GroovMe");
  setup();
  glutDisplayFunc(drawMe);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyInput);
  glutTimerFunc(5, animate, 1);
  glutMainLoop();

  return 0;

}


