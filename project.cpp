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



//Globals
static GLsizei width, height;
//static float warp = 0.2; //The amount of warpage in a given visualization
static int up = 0.0; 
static int sides = 4;
static int isAnimate = 0;
static int animateInterval = 50; //Time interval between frames.
//static float pointSize = 3.0;

read_midi inputMidi;


std::vector < unsigned char> keyvals; 

std::vector < unsigned char> torusvals;

  
void animate(int value){
  torusvals.clear();
  keyvals = inputMidi.get_array();
  if (isAnimate){
    for (int i = 0; i < (int)keyvals.size(); i++){
      if ((float)keyvals[i] > 0){
	torusvals.push_back(keyvals[i]);
      }
    }
    if (sides > 25)
      up = 0;
    else if (sides < 4)
      up = 1;

    if (up)
      sides++;
    else
      sides--;
  }

  glutTimerFunc(animateInterval, animate, 1);
  glutPostRedisplay();
}

void drawMe(void){
  
  glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -1.00001);
  if (isAnimate){
    
    
    glColor3f(0.0, 1.0, 0.0);
    //glutSolidTorus(40.0, 100.0, 20, sides);
    //glutWireTorus(40.0, 30.0, 20, sides);
    
  }else {
    
    glBegin(GL_LINES);
    for (int i = 24; i < (int)keyvals.size(); i++){
      glColor3f(0.0, 1.0 * (i-24)/(float)keyvals.size(), 0.0);
      glVertex3f((i - 24) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, 0.0);
      glVertex3f((i - 23) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, 0.0);
      
    }
    glEnd(); 
    glBegin(GL_QUADS);
    //glColor3f(1.0, 0.0, 1.0);
    for (int i = 24; i < (int)keyvals.size(); i++){
      glColor3f(0.0, 1.0 * (i-24)/(float)keyvals.size(), 0.0);
      glVertex3f((i - 24) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, 0.0);
      glVertex3f((i - 24) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, -width / (keyvals.size()-24));
      glVertex3f((i - 23) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, 0.0 );
      glVertex3f((i - 23) * width / (keyvals.size()-24), height/2 + height/2 * ((float)keyvals[i])/127.0, -width / (keyvals.size()-24));

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
  glEnable(GL_DEPTH_TEST);
  inputMidi.init(1);
}

void keyInput(unsigned char key, int x, int y)
{
  switch(key)
    {
    case 27:
      exit(0);
      break;
    case 'u':
      break;
    case ' ':
      if(isAnimate){
	isAnimate = 0;
	//glLoadIdentity();
	//glOrtho(0.0, (float)width, 0.0, (float)height, -1.0, 1.0);
      }else{
	isAnimate = 1;
	//glLoadIdentity();
	//glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	
      }
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
   glFrustum(0.0, (float)w, 0.0, (float)h, 1.0, 100.0);
   //glFrustum(-(float)w, (float)w, -(float)h, (float)h, 1.0, 100.0);
   /*
   if (isAnimate){
     glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
   }else{
     glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);
   }
   */
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


