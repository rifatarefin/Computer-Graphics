#define pi acos(-1)
#define eps 1e-5
#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;
ifstream fin("stage3.txt");
ifstream fconfig("config.txt");
ofstream fout("z_buffer.txt");

double screen_width,screen_height;
double limit_x,limit_y,front_z,rear_z;
double dx,dy;
double top_y,left_x;
double **arr;

struct point
{
    double x;
    double y;
    double z;
};

class Triangle
{
public:
    point a,b,c;
    int R,G,B;
    Triangle(point p, point q, point r)
    {
        a=p;
        b=q;
        c=r;
        R=rand()%256;
        G=rand()%256;
        B=rand()%256;
    }
};
vector<Triangle>trlist;

void printTriangle(Triangle t)
{
    cout<<t.a.x<<" "<<t.a.y<<" "<<t.a.z<<endl;
    cout<<t.b.x<<" "<<t.b.y<<" "<<t.b.z<<endl;
    cout<<t.c.x<<" "<<t.c.y<<" "<<t.c.z<<endl;
    cout<<"****\n";
}
void input()
{
    fconfig>>screen_width>>screen_height;
    fconfig>>limit_x>>limit_y;
    fconfig>>front_z>>rear_z;

    while(true)
    {
        point p,q,r;
        fin>>p.x>>p.y>>p.z;
        fin>>q.x>>q.y>>q.z;
        fin>>r.x>>r.y>>r.z;
        Triangle t(p,q,r);
        trlist.push_back(t);
        if(fin.eof())break;
    }

    arr=new double*[(int)screen_width];
    for(int i=0; i<screen_width; i++)
    {
        arr[i]=new double[(int)screen_height];
        for(int j=0; j<screen_height; j++)
        {
            arr[i][j]=rear_z;
        }
    }

    dx=(limit_x*-2)/screen_width;
    dy=(limit_y*-2)/screen_height;
    top_y=-limit_y-(dy/2);
    left_x=limit_x+(dx/2);
}

void lineFromPoints(point P, point Q, double &a,
                    double &b, double &c)
{
    a = Q.y - P.y;
    b = P.x - Q.x;
    c = a*(P.x)+ b*(P.y);
}

point lineLineIntersection(double a1, double b1, double c1,
                           double a2, double b2, double c2)
{
    double determinant = a1*b2 - a2*b1;
    point a;
    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX

        return a= {FLT_MAX, FLT_MAX};
    }

    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return a= {x, y};
    }
}

int main()
{
    input();
    bitmap_image image(screen_width,screen_height);



    for(vector<Triangle>::iterator it=trlist.begin(); it!=trlist.end(); it++)

    {
        Triangle tr=*it;

        if(min(min(tr.a.y,tr.b.y),tr.c.y)>-limit_y)continue;
        if(max(max(tr.a.y,tr.b.y),tr.c.y)<limit_y)continue;

        double top=min(-limit_y,max(max(tr.a.y,tr.b.y),tr.c.y));
        double bottom=max(limit_y,min(min(tr.a.y,tr.b.y),tr.c.y));

        int top_row=(-limit_y-top)/dy;
        int bottom_row=(-limit_y-bottom)/dy;

        top=ceil((top/dy));
        top=top*dy-(dy/2);

        //plane eqn
        point v1={tr.b.x-tr.a.x,tr.b.y-tr.a.y,tr.b.z-tr.a.z};
        point v2={tr.c.x-tr.a.x,tr.c.y-tr.a.y,tr.c.z-tr.a.z};
        point normal;
        normal.x=(v1.y*v2.z)-(v1.z*v2.y);
        normal.y=(v1.z*v2.x)-(v1.x*v2.z);
        normal.z=(v1.x*v2.y)-(v1.y*v2.x);

        double d=-(normal.x*tr.a.x)-(normal.y*tr.a.y)-(normal.z*tr.a.z);
        cout<<normal.x<<" "<<normal.y<<" "<<normal.z<<" "<<d<<endl;


        for(int i=top_row; i<bottom_row; i++,top-=dy)
        {
            double a1,a2,a3,b1,b2,b3,c1,c2,c3;
            lineFromPoints(tr.a,tr.b,a1,a2,a3);
            lineFromPoints(tr.b,tr.c,b1,b2,b3);
            lineFromPoints(tr.c,tr.a,c1,c2,c3);
            double d1=0,d2=1,d3=top;

            point p1=lineLineIntersection(a1,a2,a3,d1,d2,d3);
            point p2=lineLineIntersection(b1,b2,b3,d1,d2,d3);
            point p3=lineLineIntersection(c1,c2,c3,d1,d2,d3);

            point lt,rt;
            if(p1.x<=min(tr.a.x,tr.b.x)-eps || p1.x>=max(tr.a.x,tr.b.x)-eps)
            {
                lt=p2;
                rt=p3;
            }
            else if(p2.x<=min(tr.b.x,tr.c.x)-eps || p2.x>=max(tr.b.x,tr.c.x)-eps)
            {
                lt=p3;
                rt=p1;
            }
            else if(p3.x<=min(tr.c.x,tr.a.x)-eps || p3.x>=max(tr.c.x,tr.a.x)-eps)
            {
                lt=p1;
                rt=p2;
            }

            double left_most,right_most;
            left_most=lt.x<=rt.x?lt.x:rt.x;
            right_most=rt.x>lt.x?rt.x:lt.x;

            //if(rt.x==FLT_MAX)continue;
            if(right_most<limit_x)continue;
            if(left_most>-limit_x)continue;

            left_most=max(left_most,limit_x);
            right_most=min(right_most,-limit_x);

            int left_col=(left_most-limit_x)/dx;
            int right_col=(right_most-limit_x)/dx;

            left_most=floor((left_most/dx));

            left_most=left_most*dx+(dx/2);

            cout<<left_most<<endl;

            for(int j=left_col;j<right_col;j++,left_most+=dx)
            {

                double z=(-(normal.x*left_most)-(normal.y*top)-d)/normal.z;
                cout<<z;
                if(z<arr[i][j])

                {
                    arr[i][j]=z;
                    image.set_pixel(j,i,tr.R,tr.G,tr.B);
                }
            }
        }

    }
    image.save_image("1.bmp");;
    image.clear();
    for(int i=0;i<screen_height;i++)
    {
        for(int j=0;j<screen_width;j++)
        {
            if(arr[i][j]<rear_z)
            {
                fout<<fixed<<setprecision(7)<<arr[i][j]<<"\t";
            }
        }fout<<endl;
    }
    trlist.clear();
    for(int i=0;i<screen_height;i++)
        delete[] arr[i];
    delete[] arr;

//    bitmap_image image(500,300);
//
//    for(int i=0;i<200;i++){
//        for(int j=0;j<100;j++){
//            image.set_pixel(i,j,255,255,0);
//        }
//    }
//
//    image.save_image("test.bmp");;

}
