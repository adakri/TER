#include "systeme_triangulaire_sup.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;


#include "GradConj.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;


systeme_triangulaire_sup::systeme_triangulaire_sup(MatrixXd A,VectorXd b) :  A_(A),b_(b)
{	
	
}

VectorXd systeme_triangulaire_sup::Solve()const								
{
    
    int n=A_.rows() ;
    int m=A_.cols() ;
    VectorXd X(m) ;
    for (int k=0 ; k <n ; k++)
    {
        X(m-1-k)=b_(n-1-k) ;
        if (k>0)
        {
        for (int i=n-k; i<n; i++)
        {
            X(n-1-k)=X(n-1-k)-A_(n-1-k,i)*X(i) ;
        }
        }
        X(n-1-k)=X(n-1-k)/A_(n-1-k,n-1-k) ;
    }
    return X ;
}



