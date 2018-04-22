#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <GL/glut.h>


//#include <GL/glx.h>    /* this includes the necessary X headers */
//#include <GL/gl.h>
//
//#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
//#include <X11/keysym.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
    double x,y,z;
};

point pos,look,right,upp;
int step=3;

void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
    }
    glEnd();
}

void position(point a, bool diff=false)
{
    if(diff==true)
    {
        pos.x-=step*a.x;
        pos.y-=step*a.y;
        pos.z-=step*a.z;
//        goal.x-=step*a.x;
//        goal.y-=step*a.y;
//        goal.z-=step*a.z;
    }
    else
    {
        pos.x+=step*a.x;
        pos.y+=step*a.y;
        pos.z+=step*a.z;
//        goal.x+=step*a.x;
//        goal.y+=step*a.y;
//        goal.z+=step*a.z;
    }

}


void lookUP(bool rev=false)
{


    if(rev==true)
    {
        upp.x=-upp.x;
        upp.y=-upp.y;
        upp.z=-upp.z;
    }
    look.x=sin(step*pi/180)*upp.x+cos(step*pi/180)*look.x;
    look.y=sin(step*pi/180)*upp.y+cos(step*pi/180)*look.y;
    look.z=sin(step*pi/180)*upp.z+cos(step*pi/180)*look.z;


    upp.x=right.y*look.z-right.z*look.y;
    upp.y=right.z*look.x-right.x*look.z;
    upp.z=right.x*look.y-right.y*look.x;


}

void lookLeft(bool rev=false)
{


    if(rev==false)
    {
        right.x=-right.x;
        right.y=-right.y;
        right.z=-right.z;
    }
    look.x=sin(step*pi/180)*right.x+cos(step*pi/180)*look.x;
    look.y=sin(step*pi/180)*right.y+cos(step*pi/180)*look.y;
    look.z=sin(step*pi/180)*right.z+cos(step*pi/180)*look.z;

    right.x=look.y*upp.z-look.z*upp.y;
    right.y=look.z*upp.x-look.x*upp.z;
    right.z=look.x*upp.y-look.y*upp.x;



}


void Tilt(bool rev=false)
{


    if(rev==true)
    {
        right.x=-right.x;
        right.y=-right.y;
        right.z=-right.z;
    }
    upp.x=sin(step*pi/180)*right.x+cos(step*pi/180)*upp.x;
    upp.y=sin(step*pi/180)*right.y+cos(step*pi/180)*upp.y;
    upp.z=sin(step*pi/180)*right.z+cos(step*pi/180)*upp.z;

    right.x=look.y*upp.z-look.z*upp.y;
    right.y=look.z*upp.x-look.x*upp.z;
    right.z=look.x*upp.y-look.y*upp.x;



}


void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1':
        //drawgrid=1-drawgrid;
        lookLeft();
        break;

    case '2':
        lookLeft(true);
        break;

    case '3':
        lookUP();
        break;

    case '4':
        lookUP(true);
        break;

    case '5':
        Tilt();
        break;

    case '6':
        Tilt(true);
        break;

    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        cameraHeight -= 3.0;
        position(look,true);
        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;
        position(look);
        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        position(right);
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        position(right,true);
        break;

    case GLUT_KEY_PAGE_UP:
        position(upp);
        break;
    case GLUT_KEY_PAGE_DOWN:
        position(upp,true);
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}



void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    //gluLookAt(0,0,200,	0,0,0,	0,1,0);
    gluLookAt(pos.x,pos.y,pos.z,   pos.x+look.x,pos.y+look.y,pos.z+look.z,    upp.x,upp.y,upp.z);
    drawSquare(20);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    //drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    //drawSphere(30,24,20);




    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    angle+=0.5;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;

    pos.x=100,pos.y=50,pos.z=50;

    upp.x=0,upp.y=0,upp.z=1;
    look.x=-(1/sqrt(2)),look.y=-(1/sqrt(2)),look.z=0;
    right.x=-(1/sqrt(2)),right.y=(1/sqrt(2)),right.z=0;
    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
