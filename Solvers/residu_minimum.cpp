#include "residu_minimum.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;
residu_minimum::residu_minimum(MatrixXd A,VectorXd b , int kmax ) :  A_(A),b_(b),kmax_(kmax)
{	

}

VectorXd residu_minimum::Solve ()const
{
    
    int p = A_.rows();
	VectorXd u(p);											
	for (int i = 0; i < p; i++)
	{
		u[i]=0.;
		
	}
    VectorXd r=b_ ;
    int k=0 ;
    double Tol=0.0001 ;
    VectorXd z ;
    double alpha ;
    while ( k<=kmax_ && sqrt(r.dot(r))>Tol ) 
    {
        z=A_*r;
        alpha=(r.dot(z))/(z.dot(z));
        u=u+alpha*r ;
        r=r-alpha*z ;
        k=k+1 ;
    }
    return u ;
    if ( k>kmax_ ) 
    {
        std::cout<< " Tolérance non atteinte "<< sqrt(r.dot(r)) <<std::endl ;

    }


}
