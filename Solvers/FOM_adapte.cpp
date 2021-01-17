#include "FOM_adapte.h"
#include "cholesky_adapte.h"
#include "Arnoldi_adapte.h"
#include "SDP.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>

using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;


FOM_adapte::FOM_adapte(MatrixXd A,VectorXd b,int m, int kmax) :  A_(A),b_(b),m_(m),kmax_(kmax)
{

}

VectorXd FOM_adapte::Solve()
{
    int p = A_.rows();
	VectorXd u(p),e1(m_);
	for (int i = 0; i < p; i++)
	{
		u[i]=0.;
        b_[i]=1. ;
        if (i<m_)
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
    MatrixXd H_bar(m_+1,m_) ,V_bar(p,m_+1) ,V(p,m_) , H(m_,m_) , R (m_,m_) ,Q(m_,m_),R_bar(m_+1,m_) ,Q_bar(m_+1,m_+1) ;
    VectorXd y(m_) ;
    Arnoldi_adapte H_V (r,m_,A_);

    while ( k<=kmax_)
    {
        Arnoldi_adapte H_V (r,m_,A_);
        H_V.orthogonalisation() ;
        H_bar= H_V.get_H() ; V_bar=H_V.get_V() ;
        V=V_bar.block(0,0,p,m_) ;
        H=(V.transpose())*A_*V;
        cout<<H_bar<<endl;
        cout<<"---------------"<<endl;
        cout<<V_bar<<endl;

        //cout << H << endl ;
        // LLT<MatrixXd> llt;
        // llt.compute(H);
        // y=llt.solve(beta*e1) ;
        cholesky_adapte chol(H,beta*e1);
        y=chol.Solve();
        u=u+V*y;
        r=b_- A_*u ;
        beta=r.norm() ;
        k=k+1 ;

    }



    return u ;

}
