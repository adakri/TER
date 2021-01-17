#include "GradConj.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;


GradConj::GradConj(MatrixXd A,VectorXd b, int jmax) :  A_(A),b_(b) ,jmax_(jmax)
{	
	
}

VectorXd GradConj::Solve()const								
{
	int p = A_.rows();
	VectorXd u(p);											
	for (int i = 0; i < p; i++)
	{
		u[i]=0.;
		
	}

	VectorXd d(p),d0(p),b(b_);
	d = b - A_*u ;
	d0= d  ;	// calcul du residu
	double alpha;
	double mu=d0.norm();
	double Tol=0.0001 ; 
	VectorXd dSuivant(p);
	VectorXd uSuivant(p),z;
	int j = 0;
	while ( j<=jmax_)
	{
		z=A_*d0 ;
		alpha= (d.dot(d) )  / (d0.dot(z)) ;
		uSuivant= u + alpha*d0;
        dSuivant=d-alpha*(A_*d0);
		mu= (dSuivant.dot(dSuivant))/(d.dot(d));
		d0= dSuivant+ mu*d0;
		u=uSuivant ;
		d=dSuivant;
		j++;
	}
	return u;
}
	




		

	


