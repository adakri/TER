#include "GMRes.h"
#include "systeme_triangulaire_sup.h"
#include "Householder.h"
#include "Arnoldi.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;


GMRes::GMRes(MatrixXd A,VectorXd b,int m,int kmax) :  A_(A),b_(b),m_(m), kmax_(kmax)
{	
	
}

VectorXd GMRes::Solve()const								
{
    int p = A_.rows();
	VectorXd u(p),e1(m_+1);											
	for (int i = 0; i < p; i++)
	{
		u[i]=0.;
        if (i<m_+1)
        {
            e1[i]=0 ;
        }
	}
    e1[0]=1;
    VectorXd r(p)  ;
    r=b_ ;
    double beta=sqrt(r.dot(r)) ;
    int k=0 ;
    double Tol=0.001  ;
    MatrixXd H(m_+1,m_) ,V(p,m_+1) , R (m_,m_) ,Q(m_,m_),R_bar(m_+1,m_) ,Q_bar(m_+1,m_+1) ;
    VectorXd y(m_) ;
    Arnoldi H_V (r,m_,A_);

    while ( k<=kmax_)
    {
        Arnoldi H_V (r,m_,A_);
        H_V.orthogonalisation() ;
        H= H_V.get_H() ; V=H_V.get_V() ;
        V=V.block(0,0,p,m_) ;
        Householder hld ( H , beta*e1) ;
        hld.Decomposition_QR () ;
        R_bar=hld.get_R() ;
        Q_bar=hld.get_Q() ;
        Q=Q_bar.block(0,0,m_+1,m_);
        R=R_bar.block(0,0,m_,m_) ;
        systeme_triangulaire_sup sys (R,beta*Q.transpose()*e1) ;
        y = sys.Solve() ;
        u=u+V*y;
        r=b_-A_*u;      
        beta=sqrt(r.dot(r)) ;
        V.resize(p,m_+1);
        k=k+1 ;

    }

    
    return u ;
}