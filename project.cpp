#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <list>
#include "ReadMidi.h"

#define MAXVAL 127
#define PI 3.14159

//equation for frequency:
//y = 15.43386241 e ** (0.05776226064 x)
//where x is the distance from C0
#define FREQ(x) (15.43386241 * exp(0.05776226064 * (x)))

#define ONE_MIDI 1
//#define DEBUGGING

#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glut.h>
#  include <GL/glext.h>
#endif


//Globals0
static GLsizei width, height;
static float up = 0.0; 
static int isAnimate = 0;
static int animateInterval = 10; //Time interval between frames.

read_midi inputMidi;

std::vector < unsigned char > keyvals; 
std::vector < std::vector<unsigned char> > on_keys;
std::vector < float > amp;
std::vector < float > phase_shift;
std::vector < float > B;
std::list < std::vector<float> > frames;

float get_height_scalar(float x){
    float height = 0;
    float a = 0;
    for(unsigned int i = 0; i < amp.size(); i++){
        a += amp[i];
    }
    if(amp.size() == 0) return 0;
    a = a/amp.size();
    for(unsigned int i = 0; i < amp.size(); i++){
        height += a * cos(B[i]*(float)x);
    }
    height = height/amp.size();
    return height;
}

void add_note(unsigned char x, unsigned char a){
    amp.push_back(32.0);
    B.push_back(PI * 2 * FREQ(x+1));
    phase_shift.push_back(0.0);
}
  
void animate(int value){
    keyvals = inputMidi.get_array();
    on_keys = inputMidi.get_on_keys();

#ifdef DEBUGGING
    if(B.size() > 0) std::cout << amp[0] << '\t' << B[0]/(2*PI) << std::endl;
#endif

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
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -1.001);
  
    if (isAnimate) 
            gluLookAt(0.0, 0.0, 0.0, 
	                0.0, 0.0, 0.0, 
	                0.0, 1.0, 0.0);
    
    else {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glColor4f(1.0, 0.0, 0.0, 1.0);
      
        while(frames.size() >= 100) frames.pop_front();

        std::vector<float> last_frame;
        for(float i = 0; i < width; i+=width/500){
            last_frame.push_back(
                    height/2.0 + height/2.0 * get_height_scalar(i)/127.0);
        }
        frames.push_back(last_frame);
      
        std::list<std::vector<float> >::iterator it = frames.begin();
        for(unsigned int i = 0; i < frames.size(); i++){
            glColor4f(0.01 * (i), 0.0, 0.01*(100 - i),  2.0/(frames.size() - i));
	        float w = (frames.size() - i) * -0.1;
            for(unsigned int j = 0; j < it->size(); j++){
	            glVertex3f((j*(width/500) + 1 - width/2.0), (*it)[j], w);
	            glVertex3f((j*(width/500) + 1 - width/2.0), height/2.0, w );
	            glVertex3f((j*(width/500) - width/2.0), height/2.0, w);
	            glVertex3f((j*(width/500) - width/2.0), (*it)[j], w);	  
            }
            if(it == frames.end()) break;
            it++;
        }
        glEnd();
        gluLookAt(0.0, height/2.0 + 50.0, 0.0, 
                0.0, 0.0, 0.0,
		        0.0, 1.0, 0.0);
		
    }
  
    glutSwapBuffers();

}


void setup(void){
    glClearColor(1.0, 1.0 ,1.0, 0.0);
#ifdef ONE_MIDI
    inputMidi.init(1);
#else
    inputMidi.init();
#endif
}

void keyInput(unsigned char key, int x, int y)
{
    switch(key){
        case 27:
            exit(0);
            break;
        case 'u':
            up+=5.0;
            break;
        case 'j':
            up-=5.0;
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
    std::cout << "Connect MIDI input device and play.\n";
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-(float)w/2.0, (float)w/2.0, 0.0, (float)h, 1.0, 50.0);
   
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


