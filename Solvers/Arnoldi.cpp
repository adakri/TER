#include "Arnoldi.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;
Arnoldi:: Arnoldi (VectorXd v , int m , MatrixXd A)
{
    v_=v ;
    m_=m ;
    A_=A ;
}
void Arnoldi::orthogonalisation () 
{
    int n =A_.rows() ;
    V_.resize (n,m_+1) ;
    H_.resize(m_+1,m_) ;
    V_.col(0)=(1/sqrt(v_.dot(v_)))*v_ ;
    H_=MatrixXd::Zero(m_+1,m_) ;
    VectorXd z(n) ;
    for (int j=0 ; j<m_ ; j++)
    {
        for (int i=0 ; i<j+1; i++)
        {
            H_(i,j)= (A_*V_.col(j)).dot(V_.col(i)) ;
        }
    
    z=A_*V_.col(j);
    
    for (int k=0 ; k<j+1 ; k++ )
        {
        z=z-H_(k,j)*V_.col(k) ;
        }
    H_(j+1,j)=z.norm() ;
    
    if ( H_(j+1,j)==0 )
    {
        break ;
    }
    

    V_.col(j+1)=(1./H_(j+1,j))*z ;
    
    
    }    
    
}
MatrixXd  Arnoldi::get_H () 
{
    return H_ ;
}
MatrixXd Arnoldi :: get_V () 
{
    return V_ ;
}