#include "Arnoldi.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
#include "Arnoldi_adapte.h"
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;
Arnoldi_adapte:: Arnoldi_adapte (VectorXd v , int m , MatrixXd A)
{
    v_=v ;
    m_=m ;
    A_=A ;
}
void Arnoldi_adapte::orthogonalisation () 
{
    int n =A_.rows() ;
    V_.resize (n,m_+1) ;
    H_.resize(m_+1,m_) ;
    V_.col(0)=(1/sqrt(v_.dot(v_)))*v_ ;
    H_=MatrixXd::Zero(m_+1,m_) ;
    VectorXd z(n) ;
    for (int j=0 ; j<m_ ; j++)
    {
        
            H_(j,j)= (A_*V_.col(j)).dot(V_.col(j)) ;
            if ( j+1 < m_)
            {
            H_(j,j+1)=(A_*V_.col(j)).dot(V_.col(j+1));
            H_(j+1,j)=H_(j,j+1);
            }
            if ( j==0)
            {
                z=A_*V_.col(j)-H_(j,j)*V_.col(j) ;
            }
            else 
            {
                z=A_*V_.col(j)-H_(j,j)*V_.col(j)-H_(j-1,j)*V_.col(j-1) ;
            }
            if ( j==m_-1)
            {
                H_(j+1,j)=z.norm();
            }

            V_.col(j+1)=z/H_(j+1,j) ;
    }
    
}
MatrixXd  Arnoldi_adapte::get_H () 
{
    return H_ ;
}
MatrixXd Arnoldi_adapte :: get_V () 
{
    return V_ ;
}