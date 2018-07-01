#define pi acos(-1)
#define eps 1e-5
#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;
ifstream fin("stage3.txt");
ifstream fconfig("config.txt");

double screen_width,screen_height;
double limit_x,limit_y,front_z,rear_z;
double dx,dy,top=FLT_MIN,bottom=FLT_MAX;

struct point
{
    double x;
    double y;
    double z;
};

class polygon
{
public:
    point x,y,z;
    double a,b,c,d;
    int id;
    bool inout=false;
    int R,G,B;
    polygon(point a, point b, point c, int i)
    {
        x=a;
        y=b;
        z=c;
        id=i;
        point v1= {b.x-a.x,b.y-a.y,b.z-a.z};
        point v2= {c.x-a.x,c.y-a.y,c.z-a.z};

        this->a=(v1.y*v2.z)-(v1.z*v2.y);
        this->b=(v1.z*v2.x)-(v1.x*v2.z);
        this->c=(v1.x*v2.y)-(v1.y*v2.x);

        this->d=-(this->a*a.x)-(this->b*a.y)-(this->c*a.z);

        R=rand()%256;
        G=rand()%256;
        B=rand()%256;
    }
};

class Edge
{
public:

    double ymax,ymin;
    double xat_ymin;
    double del_x;
    int id;
    Edge(point a, point b, int i)
    {
        point ma=a.y>=b.y?a:b;
        point mi=a.y<b.y?a:b;
        double s1=ma.x-mi.x;
        double s2=ma.y-mi.y;
        del_x=(s1*dy)/s2;
        ymax=ma.y;
        ymin=mi.y;
        xat_ymin=mi.x;

        id=i;
    }
};
vector<Edge>ET;
vector<polygon>PT;

inline bool operator==(const point& lhs, const point& rhs)
{
    return lhs.x == rhs.x  && lhs.y==rhs.y && lhs.z==rhs.z ;
}

inline bool operator==(const polygon& lhs, const polygon& rhs)
{
    return lhs.x == rhs.x  && lhs.y==rhs.y && lhs.z==rhs.z ;
}

bool compareByY(const Edge &a, const Edge &b)
{
    return a.ymin < b.ymin;
}

bool compare2(const Edge &a, const Edge &b)
{
    return a.xat_ymin < b.xat_ymin;
}
void input()
{
    fconfig>>screen_width>>screen_height;
    fconfig>>limit_x>>limit_y;
    fconfig>>front_z>>rear_z;

    dx=(limit_x*-2)/screen_width;
    dy=(limit_y*-2)/screen_height;
    int i=0;
    while(true)
    {
        point p,q,r;
        fin>>p.x>>p.y>>p.z;
        if(p.y>top)top=p.y;
        if(p.y<bottom)bottom=p.y;

        fin>>q.x>>q.y>>q.z;
        if(q.y>top)top=q.y;
        if(q.y<bottom)bottom=q.y;

        fin>>r.x>>r.y>>r.z;
        if(r.y>top)top=r.y;
        if(r.y<bottom)bottom=r.y;

        if(p.y!=q.y)
        {
            Edge e1(p,q,i);
            ET.push_back(e1);
        }
        if(q.y!=r.y)
        {
            Edge e2(q,r,i);
            ET.push_back(e2);
        }
        if(r.y!=p.y)
        {
            Edge e3(r,p,i);
            ET.push_back(e3);
        }
        polygon p1(p,q,r,i);
        PT.push_back(p1);
        if(fin.eof())break;
        i++;
    }

    top=min(-limit_y,top);
    bottom=max(limit_y,bottom);
    bottom=floor(bottom/dy);
    bottom*=dy;
//    top_y=-limit_y-(dy/2);
//    left_x=limit_x+(dx/2);
}
int main()
{
    input();
    sort(ET.begin(),ET.end(),compareByY);
    int len_e=ET.size();
    vector<Edge>AEL;
    vector<polygon>APL;
    vector<int>rec;
    int lenP=PT.size();
    bitmap_image image(screen_width,screen_height);
    //cout<<bottom<<endl;
    //cout<<top<<" "<<bottom<<" "<<top/dy;
    //for(int i=0;i<len_e;i++)cout<<ET[i].xat_ymin<<" "<<ET[i].ymin<<endl;

    for(double i=bottom; i<=top; i=i+dy) //line scan
    {
        AEL.clear();
        APL.clear();
        rec.clear();
        for(int j=0; j<len_e; j++)
        {
            Edge e=ET[j];
            if(i>=e.ymin && i<=e.ymax)
            {
                AEL.push_back(e);
                rec.push_back(j);
                //cout<<e.xat_ymin<<" "<<e.ymin<<endl;
            }


        }
        sort(AEL.begin(),AEL.end(),compare2);
//        for(int i=0;i<len_e;i++)cout<<AEL[i].xat_ymin<<" "<<AEL[i].ymin<<endl;
        cout<<AEL[0].xat_ymin<<endl;
        int len2=AEL.size();
        for(int k=0; k<len2-1; k++)        //edge loop
        {
            Edge e=AEL[k];
            PT[e.id].inout=!PT[e.id].inout;
            if(PT[e.id].inout==true)APL.push_back(PT[e.id]);
            else
            {
                vector<polygon>::iterator it=find(APL.begin(),APL.end(),PT[e.id]);
                if(it!=APL.end())APL.erase(it);
            }
            double z=FLT_MAX;
            vector<polygon>::iterator pm;
            for(vector<polygon>::iterator it=APL.begin();it!=APL.end();it++)
            {

                polygon po=*it;
                double pp=(-(e.xat_ymin*po.a)-(i*po.b)-po.d)/po.c;
                if(pp<z)
                {
                    z=pp;

                    pm=it;
                }

            }
            //cout<<z<<endl;
            //color
            double lf=max(limit_x,e.xat_ymin);
            double rf=min(-limit_x,AEL[k+1].xat_ymin);
            //cout<<lf<<" "<<rf<<endl;
            int left_col=(lf-limit_x)/dx;
            int right_col=(rf-limit_x)/dx;
            //cout<<left_col<< " "<<right_col<<endl;
            int row=(-limit_y-i)/dy;
            //cout<<row<<endl;
            for(int col=left_col;col<right_col;col++)
                image.set_pixel(col,row,(*pm).R,(*pm).G,(*pm).B);




        }
        PT[AEL[len2-1].id].inout=false;
        int j=0;
        for(vector<Edge>::iterator it=AEL.begin();it!=AEL.end();it++,j++)
        {
            ET[rec[j]].xat_ymin+=ET[rec[j]].del_x;
        }

    }
    image.save_image("2.bmp");

}
