#include "Arnoldi.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

double Norme1  ( MatrixXd A )
{
    double sum=0 , sum1  ;
    int n =A.rows() ;
    for (int i=0 ; i< n ; i++) 
    {
        sum1=0 ;
        for (int j=0 ; j< n ; j++)
        {
            sum1+=A(i,j) ;
        }
        sum1=abs(sum1) ;
        if ( sum1 > sum )
        {
            sum=sum1 ;
        }

    }
    return sum ;
}