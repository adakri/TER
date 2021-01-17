#include "systeme_triangulaire_sup.h"
#include "Householder.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

Householder::Householder(MatrixXd A,VectorXd b) :  A_(A),b_(b)
{	
	
}
void Householder::Decomposition_QR ()
{
   
   double beta=0 ;
   int n =A_.rows() ;
   int m=A_.cols() ;
   MatrixXd P (n,m*n),P1(n,n);
   R_.resize (n,m); Q_.resize(n,n) ;
    MatrixXd I(n,n) ;
    I=MatrixXd::Identity(n,n) ;
    Q_=I ;
    R_=A_ ;
    VectorXd z(n),w(n)  ;
    for (int k=0;k<m;k++)
    {
        if(k>0)
        {
            for (int i=0 ; i< k ; i++)
                {
                    P1=P.block(0,i*n,n,n) ;
                    R_.col(k)=P1*R_.col(k);
                    
                }
        }
        beta=0 ;
        if ( A_(k,k)!=0)
        {
        for (int i=k ; i< n; i++)
        {
            beta=sqrt(beta*beta+A_(i,k)*A_(i,k)) ;
        }
        
        beta=(A_(k,k)/abs(A_(k,k)))*beta ;

        }

        for (int j=0 ; j< n ;j++) 
        {
            if (j<k)
            {
                z(j)=0 ;
            }
            if(j==k)
            {
                z(j)=beta+A_(j,j) ;
            }
            if (j>k)
            {
                z(j)=A_(j,k) ;
            }
        }
        w=(1/sqrt(z.dot(z)))*z ;
        P1=I-2*w*w.transpose() ;
        P.block(0,k*n,n,n)=P1 ;
        R_.col(k)=P1*R_.col(k);
        for (int i=0 ; i< k+1 ; i++)
                {
                    P1=P.block(0,(k-i)*n,n,n);
                    Q_.col(k)=P1*Q_.col(k);
                    
                }
       
    }

}
VectorXd Householder::Solve () 
{
    systeme_triangulaire_sup u(R_,b_);
    VectorXd x=u.Solve() ;
    x=Q_.transpose()*x ;
    return x ;
}
MatrixXd Householder::get_Q () 
{
    return Q_ ;
}
MatrixXd Householder::get_R()
{
    return R_ ;
}