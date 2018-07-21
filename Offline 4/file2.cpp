#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

struct point
{
    double x,y,z;
};
extern point pos;
extern int recursion;
double dist(point a, point b)
{
    double x=a.x-b.x;
    double y=a.y-b.y;
    double z=a.z-b.z;
    return sqrt(x*x+y*y+z*z);
}

double dot(point a, point b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

point normalize(point a)
{
    double ssd=sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
    point tmp={a.x/ssd,a.y/ssd,a.z/ssd};
    return tmp;
}

class Ray
{
public:
    point start;
    point dir;

    Ray(point eye, point corner)
    {
        start=eye;
        point wew;
        wew.x=corner.x-eye.x;
        wew.y=corner.y-eye.y;
        wew.z=corner.z-eye.z;
        double zz=sqrt(wew.x*wew.x+wew.y*wew.y+wew.z*wew.z);
        dir.x=wew.x/zz;
        dir.y=wew.y/zz;
        dir.z=wew.z/zz;
    }
};

point getReflection(point dir,point normal)
{
    point rf;
    rf.x=dir.x-2.0*dot(dir,normal)*normal.x;
    rf.y=dir.y-2.0*dot(dir,normal)*normal.y;
    rf.z=dir.z-2.0*dot(dir,normal)*normal.z;
    return normalize(rf);
}
class Object
{
public:
    point reference_point;
    double height, width, length;
    int Shine;
    double color[3]= {1,0,0};
    double co_efficients[4];


    Object() { }
    virtual void draw() {}
    virtual double intersect(Ray &r, double *current_color, int level)
    {

        return -1;
    }
    virtual double getIntersectingT(Ray &r)
    {
        //if(debug==false)cout<<"ds"<<endl;
        return -1;
    }
    virtual point getNormal(point a, point center){}

    void setColor(double r, double g, double b)
    {
        color[0]=r;
        color[1]=g;
        color[2]=b;
    }
    void setShine(int s)
    {
        Shine=s;
    }
    void setCoEfficients(double a,double b,double c, double d)
    {
        co_efficients[0]=a;
        co_efficients[1]=b;
        co_efficients[2]=c;
        co_efficients[3]=d;
    }

};
extern vector<Object*>objects;
extern vector<point>lights;
class Sphere:public Object
{
public:
    Sphere(point center, double radius)
    {
        reference_point=center;
        length=radius;
    }
    point getNormal(point a, point center)
    {
        point tmp;
        tmp.x=a.x-center.x;
        tmp.y=a.y-center.y;
        tmp.z=a.z-center.z;
        return normalize(tmp);
    }
    void draw()
    {
        glPushMatrix();
        {
            glTranslatef(reference_point.x,reference_point.y,reference_point.z);
            glColor3f(color[0],color[1],color[2]);
            glutSolidSphere(length,100,100);
        }
        glPopMatrix();

    }
    double getIntersectingT(Ray &r)
    {
        point r0=r.start;
        r0.x-=reference_point.x;
        r0.y-=reference_point.y;
        r0.z-=reference_point.z;

        point rd=r.dir;
        double a=1;
        double b=2*(r0.x*rd.x+r0.y*rd.y+r0.z*rd.z);
        double c=(r0.x*r0.x+r0.y*r0.y+r0.z*r0.z)-length*length;
        double d=(b*b)-(4*a*c);
        //cout<<rd.x<<" "<<rd.y<<" "<<rd.z<<endl;
        //if(debug==true)cout<<d<<endl;
        if(d<0)return -1;
        double t1=(-b+sqrt(d))/2;
        double t2=(-b-sqrt(d))/2;

        double tmp=min(t1,t2);
        if(tmp<0)
        {
            tmp=max(t1,t2);
        }
        //if(debug==false)cout<<tmp<<endl;
        return tmp;
    }

    double intersect(Ray &r, double *current_color, int level)
    {
        double tmp=getIntersectingT(r);
        if(tmp<=0)return -1;
        if(level==0)return tmp;
        current_color[0]=color[0]*co_efficients[0];
        current_color[1]=color[1]*co_efficients[0];
        current_color[2]=color[2]*co_efficients[0];

        point intersection_point;
        intersection_point.x=r.start.x+r.dir.x*tmp;
        intersection_point.y=r.start.y+r.dir.y*tmp;
        intersection_point.z=r.start.z+r.dir.z*tmp;

        point normal=getNormal(intersection_point,reference_point);
        point reflection=getReflection(r.dir,normal);

        for(int i=0;i<lights.size();i++)
        {
            point light_source=lights[i];
            point light_dir;
            light_dir.x=light_source.x-intersection_point.x;
            light_dir.y=light_source.y-intersection_point.y;
            light_dir.z=light_source.z-intersection_point.z;
            light_dir=normalize(light_dir);

            point start;
            start.x=intersection_point.x+light_dir.x*1.0;
            start.y=intersection_point.y+light_dir.y*1.0;
            start.z=intersection_point.z+light_dir.z*1.0;

            double dis=dist(start,light_source);

            Ray L(start,light_source);
            bool flag=true;
            for(int j=0;j<objects.size();j++)
            {
                double t=objects[j]->getIntersectingT(L);
                //cout<<t<<endl;
                if(t<=0 || t>dis)continue;
                flag=false;
                cout<<t<<endl;
                break;
            }
            if(flag==true)
            {

                double lambert=dot(L.dir,normal);
                double phong=pow(dot(r.dir,reflection),Shine);

                lambert=max(lambert,0.0);
                phong=max(phong,0.0);

                current_color[0]+=lambert*co_efficients[1]+phong*co_efficients[2];
                current_color[1]+=lambert*co_efficients[1]+phong*co_efficients[2];
                current_color[2]+=lambert*co_efficients[1]+phong*co_efficients[2];
            }


        }

        if(level<recursion)
        {
            point start;
            start.x=intersection_point.x+reflection.x;
            start.y=intersection_point.y+reflection.y;
            start.z=intersection_point.z+reflection.z;

            point far;
            far.x=intersection_point.x+reflection.x*2;
            far.y=intersection_point.y+reflection.y*2;
            far.z=intersection_point.z+reflection.z*2;

            Ray reflectionRay(start,far);
            int nearest=-1;double tmin=INT_MAX;
            for(int k=0;k<objects.size();k++)
            {
                double *dummy_color;
                dummy_color=new double[3];
                double t=objects[k]->intersect(reflectionRay,dummy_color,0);
                //cout<<t<<endl;
                delete dummy_color;
                if(t<=0.000000001)continue;

                if(t<tmin)
                {
                    tmin=t;
                    nearest=k;
                    //cout<<nearest<<endl;
                }


            }
            if(nearest!=-1)
            {
                double *reflect_color;
                reflect_color=new double[3];
                double t=objects[nearest]->intersect(reflectionRay,reflect_color,level+1);

                current_color[0]+=reflect_color[0]*co_efficients[3];
                current_color[1]+=reflect_color[1]*co_efficients[3];
                current_color[2]+=reflect_color[2]*co_efficients[3];
            }
        }

        return tmp;
    }
};

class Floor:public Object
{
public:
    Floor(double floor_width, double tile_width)
    {
        reference_point= {-floor_width/2,-floor_width/2,0};
        length=tile_width;
    }
    void draw()
    {
        glPushMatrix();
        {
            glScalef(1,1,.001);
        }
        int i=reference_point.x;
        int rr=1,gg=1,bb=1;
        while(i<-reference_point.x)
        {
            rr=1-rr,gg=1-gg,bb=1-bb;
            int r=rr,g=gg,b=bb;
            int j=reference_point.y;
            while(j<-reference_point.y)
            {
                glColor3f(r,g,b);
                glPushMatrix();
                {
                    glTranslatef(i,j,0);
                    glutSolidCube(20);
                }
                glPopMatrix();
                j+=20;
                r=1-r,g=1-g,b=1-b;
            }
            i+=20;
        }
        glPopMatrix();
    }
};
