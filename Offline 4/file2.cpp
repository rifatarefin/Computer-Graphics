#include<bits/stdc++.h>
#include <GL/glut.h>
#include"bitmap_image.hpp"
#define eps 1e-4
using namespace std;
bitmap_image txt("image.bmp");
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
    point tmp= {a.x/ssd,a.y/ssd,a.z/ssd};
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
    //virtual point getNormal(point a, point center){}

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
        if(tmp<=0)
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

        for(int i=0; i<lights.size(); i++)
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
            for(int j=0; j<objects.size(); j++)
            {
                double t=objects[j]->getIntersectingT(L);
                //cout<<t<<endl;
                if(t<=0 || t>dis)continue;
                flag=false;
                //cout<<t<<endl;
                break;
            }
            if(flag==true)
            {
                Ray incident(light_source,intersection_point);
                point light_reflection=getReflection(incident.dir,normal);
                point to_eye;
                to_eye.x=-r.dir.x;
                to_eye.y=-r.dir.y;
                to_eye.z=-r.dir.z;

                double lambert=dot(L.dir,normal);
                double phong=pow(dot(to_eye,light_reflection),Shine);

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
            int nearest=-1;
            double tmin=INT_MAX;
            for(int k=0; k<objects.size(); k++)
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

class General:public Object
{
public:

    double A, B, C, D, E, F, G, H, I, J;
    double length, width, height;
    point cube_ref;
    General(double coeff[10],double len, double wid,double hei, point cube)
    {
        A=coeff[0];
        B=coeff[1];
        C=coeff[2];
        D=coeff[3];
        E=coeff[4];
        F=coeff[5];
        G=coeff[6];
        H=coeff[7];
        I=coeff[8];
        J=coeff[9];
        length=len;
        width=wid;
        height=hei;
        cube_ref=cube;

    }

    point getNormal(point a)
    {
        point tmp;
        tmp.x=2*A*a.x + D*a.y + E*a.z + G;
        tmp.y=2*B*a.y + D*a.x + F*a.z + H;
        tmp.z=2*C*a.z + E*a.x + F*a.y + I;
        return normalize(tmp);
    }

    double getIntersectingT(Ray &r)
    {
        point r0=r.start;
        point rd=r.dir;

        double a=A*rd.x*rd.x + B*rd.y*rd.y + C*rd.z*rd.z + D*rd.x*rd.y + E*rd.x*rd.z + F*rd.y*rd.z;
        double b=2*A*r0.x*rd.x + 2*B*r0.y*rd.y + 2*C*r0.z*rd.z + D*(r0.x*rd.y + r0.y*rd.x) + E*(r0.x*rd.z + r0.z*rd.x) +
                 F*(r0.y*rd.z + rd.y*r0.z) + G*rd.x + H*rd.y + I*rd.z;
        double c=A*r0.x*r0.x + B*r0.y*r0.y + C*r0.z*r0.z + D*r0.x*r0.y + E*r0.x*r0.z + F*r0.y*r0.z + G*r0.x + H*r0.y + I*r0.z + J;
        double tmp,d;
        if(a==0)
        {
            tmp=(-c)/b;
            return tmp;
        }
        d=(b*b)-(4*a*c);
        if(d<0)return -1;

        double t1=(-b+sqrt(d))/2*a;
        double t2=(-b-sqrt(d))/2*a;

        point intersect_point1, intersect_point2;
        intersect_point1.x=r.start.x+(r.dir.x*t1);
        intersect_point1.y=r.start.y+(r.dir.y*t1);
        intersect_point1.z=r.start.z+(r.dir.z*t1);

        intersect_point2.x=r.start.x+(r.dir.x*t2);
        intersect_point2.y=r.start.y+(r.dir.y*t2);
        intersect_point2.z=r.start.z+(r.dir.z*t2);

        double xMin = cube_ref.x;
        double xMax = xMin + length;

        double yMin = cube_ref.y;
        double yMax = yMin + width;

        double zMin = cube_ref.z;
        double zMax = zMin + height;

        bool f1 = (length > 0 && (xMin > intersect_point1.x || intersect_point1.x > xMax) ||
                      width > 0 && (yMin > intersect_point1.y || intersect_point1.y > yMax) ||
                      height > 0 && (zMin > intersect_point1.z || intersect_point1.z > zMax));

        bool f2 = (length > 0 && (xMin > intersect_point2.x || intersect_point2.x > xMax) ||
                      width > 0 && (yMin > intersect_point2.y || intersect_point2.y > yMax) ||
                      height > 0 && (zMin > intersect_point2.z || intersect_point2.z > zMax));

        if (f1 && f2)
        {
            return -1;
        }
        else if (f1)
        {
            return t2;
        }
        else if (f2)
        {
            return t1;
        }
        else
        {
            tmp=min(t1,t2);
            return tmp;
        }




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

        point normal=getNormal(intersection_point);
        point reflection=getReflection(r.dir,normal);

        for(int i=0; i<lights.size(); i++)
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
            for(int j=0; j<objects.size(); j++)
            {
                double t=objects[j]->getIntersectingT(L);
                //cout<<t<<endl;
                if(t<=0 || t>dis)continue;
                flag=false;
                //cout<<t<<endl;
                break;
            }
            if(flag==true)
            {
                Ray incident(light_source,intersection_point);
                point light_reflection=getReflection(incident.dir,normal);
                point to_eye;
                to_eye.x=-r.dir.x;
                to_eye.y=-r.dir.y;
                to_eye.z=-r.dir.z;

                double lambert=dot(L.dir,normal);
                double phong=pow(dot(to_eye,light_reflection),Shine);

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
            int nearest=-1;
            double tmin=INT_MAX;
            for(int k=0; k<objects.size(); k++)
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
        cout<<current_color[0]<<" "<<current_color[1]<<" "<<current_color[2]<<endl;
        return tmp;
    }

};

class Triangle:public Object
{
public:
    point p,q,r,n;
    double a,b,c,d;
    Triangle(point x, point y, point z)
    {
        p=x;
        q=y;
        r=z;
        point v1= {q.x-p.x,q.y-p.y,q.z-p.z};
        point v2= {r.x-p.x,r.y-p.y,r.z-p.z};

        this->a=(v1.y*v2.z)-(v1.z*v2.y);
        this->b=(v1.z*v2.x)-(v1.x*v2.z);
        this->c=(v1.x*v2.y)-(v1.y*v2.x);

        n= {a,b,c};
        n=normalize(n);
        a=n.x,b=n.y,c=n.z;

        this->d=-(this->a*p.x)-(this->b*p.y)-(this->c*p.z);

    }

    void draw()
    {

        glBegin(GL_TRIANGLES);
        glColor3f(color[0],color[1],color[2]);
        glVertex3f(p.x,p.y,p.z);
        glVertex3f(q.x,q.y,q.z);
        glVertex3f(r.x,r.y,r.z);
        glEnd();
    }
    point getNormal(Ray &r)
    {
        point df= {a,b,c};
        //cout<<dot(df,r.dir)<<endl;
        if(dot(df,r.dir)>0)df= {-a,-b,-c};
        //if(er==true)return df;
        return df;
    }

    point checkTurn(point a, point b, point c)
    {
        double v1x=b.x-a.x;
        double v1y=b.y-a.y;
        double v1z=b.z-a.z;

        double v2x=c.x-a.x;
        double v2y=c.y-a.y;
        double v2z=c.z-a.z;

        point tmp;
        tmp.x=v1y*v2z-v2y*v1z;
        tmp.y=v1z*v2x-v2z*v1x;
        tmp.z=v1x*v2y-v2x*v1y;
        return tmp;
    }

    int position(point ss)
    {
        point i,j,k;
        i=checkTurn(p,q,ss);
        j=checkTurn(q,r,ss);
        k=checkTurn(r,p,ss);
        if(dot(i,j)>=1-eps && dot(i,k)>1-eps)return 1;

        //else if(checkTurn(t.a,t.b,p)>=0 && checkTurn(t.b,t.c,p)>=0 && checkTurn(t.c,t.a,p)>=0)return 2;
        else return -1;

    }

    double getIntersectingT(Ray &r)
    {

        double t= -(d+dot(n,r.start))/dot(n,r.dir);
        //cout<<t<<endl;
        point tch;
        tch.x=r.start.x+r.dir.x*t;
        tch.y=r.start.y+r.dir.y*t;
        tch.z=r.start.z+r.dir.z*t;
        if(position(tch)==1)return t;
        else return -1;

    }

    double intersect(Ray &r, double *current_color, int level)
    {
        double tmp=getIntersectingT(r);
        //cout<<tmp<<endl;
        if(tmp<=0)return -1;
        if(level==0)return tmp;
        point intersection_point;
        intersection_point.x=r.start.x+r.dir.x*tmp;
        intersection_point.y=r.start.y+r.dir.y*tmp;
        intersection_point.z=r.start.z+r.dir.z*tmp;
        //cout<<intersection_point.z<<endl;


        //cout<<rr<<" "<<g<<" "<<b<<endl;
        current_color[0]=color[0]*co_efficients[0];
        current_color[1]=color[1]*co_efficients[0];
        current_color[2]=color[2]*co_efficients[0];
        //cout<<current_color[0]<<endl;
        point normal=getNormal(r);
        point reflection=getReflection(r.dir,normal);
        //cout<<current_color[0]<<" "<<current_color[1]<<" "<<current_color[2]<<endl;
        for(int i=0; i<lights.size(); i++)
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
            for(int j=0; j<objects.size(); j++)
            {
                double t=objects[j]->getIntersectingT(L);
                //cout<<t<<endl;
                if(t<=0 || t>dis)continue;
                flag=false;
                //cout<<t<<endl;
                break;
            }
            if(flag==true)
            {

                Ray incident(light_source,intersection_point);
                point light_reflection=getReflection(incident.dir,normal);
                point to_eye;
                to_eye.x=-r.dir.x;
                to_eye.y=-r.dir.y;
                to_eye.z=-r.dir.z;

                double lambert=dot(L.dir,normal);
                double phong=pow(dot(to_eye,light_reflection),Shine);
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
            int nearest=-1;
            double tmin=INT_MAX;
            for(int k=0; k<objects.size(); k++)
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

    point getNormal()
    {
        point nrm= {0,0,1};
        return nrm;
    }

    double getIntersectingT(Ray &r)
    {
        point normal=getNormal();
        return -(dot(normal,r.start))/dot(normal,r.dir);
    }

    double intersect(Ray &r, double *current_color, int level)
    {
        double tmp=getIntersectingT(r);
        if(tmp<=0)return -1;
        if(level==0)return tmp;
        point intersection_point;
        intersection_point.x=r.start.x+r.dir.x*tmp;
        intersection_point.y=r.start.y+r.dir.y*tmp;
        intersection_point.z=r.start.z+r.dir.z*tmp;
        //cout<<intersection_point.z<<endl;


        if(intersection_point.x<reference_point.x+eps || intersection_point.x>-reference_point.x-eps ||
                intersection_point.y<reference_point.y+eps || intersection_point.y>-reference_point.y-eps)
            return -1;
        int i=intersection_point.x-reference_point.x;
        int j=intersection_point.y-reference_point.y;
        int k=-reference_point.y-intersection_point.y;
        int ro=i/length;
        int co=j/length;

        if((ro+co)%2==0)
        {
            color[0]=1.0,color[1]=1.0,color[2]=1.0;
        }
        else
        {
            color[0]=0,color[1]=0,color[2]=0;
        }
        //cout<<rr<<" "<<g<<" "<<b<<endl;
        unsigned char red,green,blue;

        txt.get_pixel(i,k,red,green,blue);
        //cout<<(int)red<<" "<<(int)green<<" "<<(int)blue<<endl;
        current_color[0]=color[0]*co_efficients[0]*red/255;
        current_color[1]=color[1]*co_efficients[0]*green/255;
        current_color[2]=color[2]*co_efficients[0]*blue/255;
        point normal=getNormal();
        point reflection=getReflection(r.dir,normal);
        //cout<<color[0]<<endl;
        for(int i=0; i<lights.size(); i++)
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
            for(int j=0; j<objects.size(); j++)
            {
                double t=objects[j]->getIntersectingT(L);
                //cout<<t<<endl;
                if(t<=0 || t>dis)continue;
                flag=false;
                //cout<<t<<endl;
                break;
            }
            if(flag==true)
            {

                Ray incident(light_source,intersection_point);
                point light_reflection=getReflection(incident.dir,normal);
                point to_eye;
                to_eye.x=-r.dir.x;
                to_eye.y=-r.dir.y;
                to_eye.z=-r.dir.z;

                double lambert=dot(L.dir,normal);
                double phong=pow(dot(to_eye,light_reflection),Shine);

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
            int nearest=-1;
            double tmin=INT_MAX;
            for(int k=0; k<objects.size(); k++)
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
    void draw()
    {
        glPushMatrix();
        {
            glScalef(1,1,.001);
        }
        int i=reference_point.x;
        int rr=0,gg=0,bb=0;
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
