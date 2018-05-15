#define pi acos(-1)
#include<bits/stdc++.h>
using namespace std;
ifstream fin("scene.txt");
ofstream stage1("stage1.txt");
ifstream stage1in("stage1.txt");
ofstream stage2("stage2.txt");
ifstream stage2in("stage2.txt");
ofstream stage3("stage3.txt");


struct point
{
    double x;
    double y;
    double z;
};

class Matrix
{


public :
    int row,column;
    double **mat;
    Matrix(int row, int column)
    {
        this->row=row;
        this->column=column;
        mat=new double*[row];
        for(int i=0; i<row; i++)mat[i]=new double[column];

    }

    Matrix(const Matrix &ob)
    {
        this->row=ob.row;
        this->column=ob.column;
        this->mat=new double*[this->row];
        for(int i=0; i<row; i++)this->mat[i]=new double[this->column];
        for(int i=0;i<this->row;i++)
            for(int j=0;j<this->column;j++)
            this->mat[i][j]=ob.mat[i][j];
    }

//    ~Matrix()
//    {
//        for(int i=0;i<row;i++)
//        {
//            delete[] mat[i];
//            cout<<row<<endl;
//        }
//        delete[] mat;
//    }

    void identity()
    {
        for(int i=0; i<row; i++)
        {
            for(int j=0; j<column; j++)
            {
                if(i==j)mat[i][j]=1;
                else mat[i][j]=0;
            }
        }
    }
    void multiply(Matrix b)
    {

        double **mult=new double*[row];
        for(int i=0; i<row; i++)mult[i]=new double[b.column];

        for(int i=0; i<row; i++)
        {

            for(int j=0; j<b.column; j++)
            {
                mult[i][j]=0;
                for(int k=0; k<column; k++)
                {
                    mult[i][j]+=mat[i][k]*b.mat[k][j];
                }

            }
        }
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<b.column;j++)
                mat[i][j]=mult[i][j];
        }

        for(int i=0; i<row; i++)
        {
            delete[] mult[i];
        }
        delete[] mult;


    }
    double **pointMultiply(Matrix b)
    {
        double **mult=new double*[row];
        for(int i=0; i<row; i++)mult[i]=new double[b.column];

        for(int i=0;i<row;i++)
        {
            for(int j=0;j<b.column;j++)mult[i][j]=0;
        }

        for(int i=0; i<row; i++)
        {

            for(int j=0; j<b.column; j++)
            {

                for(int k=0; k<column; k++)
                {
                    mult[i][j]+=mat[i][k]*b.mat[k][j];
                }

            }
        }

        return mult;
    }
};

Matrix makeTranslation(double x,double y, double z)
{
    Matrix t(4,4);
    t.identity();
    t.mat[0][3]=x;
    t.mat[1][3]=y;
    t.mat[2][3]=z;
    return t;
}

Matrix makeScale(double x,double y, double z)
{
    Matrix t(4,4);
    t.identity();
    t.mat[0][0]=x;
    t.mat[1][1]=y;
    t.mat[2][2]=z;
    return t;
}

Matrix makeRotate(double angle, double x,double y, double z)
{
    Matrix t(4,4);

    double cs,ss;

        angle = angle*pi/180;
        cs=cos(angle);ss=sin(angle);

    t.mat[0][0]=cs+(1-cs)*x*x;
    t.mat[0][1]=(1-cs)*x*y-ss*z;
    t.mat[0][2]=(1-cs)*x*z+ss*y;
    t.mat[0][3]=0;
    t.mat[1][0]=(1-cs)*x*y+ss*z;
    t.mat[1][1]=cs+(1-cs)*y*y;
    t.mat[1][2]=(1-cs)*y*z-ss*x;
    t.mat[1][3]=0;
    t.mat[2][0]=(1-cs)*x*z-ss*y;
    t.mat[2][1]=(1-cs)*y*z+ss*x;
    t.mat[2][2]=cs+(1-cs)*z*z;
    t.mat[2][3]=0;
    t.mat[3][0]=0;
    t.mat[3][1]=0;
    t.mat[3][2]=0;
    t.mat[3][3]=1;


    return t;
}

Matrix makeTriangle(double x,double y, double z)
{
    Matrix t(4,1);
    t.mat[0][0]=x;
    t.mat[1][0]=y;
    t.mat[2][0]=z;
    t.mat[3][0]=1;
    return t;
}



stack<Matrix>S;
stack<stack<Matrix> >All;
point eye, look, upp;
double fovy, aspectR, near, far;

point cross(point a, point b)
{
    point tmp;
    tmp.x=a.y*b.z-a.z*b.y;
    tmp.y=a.z*b.x-a.x*b.z;
    tmp.z=a.x*b.y-a.y*b.x;
    return tmp;
}

