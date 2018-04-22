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

point pos,v;
int step=40,ang=4;

void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,100,0);
            glVertex3f(-100,100,0);

            glVertex3f(100,-100,0);
            glVertex3f(100, 100,0);

            glVertex3f( 100,-100,0);
            glVertex3f(-100,-100,0);

            glVertex3f(-100,-100,0);
            glVertex3f(-100, 100,0);

        }
        glEnd();
    }
}

void drawLine()
{
    if(pos.x+(step*v.x)>100)v.x=-v.x;
    if(pos.x+(step*v.x)<-100)v.x=-v.x;
    if(pos.y+(step*v.y)>100)v.y=-v.y;
    if(pos.y+(step*v.y)<-100)v.y=-v.y;

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    {
        glVertex3f(pos.x,pos.y,0);
        glVertex3f(pos.x+(step*v.x),pos.y+(step*v.y),0);
    }
    glEnd();

    point perp1,perp2;

    perp1.x=-v.y;
    perp1.y=v.x;
    perp1.z=0;

    perp2.x=-perp1.x;
    perp2.y=-perp2.y;
    perp2.z=0;

    int t=25;
    glColor3f(1.0, 0, 0);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(pos.x+(step*v.x)+t*perp1.x,pos.y+(step*v.y)+t*perp1.y,0);
        glVertex3f(pos.x+(step*v.x)+t*perp2.x,pos.y+(step*v.y)+t*perp2.y,0);
        glVertex3f(pos.x+(step*v.x)+t*v.x,pos.y+(step*v.y)+t*v.y,0);
    }
    glEnd();




}




void rotation(bool clockwise=false)
{

    point perp;

    perp.x=-v.y;
    perp.y=v.x;

     if(clockwise==true)
    {
        perp.x=-perp.x;
        perp.y=-perp.y;
    }
    v.x=sin(ang*pi/180)*perp.x+cos(ang*pi/180)*v.x;
    v.y=sin(ang*pi/180)*perp.y+cos(ang*pi/180)*v.y;


}





void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1':
        //drawgrid=1-drawgrid;
        break;

    case '2':
        break;

    case '3':
        break;

    case '4':
        break;

    case '5':
        break;

    case '6':
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
        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;
        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        rotation(true);
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        rotation();
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
    gluLookAt(0,0,200,	0,0,0,	0,1,0);



    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawLine();
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
    pos.x+=2*v.x;
    pos.y+=2*v.y;
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

    pos.x=10,pos.y=10,pos.z=50;
    v.x=.35,v.y=.26,v.z=0;
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
