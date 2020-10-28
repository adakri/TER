#include <math.h>
#include <fstream>
#include "Dense"

using namespace std;
using namespace Eigen;

std::vector<std::vector<double>> cons(double a,int n,int m)
{
    std ::vector<std:: vector<double>> M(n) ;
    for (int i=0;i<n-1;i++)
    {
        M[i].resize(m);
        for (int j=0;j<m-1;j++)
        {
            M[i][j]=a ;
        }
    }
    return M ;
}




double infini (std:: vector <double> X)
{
    int n=sizeof(X) ;
    double e= abs(X[0]) ;
    for (int i=1;i<n-1;i++)
    {
        if (abs(X[i])> e)
        {
            e=abs(X[i]);
        }
    }
return e;
}


std :: vector<double> prod (std::vector<std::vector<double>> A ,std:: vector<double> X)
{
std::vector <double> Y ;
for (int i=0;i<sizeof(X)-1;i++)
{
    for (int j=0;j<sizeof(X)-1;j++)
    {
    Y[i]=Y[i]+A[i][j]*X[j] ;
    }
}
return Y ;
}


std:: vector<std::vector<double>> inverse_diag (std::vector<std::vector<double>> M)
{
    std::vector <std::vector<double>> M_(sizeof(M));
    for (int i=0;i<sizeof(M)-1;i++)
    {
    M_[i][i]=1.0/M[i][i] ;
    }
return M_ ;
}


std:: vector <double> ver_naive (std::vector <std::vector<double>> A , std::vector <double> b)
{
    int n=sizeof(A) ;
    std:: vector<std::vector<double>> M(n),N(n),M_ ;
    std:: vector <double> X(n),e(n),Y(n);
    double tol=0.001 ;
    X=1;
    e=X ;
    for (int i=0;i<n-1;i++)
    {
        M[i][i]=A[i][i] ;
    }
    e=b ;
    for (int j=0;j<n-1;j++)
    {
    N[j]=M[j]-A[j];
    }

    M_=inverse_diag(M);

    while (infini(e)> tol)
    {
        Y=X;
        X=prod(N,X)+b ;
        X=prod(M_,X) ;
        e=b-prod(A,X) ;
    }
    return X ;
}


std:: vector <double> ver_am (std::vector <std::vector<double>> A , std::vector <double> b)
{
    int n=sizeof(A) ;
    std:: vector<vector<double>> M(n),N(n),M_ ;
    std:: vector <double> X(n),e(n);
    int k=0 ;
    double tol=0.001 ;
    X=1;
    e=b ;
    M=cons(0,n,n);
    for (int i=0;i<n-1;i++)
    {
        M[i][i]=A[i][i] ;
    }
   for (int j=0;j<n-1;j++)
    {
    N[j]=M[j]-A[j];
    }
    M_=inverse_diag(M);

    while (infini(e)> tol and k<1000)
    {
        k=k+1
        X=prod(N,X)+b ;
        X=prod(M_,X) ;
        e=b-prod(A,X) ;
    }
    if ( k>1000)
    {
        return infini(e) ;
    }
    return X ;
}
//gradien à pas optimal
std:: vector <double> ver_am (std::vector <std::vector<double>> A , std::vector <double> b)
{
    int n=sizeof(A) ;
    std:: vector <double> X(n),e(n),z(n);
    int k=0 ;
    double tol=0.001 ;
    e=b
    while (infini(e)> tol and k<1000)
    {
        k=k+1
        z= prod(A,e)
        alpha=(e*e)/(z*e)
        X=X+alpha*e
        e=e-alpha*z
    }
    if ( k>1000)
    {
        return infini(e) ;
    }
    return X ;
}




int main ()
{
std::vector <std::vector<double>> A (2) ;
A=cons(0,2,2);
A[1][1]=1 ;
A[2][2]=1 ;
std::vector <double> X(2) ;
X[1]=1 ;
X[2]=1 ;
X=ver_naive (A,X) ;
std::cout << X << std::endl ;
return 0;

}
