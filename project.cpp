#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include "ReadMidi.h"

#define MAXVAL 127
#define PI 3.14159

//equation for frequency:
//y = 15.43386241 e ** (0.05776226064 x)
//where x is the distance from C0

#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glut.h>
#  include <GL/glext.h>
#endif


//Globals
static GLsizei width, height;
//static float warp = 0.2; //The amount of warpage in a given visualization
static float up = 0.0; 
static float ticker = 0.0;
static int isAnimate = 0;
static int animateInterval = 50; //Time interval between frames.
//static float pointSize = 3.0;

read_midi inputMidi;


std::vector < unsigned char > keyvals; 
std::vector < std::vector<unsigned char> > on_keys;
//std::vector < float > xvector; 
//std::vector < float > yvector;
std::vector < float > amp;
std::vector < float > phase_shift;
std::vector < float > B;

float get_height_scalar(int x){
    float height = 0;
    for(unsigned int i = 0; i < amp.size(); i++){
        height += amp[i] * cos(PI*(float)x/6.0-phase_shift[i]);
    }  
    return height;
}

void add_note(unsigned char x, unsigned char a){
    amp.push_back((float) a);
    B.push_back(PI/6);
    phase_shift.push_back((((int)x)*PI/6.0));
}
  
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

  if (ticker > 12.0 * (width / (float)keyvals.size()))
    ticker = 0.0;
  else
    ticker += 2.0;

  keyvals = inputMidi.get_array();
  on_keys = inputMidi.get_on_keys();

  amp.clear();
  phase_shift.clear();
  B.clear();
  for(unsigned int i = 0; i < on_keys.size(); i++){
    add_note(on_keys[i][0], on_keys[i][1]);
  }
  glutTimerFunc(animateInterval, animate, 1);
  glutPostRedisplay();
}

void drawMe(void){
  
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -1.001);
  
  if (isAnimate) 
    std::cout << "PRINT STUFF!\n";
  /*for (int i = 24; i < keyvals.size(); i++){
      
    glutSolidTorus(3.0, 15.0, 5, 5); 
    }*/
    
  else 
    {
      glBegin(GL_QUADS);
      //glColor3f(1.0, 0.0, 0.0);
      for (unsigned int i = 0; i < keyvals.size(); i++){
	glColor3f(0.0, 1.0 * (float)i /(float)keyvals.size(), 0.0);
	    
	glVertex3f((i + 1) * width / (keyvals.size()) + ticker, 
		   height/2 + height/2 * (get_height_scalar(i))/127.0, 0.0);
	glVertex3f((i + 1) * width / (keyvals.size()) + ticker, height/2, 0.0);
	glVertex3f((i) * width / (keyvals.size()) + ticker, height/2, 0.0);
	glVertex3f((i) * width / (keyvals.size()) + ticker, 
		   height/2 + height/2 * (get_height_scalar(i))/127.0, 0.0);	
	
      }
      glEnd();
      glBegin(GL_QUADS);
      for (unsigned int i = 0; i < keyvals.size(); i++){
	glColor3f(0.0, 1.0 * (float)i /(float)keyvals.size(), 0.0);
	    
	glVertex3f((i + 1) * width / (keyvals.size()) + ticker, 
		   height/2 + height/2 * (get_height_scalar(i))/127.0, 1.0);
	glVertex3f((i + 1) * width / (keyvals.size()) + ticker, height/2, 1.0);
	glVertex3f((i) * width / (keyvals.size()) + ticker, height/2, 1.0);
	glVertex3f((i) * width / (keyvals.size()) + ticker, 
		   height/2 + height/2 * (get_height_scalar(i))/127.0, 1.0);
      }
      glEnd();
      glBegin(GL_QUADS);
      for (unsigned int i = 0; i < keyvals.size(); i++){
	glColor3f(0.0, 1.0 * (float)i /(float)keyvals.size(), 0.0);
	    
	glVertex3f((i + 1) * width / (keyvals.size()) + ticker, 
		   height/2 + height/2 * (get_height_scalar(i))/127.0, 1.0);
	glVertex3f((i + 1) * width / (keyvals.size()) + ticker, height/2, 0.0);
	glVertex3f((i) * width / (keyvals.size()) + ticker, height/2, 1.0);
	glVertex3f((i) * width / (keyvals.size()) + ticker, 
		   height/2 + height/2 * (get_height_scalar(i))/127.0, 0.0);
      }
      glEnd();
      glBegin(GL_QUADS);
      for (unsigned int i = 0; i < keyvals.size(); i++){
	glColor3f(0.0, 1.0 * (float)i /(float)keyvals.size(), 0.0);
	    
	glVertex3f((i + 1) * width / (keyvals.size()) + ticker, 
		   height/2 + height/2 * (get_height_scalar(i))/127.0, 0.0);
	glVertex3f((i + 1) * width / (keyvals.size()) + ticker, height/2, 1.0);
	glVertex3f((i) * width / (keyvals.size()) + ticker, height/2, 0.0);
	glVertex3f((i) * width / (keyvals.size()) + ticker, 
		   height/2 + height/2 * (get_height_scalar(i))/127.0, 1.0);
      }
      glEnd();

    }
  
  glutSwapBuffers();

}


void setup(void){
  glClearColor(1.0, 1.0 ,1.0, 0.0);
  inputMidi.init(1);
  glEnable(GL_DEPTH_TEST);
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
    std::cout << "This is a work in progress...\n";
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glFrustum(0.0, (float)w, 0.0, (float)h, 1.0, 50.0);
   //glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);
   
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
  glutCreateWindow("Midulization");
  setup();
  glutDisplayFunc(drawMe);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyInput);
  glutTimerFunc(5, animate, 1);
  glutMainLoop();

  return 0;

}


