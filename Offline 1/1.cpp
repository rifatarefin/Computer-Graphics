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
int step=3,r;

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
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
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

    }
    else
    {
        pos.x+=step*a.x;
        pos.y+=step*a.y;
        pos.z+=step*a.z;

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
void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCylinder(double radius, double height, int segments)
{
    int i;
    struct point points[100];
    glColor3f(0,1,0);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*(pi/2));
        points[i].y=radius*sin(((double)i/(double)segments)*(pi/2));
    }

    for(i=0; i<segments; i++)
    {
        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x,points[i].y,-(height));
            glVertex3f(points[i+1].x,points[i+1].y,-(height));
            glVertex3f(points[i+1].x,points[i+1].y,height);
            glVertex3f(points[i].x,points[i].y,height);
        }
        glEnd();
    }
}

void drawSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
            points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}

void shape()
{
    glPushMatrix();
    {
        glTranslatef(0,16,0);
        glRotatef(90,1,0,0);
        drawSquare(r);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(0,-16,0);
        glRotatef(90,1,0,0);
        drawSquare(r);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glTranslatef(16,0,0);
        glRotatef(90,0,1,0);
        drawSquare(r);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glTranslatef(-16,0,0);
        glRotatef(90,0,1,0);
        drawSquare(r);
    }

    glPopMatrix();

    glPushMatrix();

    {
        glTranslatef(0,0,16);
        glRotatef(90,0,0,1);
        drawSquare(r);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glTranslatef(0,0,-16);
        glRotatef(90,0,0,1);
        drawSquare(r);
    }
    glPopMatrix();
    glColor3f(0,1,0);
    glPushMatrix();

    {

        glRotatef(45,0,0,1);
        glTranslatef(r+(r/2.4),0,0);
        glRotatef(-45,0,0,1);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();
    glPushMatrix();

    {
        glRotatef(45,0,0,1);
        glTranslatef(0,r+(r/2.4),0);
        glRotatef(45,0,0,1);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glRotatef(45,0,0,1);
        glTranslatef(-r-(r/2.4),0,0);
        glRotatef(135,0,0,1);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();
    glPushMatrix();

    {
        glRotatef(45,0,0,1);
        glTranslatef(0,-r-(r/2.4),0);
        glRotatef(225,0,0,1);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();
    glPushMatrix();

    {
        glRotatef(-45,0,1,0);
        glTranslatef(r+(r/2.4),0,0);
        glRotatef(45,0,1,0);
        glRotatef(90,1,0,0);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(45,0,1,0);
        glTranslatef(r+(r/2.4),0,0);
        glRotatef(45,0,1,0);
        glRotatef(90,1,0,0);
        drawCylinder(16-r,r,50);
    }

    glPopMatrix();
    glPushMatrix();

    {
        glRotatef(-45,0,1,0);
        glTranslatef(0,0,r+(r/2.4));
        glRotatef(-45,0,1,0);
        glRotatef(90,1,0,0);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glRotatef(-45,0,1,0);
        glTranslatef(-r-(r/2.4),0,0);
        glRotatef(-135,0,1,0);
        glRotatef(90,1,0,0);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glRotatef(-45,1,0,0);
        glTranslatef(0,r+(r/2.4),0);
        glRotatef(45,1,0,0);
        glRotatef(90,0,1,0);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glRotatef(45,1,0,0);
        glTranslatef(0,r+(r/2.4),0);
        glRotatef(45,1,0,0);
        glRotatef(90,0,1,0);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glRotatef(135,1,0,0);
        glTranslatef(0,r+(r/2.4),0);
        glRotatef(45,1,0,0);
        glRotatef(90,0,1,0);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glRotatef(-135,1,0,0);
        glTranslatef(0,r+(r/2.4),0);
        glRotatef(45,1,0,0);
        glRotatef(90,0,1,0);
        drawCylinder(16-r,r,50);
    }
    glPopMatrix();

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
        if(r>0)r--;
        break;
    case GLUT_KEY_END:
        if(r<16)r++;
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



    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();

    shape();
    glColor3f(1,0,0);

    glPushMatrix();

    {
        glTranslatef(r,r,r);
        drawSphere(16-r,50,50);
    }
    glPopMatrix();
    glPushMatrix();

    {
        glTranslatef(-r,r,r);
        glRotatef(90,0,0,1);
        drawSphere(16-r,50,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glTranslatef(r,-r,r);
        glRotatef(-90,0,0,1);
        drawSphere(16-r,50,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glTranslatef(-r,-r,r);
        glRotatef(180,0,0,1);
        drawSphere(16-r,50,50);
    }
    glPopMatrix();
    glPushMatrix();

    {
        glTranslatef(r,r,-r);
        glRotatef(90,0,1,0);
        drawSphere(16-r,50,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glTranslatef(r,-r,-r);
        glRotatef(90,0,1,0);
        glRotatef(-90,0,0,1);
        drawSphere(16-r,50,50);
    }
    glPopMatrix();

    glPushMatrix();

    {
        glTranslatef(-r,r,-r);
        glRotatef(-90,0,1,0);
        glRotatef(90,0,0,1);
        drawSphere(16-r,50,50);
    }
    glPopMatrix();

        glTranslatef(-r,-r,-r);
        glRotatef(-90,0,1,0);
        glRotatef(180,0,0,1);
        drawSphere(16-r,50,50);




    //drawCylinder(r,40,50);

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
    angle=0,r=10;

    pos.x=50,pos.y=50,pos.z=30;

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
