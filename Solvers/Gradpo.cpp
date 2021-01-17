#include "Gradpo.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

//constructeur
Gradpo::Gradpo(MatrixXd A,VectorXd b,int kmax) :  A_(A),b_(b), kmax_(kmax)
{	
}


//gradient à pas optimal
VectorXd Gradpo::Solve()const								
{
	
	MatrixXd A=A_;
	int n = A.rows();
	VectorXd x(n);											
	for (int i = 0; i < n; i++)
	{
		x[i]=0.;
		
	}

	VectorXd r(n),b(b_) ;
	r = b -A*x;
	double alpha;
	VectorXd rSuivant(n);
	VectorXd xSuivant(n);
	VectorXd z(n);
	double beta=r.norm();
	int k=0;
	while (k<kmax_)
	{
		// calcul des nouveau paramètres 

		z=A*r;
		alpha= (r.dot(r) )  / (z.dot(r)) ;
		xSuivant= x + alpha*r;
        rSuivant=r-alpha*z;
		
		// mise à jour des paramètres 

		x=xSuivant ;
		r=rSuivant;
		beta=r.norm();
		k=k +1;
	}
	return x;
}