#include "SDP.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

 SDP::SDP (const MatrixXd& A ) 
 {
     _A=A ;
 }
 bool SDP::check ( )
 {
     if ( _A==_A.transpose())
     {
          SelfAdjointEigenSolver<MatrixXd> es(_A);
          for (int i=0 ; i < _A.rows();i++)
          {
              if (es.eigenvalues()(i)<= 0)
              {
                  return false ;
              }

          }
            return true ;
         
     }
     return false;
 }