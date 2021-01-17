#include "FOM.h"
#include "Householder.h"
#include "Arnoldi.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;


FOM::FOM(MatrixXd A,VectorXd b,int m, int kmax) :  A_(A),b_(b),m_(m),kmax_(kmax)
{	
	
}

VectorXd FOM::Solve()const								
{
    int p = A_.rows();
	VectorXd u(p), e1(m_),em(m_);											
	for (int i = 0; i < p; i++)
	{
		u[i]=0.;
        if (i<m_)
        {
        e1[i]=0 ;
        em[i]=0 ;
        }
	}
    e1[0]=1 ; em[m_-1]=1 ;
    VectorXd r(p) ;
    r=b_ ;
    double beta=sqrt(r.dot(r)) ;
    int k=0 ;
    double Tol=0.1  ;
    MatrixXd H_bar(m_+1,m_) ,V_bar(p,m_+1) , V (p,m_) , H (m_,m_);
    VectorXd y(m_) ;
    Arnoldi H_V (r,m_,A_);

    while ( k<=kmax_)
    {
        Arnoldi H_V (r,m_,A_);
        H_V.orthogonalisation() ;
        H_bar= H_V.get_H() ; V_bar=H_V.get_V() ; 
        V=V_bar.block(0,0,p,m_) ;
        H=H_bar.block(0,0,m_,m_) ;
        Householder hld ( H , beta*e1) ;
        hld.Decomposition_QR () ;
        y=hld.Solve() ;
        u=u+V*y;
        r=b_- A_*u ;
        beta=r.norm() ;
        k=k+1 ;

    }

    
    
    return u ;

}