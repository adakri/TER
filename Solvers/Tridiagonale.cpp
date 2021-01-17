#include "Tridiagonale.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

Tridiagonale::Tridiagonale  (double a , double b , double c ,int n ) 
{
    a_=a ; b_=b ;c_=c; n_=n ;
    M_=MatrixXd::Zero (n_,n_);
    for (int k=0 ; k< n_ ; k++)
    {
        if ( k > 0 && k< n_-1)
        {
            M_(k,k-1)=a_ ;
            M_(k,k)=b_ ;
            M_(k,k+1) = c_ ;
        }
        M_(0,0)=b_ ;
        M_(0,1)=c_ ;
        M_(n_-1,n_-1)=b_ ;
        M_ (n_-1,n_-2)=a_ ;
    }
}

MatrixXd  Tridiagonale::get_matrix() 
{
    return M_ ;
}
