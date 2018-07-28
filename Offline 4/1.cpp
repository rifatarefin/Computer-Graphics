#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"file2.cpp"
#include <GL/glut.h>
ifstream fin("scene.txt");


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
int recursion;
int image_width,window_height=500,window_width=500;
vector<Object*>objects;
vector<point>lights;
point pos,look,right_v,upp;
int step=3,r;
//{
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


    upp.x=right_v.y*look.z-right_v.z*look.y;
    upp.y=right_v.z*look.x-right_v.x*look.z;
    upp.z=right_v.x*look.y-right_v.y*look.x;


}

void lookLeft(bool rev=false)
{


    if(rev==false)
    {
        right_v.x=-right_v.x;
        right_v.y=-right_v.y;
        right_v.z=-right_v.z;
    }
    look.x=sin(step*pi/180)*right_v.x+cos(step*pi/180)*look.x;
    look.y=sin(step*pi/180)*right_v.y+cos(step*pi/180)*look.y;
    look.z=sin(step*pi/180)*right_v.z+cos(step*pi/180)*look.z;

    right_v.x=look.y*upp.z-look.z*upp.y;
    right_v.y=look.z*upp.x-look.x*upp.z;
    right_v.z=look.x*upp.y-look.y*upp.x;



}


void Tilt(bool rev=false)
{


    if(rev==true)
    {
        right_v.x=-right_v.x;
        right_v.y=-right_v.y;
        right_v.z=-right_v.z;
    }
    upp.x=sin(step*pi/180)*right_v.x+cos(step*pi/180)*upp.x;
    upp.y=sin(step*pi/180)*right_v.y+cos(step*pi/180)*upp.y;
    upp.z=sin(step*pi/180)*right_v.z+cos(step*pi/180)*upp.z;

    right_v.x=look.y*upp.z-look.z*upp.y;
    right_v.y=look.z*upp.x-look.x*upp.z;
    right_v.z=look.x*upp.y-look.y*upp.x;



}
//{
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
//}
void capture()
{
    bitmap_image image(image_width,image_width);
    double plane_dist=(window_height/2.0)/tan((40*pi)/180);
    point top_left;


    top_left.x=pos.x+(look.x*plane_dist)-(right_v.x*window_width/2.0)+(upp.x*window_height/2.0);
    top_left.y=pos.y+(look.y*plane_dist)-(right_v.y*window_width/2.0)+(upp.y*window_height/2.0);
    top_left.z=pos.z+(look.z*plane_dist)-(right_v.z*window_width/2.0)+(upp.z*window_height/2.0);
    double du=(double)window_width/(double)image_width;
    double dv=(double)window_height/(double)image_width;

    for(int i=0; i<image_width; i++)
    {

        for(int j=0; j<image_width; j++)
        {
            point corner;
            corner.x=top_left.x+(j*du*right_v.x)-(i*dv*upp.x);
            corner.y=top_left.y+(j*du*right_v.y)-(i*dv*upp.y);
            corner.z=top_left.z+(j*du*right_v.z)-(i*dv*upp.z);
            //cout<<corner.x<<" "<<corner.y<<" "<<corner.z<<endl;
            Ray ray(pos,corner);
            int nearest=-1;
            double tmin=INT_MAX;
            for(int k=0; k<objects.size(); k++)
            {
                double *dummy_color;
                dummy_color=new double[3];
                double t=objects[k]->intersect(ray,dummy_color,0);

                delete dummy_color;
                if(t<=eps)continue;

                if(t<tmin)
                {
                    tmin=t;
                    nearest=k;

                }


            }
            if(nearest!=-1)
            {
                double *color;
                color=new double[3];
                double t=objects[nearest]->intersect(ray,color,1);
                //cout<<color[0]<<" "<<color[1]<<color[2]<<endl;
                image.set_pixel(j,i,255*color[0],255*color[1],255*color[2]);
                delete color;

            }//image.set_pixel(j,i,255,0,0);
        }//cout<<endl;
    }

    image.save_image("1.bmp");
    image.clear();
    cout<<"saved"<<endl;
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

    case '0':
        capture();
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
        position(look,true);
        break;
    case GLUT_KEY_UP:		// up arrow key
        position(look);
        break;

    case GLUT_KEY_RIGHT:
        position(right_v);
        break;
    case GLUT_KEY_LEFT:
        position(right_v,true);
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

void loadFileData()
{
    int num=10;
    fin>>recursion;
    fin>>image_width;
    fin>>num;

    double rad,r,g,b,i,j,k,l;
    int sh;
    Object *temp;
    string s;
    for(int v=0; v<num; v++)
    {
        fin>>s;
        if(s=="sphere")
        {
            point refe;
            fin>>refe.x>>refe.y>>refe.z;

            fin>>rad;
            temp=new Sphere(refe,rad);
        }

        else if(s=="triangle")
        {
            point a,b,c;
            fin>>a.x>>a.y>>a.z;
            fin>>b.x>>b.y>>b.z;
            fin>>c.x>>c.y>>c.z;
            temp=new Triangle(a,b,c);
        }

        else if(s=="general")
        {
            double A, B, C, D, E, F, G, H, I, J;
            fin>>A>>B>>C>>D>>E>>F>>G>>H>>I>>J;
            double co[10]={A, B, C, D, E, F, G, H, I, J};
            point cube_ref;
            fin>>cube_ref.x>>cube_ref.y>>cube_ref.z;
            double length, width, height;
            fin>>length>>width>>height;
            temp=new General(co,length,width,height,cube_ref);

        }
        fin>>r>>g>>b;
        fin>>i>>j>>k>>l;
        fin>>sh;

        temp->setColor(r,g,b);
        temp->setCoEfficients(i,j,k,l);
        temp->setShine(sh);
        objects.push_back(temp);
    }

    temp=new Floor(1000,20);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    fin>>num;
    for(int v=0;v<num;v++)
    {
        point llt;
        fin>>llt.x>>llt.y>>llt.z;
        lights.push_back(llt);

    }
    //lights.push_back({-50,50,60});
}
void loadTestData()
{
    Object *temp=new Sphere({0,0,10},10);
    temp->setColor(1,0,0);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);
    lights.push_back({-50,50,60});

    temp=new Floor(1000,20);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    temp=new Sphere({-25,25,35},20);
    temp->setColor(0,1,0);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    temp=new Triangle({30,60,0}, {50,30,0}, {50,45,50});
    temp->setColor(1.0,0.0,0.0);
    temp->setCoEfficients(0.4,0.2,0.1,0.3);
    temp->setShine(1);
    objects.push_back(temp);

    image_width=768;
    recursion=4;
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

    for(vector<Object*>::iterator it=objects.begin(); it!=objects.end(); it++)
    {

        (*it)->draw();

    }
    glPointSize(5);
    glColor3f(1,1,0);
    for(vector<point>::iterator it=lights.begin(); it!=lights.end(); it++)
    {
        point ds=*it;
        glBegin(GL_POINTS);
        {
            glVertex3f(ds.x,ds.y,ds.z);
        }
        glEnd();
    }
    glFlush();

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

    pos.x=20,pos.y=-100,pos.z=20;

    upp.x=0,upp.y=0,upp.z=1;
    look.x=-(20/sqrt(10800)),look.y=(100/sqrt(10800)),look.z=-(20/sqrt(10800));
    right_v.x=(100/sqrt(10400)),right_v.y=-(20/sqrt(10400)),right_v.z=0;

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
    loadFileData();
    //loadTestData();
    //cout<<objects.size()<<endl;
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
