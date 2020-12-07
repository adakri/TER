#include "GradConj.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

//constructeur
GradConj::GradConj(MatrixXd A,VectorXd b) :  A_(A),b_(b)
{	
}


//gradient conjugué
VectorXd GradConj::Solve()const								
{
	
	MatrixXd A=A_;
	int n = A.rows();
	VectorXd x(n);											
	for (int i = 0; i < n; i++)
	{
		x[i]=0.;
		
	}

	VectorXd r(n),b(b_),p(n);
	//cout<<b<<endl;
	r = b -A*x;
	p= r  ;	// calcul du residu
	double alpha;
	double gamma;
	VectorXd rSuivant(n);
	VectorXd xSuivant(n);
	VectorXd z(n);
	int j = 0;
	double beta=r.norm();
	int nb_iterat_=0;
	while (beta > pow(10,-10))
	{
		z=A*p;
		alpha= (r.dot(r) )  / (z.dot(p)) ;
		xSuivant= x + alpha*p;
        rSuivant=r-alpha*z;
		gamma= (rSuivant.dot(rSuivant))/(r.dot(r));
		p= rSuivant+ gamma*p;
		x=xSuivant ;
		//cout<<x<<endl;
		//cout<<"----------------------------------------"<<endl;
		r=rSuivant;
		beta=r.norm();
		nb_iterat_=nb_iterat_ +1;
		j++;
	}
	cout<<nb_iterat_<<endl;
	cout<<"----------------Gradient conjugué------------------------"<<endl;
	return x;
}
	




		

	


