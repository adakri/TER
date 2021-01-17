
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
#include "cholesky_adapte.h"

using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

cholesky_adapte::cholesky_adapte (MatrixXd A , VectorXd b )
{
    A_=A ; b_=b ;
}
void cholesky_adapte::Decomposition ()
{
    int n= A_.rows() ;
    L_=MatrixXd::Zero(n,n) ;
    double beta=sqrt(A_(0,0)) , alpha=0 ;
    L_(0,0)=beta ;
    for ( int i =1 ; i < n ; i++)
    {
        alpha=(A_(i,i-1)/beta) ;
        beta = sqrt(A_(i,i)-alpha*alpha) ;
        L_(i,i)=beta ;
        L_(i,i-1)=alpha ;

    }
    //cout << L_ << endl ; 

}
VectorXd cholesky_adapte::Solve ()
{

    this ->Decomposition() ;
    int n=A_.rows() ;
    MatrixXd L(n,n) ;
    VectorXd y(n) ,u(n)  ;
    y(0)=b_(0)/L_(0,0);
    for (int i =1 ; i<n ; i++)
    {
        y(i)=(b_(i)-L_(i,i-1)*y(i-1))/L_(i,i) ;
    }
    L=L_.transpose();
    u(n-1)=y(n-1)/L(n-1,n-1) ;
    for (int i= n-2 ; i>-1 ; i=i-1)
    {
        u(i)=(b_(i)-L(i,i+1)*y(i+1))/L(i,i) ;
    }

    return u ;

}