Matrix getView()
{
    double lx=look.x-eye.x;
    double ly=look.y-eye.y;
    double lz=look.z-eye.z;

    double w=sqrt(lx*lx+ly*ly+lz*lz);
    point l;
    l.x=lx/w;
    l.y=ly/w;
    l.z=lz/w;

    point r=cross(l,upp);
    w=sqrt(r.x*r.x+r.y*r.y+r.z*r.z);
    r.x/=w;
    r.y/=w;
    r.z/=w;

    point u=cross(r,l);
    Matrix v(4,4);
    v.mat[0][0]=r.x;
    v.mat[0][1]=r.y;
    v.mat[0][2]=r.z;
    v.mat[0][3]=0;
    v.mat[1][0]=u.x;
    v.mat[1][1]=u.y;
    v.mat[1][2]=u.z;
    v.mat[1][3]=0;
    v.mat[2][0]=-l.x;
    v.mat[2][1]=-l.y;
    v.mat[2][2]=-l.z;
    v.mat[2][3]=0;
    v.mat[3][0]=0;
    v.mat[3][1]=0;
    v.mat[3][2]=0;
    v.mat[3][3]=1;

    Matrix t=makeTranslation(-eye.x,-eye.y,-eye.z);
    v.multiply(t);
    return v;

}

Matrix getProjection()
{
    double fovx=fovy*aspectR;
    double t=near*tan(fovy*pi/360);
    double r=near*tan(fovx*pi/360);

    Matrix ac(4,4);
    ac.mat[0][0]=near/r;
    ac.mat[0][1]=0;
    ac.mat[0][2]=0;
    ac.mat[0][3]=0;

    ac.mat[1][0]=0;
    ac.mat[1][1]=near/t;
    ac.mat[1][2]=0;
    ac.mat[1][3]=0;

    ac.mat[2][0]=0;
    ac.mat[2][1]=0;
    ac.mat[2][2]=-(far+near)/(far-near);
    ac.mat[2][3]=-(2*far*near)/(far-near);

    ac.mat[3][0]=0;
    ac.mat[3][1]=0;
    ac.mat[3][2]=-1;
    ac.mat[3][3]=0;

    return ac;

}

void input()
{
    double x,y,z;
    fin>>eye.x>>eye.y>>eye.z;
    fin>>look.x>>look.y>>look.z;
    fin>>upp.x>>upp.y>>upp.z;
    fin>>fovy>>aspectR>>near>>far;

}
void printTop()
{
    Matrix t=S.top();
    for(int i=0; i<t.row; i++)
    {
        for(int j=0; j<t.column; j++)
            cout<<t.mat[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl<<endl;
}

int main()
{
    input();
    Matrix m(4,4);
    m.identity();
    S.push(m);
    string command;
    while(true)
    {
        fin>>command;
        //cout<<command<<endl;
        if(command=="translate")
        {
            Matrix t=S.top();

            double x,y,z;
            fin>>x>>y>>z;
            Matrix b=makeTranslation(x,y,z);

            t.multiply(b);
            S.push(t);

        }
        else if(command=="scale")
        {
            Matrix t=S.top();

            double x,y,z;
            fin>>x>>y>>z;
            Matrix b=makeScale(x,y,z);
            t.multiply(b);
            S.push(t);
        }
        else if(command=="rotate")
        {
            Matrix t=S.top();

            double angle,x,y,z;
            fin>>angle>>x>>y>>z;
            double xz=sqrt(x*x+y*y+z*z);
            x=x/xz;
            y=y/xz;
            z=z/xz;
            Matrix b=makeRotate(angle,x,y,z);


            t.multiply(b);
            S.push(t);
        }
        else if(command=="triangle")
        {

            for(int i=0; i<3; i++)
            {
                Matrix t=S.top();
                double x,y,z;
                fin>>x>>y>>z;
                Matrix b=makeTriangle(x,y,z);

                t.multiply(b);

                double w=t.mat[3][0];
                stage1<<fixed<<setprecision(7)<<t.mat[0][0]/w<<" "<<t.mat[1][0]/w<<" "<<t.mat[2][0]/w<<endl;
            }
            stage1<<"\n";
        }
        else if(command=="push")
        {
            All.push(S);
        }
        else if(command=="pop")
        {
            if(All.size()>0)
            {
                S=All.top();
                All.pop();
            }
        }
        else if(command=="end")break;

        //printTop();


    }
    int i=0;
    Matrix t=getView();
    while(true)
    {
        double x,y,z;
        stage1in>>x>>y>>z;
        if(stage1in.eof())break;


        Matrix b=makeTriangle(x,y,z);
        //t.multiply(b);
        double **arr=t.pointMultiply(b);

        double w=arr[3][0];
        stage2<<fixed<<setprecision(7)<<arr[0][0]/w<<" "<<arr[1][0]/w<<" "<<arr[2][0]/w<<endl;

        i++;
        if(i%3==0)stage2<<endl;
        for(int i=0;i<4;i++)delete[] arr[i];
        delete[] arr;

    }

    i=0;
    t=getProjection();
    while(true)
    {
        double x,y,z;
        stage2in>>x>>y>>z;
        if(stage2in.eof())break;

        Matrix b=makeTriangle(x,y,z);
        double **arr=t.pointMultiply(b);

        double w=arr[3][0];
        stage3<<fixed<<setprecision(7)<<arr[0][0]/w<<" "<<arr[1][0]/w<<" "<<arr[2][0]/w<<endl;

        i++;
        if(i%3==0)stage3<<endl;
        for(int i=0;i<4;i++)delete[] arr[i];
        delete[] arr;

    }


}
