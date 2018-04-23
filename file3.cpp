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
double angle,angle2,trian,fing1,fing2,top;

struct point
{
    double x,y,z;
};

point pos,upp;
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
void left(bool right=false)
{
    point tmp;

    tmp.x=pos.y;
    tmp.y=-pos.x;
    tmp.z=0;
    if(right==true)
    {
        tmp.x=-tmp.x;
        tmp.y=-tmp.y;
    }
    pos.x=sin(step*pi/180)*tmp.x+cos(step*pi/180)*pos.x;
    pos.y=sin(step*pi/180)*tmp.y+cos(step*pi/180)*pos.y;
}

void arm1()
{
    glRotatef(top,0,1,0);
    glPushMatrix();
    {
        glRotatef(angle,1,0,0);
        glTranslatef(0,0,-30);
        glScalef(1,1,5);
        glutWireSphere(6,10,10);
        glScalef(1,1,.2);
    }
    glPopMatrix();
    glRotatef(angle,1,0,0);
    glTranslatef(0,0,-60);
    glRotatef(angle2,1,0,0);
    glTranslatef(0,0,-20);

    glScalef(1,1,5);
    glutWireSphere(4,10,10);
    glRotatef(trian,0,0,1);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(0,0,-4);
        glVertex3f(-6,0,-8);
        glVertex3f(6,0,-8);
    }
    glEnd();
    glScalef(1,1,.2);

    glPushMatrix();
    {
        glTranslatef(-6,0,-50);
        glRotatef(fing1,0,0,1);
    glScalef(1,1,5);
    glutWireSphere(2,5,5);
    }
    glPopMatrix();

    glTranslatef(6,0,-40);
    glRotatef(fing2,1,0,0);
    glTranslatef(0,0,-10);
    glScalef(1,1,5);
    glutWireSphere(2,5,5);


}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1':
        if(angle<45)angle+=3;
        break;
    case '2':
        if(angle>-45)angle-=3;
        break;
    case '3':
        if(angle2>-90)angle2-=3;
        break;
    case '4':
        if(angle2<0)angle2+=3;
        break;
    case '5':
        if(trian<45)trian+=3;
        break;
    case '6':
        if(trian>-45)trian-=3;
        break;
    case '7':
        if(fing1<45)fing1+=3;
        break;
    case '8':
        if(fing1>-45)fing1-=3;
        break;
    case '9':
        if(fing2>-90)fing2-=3;
        break;
    case '0':
        if(fing2<0)fing2+=3;
        break;
    case 'q':
        if(top>-90)top-=3;
        break;
    case 'w':
        if(top<0)top+=3;
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
        pos.z-=step;
        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;
        pos.z+=step;
        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        left(true);
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        left();
        break;

    case GLUT_KEY_PAGE_UP:
        break;
    case GLUT_KEY_PAGE_DOWN:
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
    gluLookAt(pos.x,pos.y,pos.z,	0,0,-40,	0,0,1);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();

    arm1();





    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{

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
    angle=0,angle2=0,trian=0,fing1=0,fing2=0,top=0;
    pos.x=90,pos.y=90,pos.z=-20;
    upp.x=0,upp.y=0,upp.z=1;

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

