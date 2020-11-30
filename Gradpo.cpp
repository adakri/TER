#include "Gradpo.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

//constructeur
Gradpo::Gradpo(MatrixXd A,VectorXd b) :  A_(A),b_(b)
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

	VectorXd r(n),b(b_);
	//cout<<b<<endl;
	r = b -A*x;
	double alpha;
	VectorXd rSuivant(n);
	VectorXd xSuivant(n);
	VectorXd z(n);
	int j = 0;
	double beta=r.norm();
	int nb_iterat_=0;
	while (beta > pow(10,-10))
	{
		z=A*r;
		alpha= (r.dot(r) )  / (z.dot(r)) ;
		xSuivant= x + alpha*r;
        rSuivant=r-alpha*z;
		x=xSuivant ;
		//cout<<x<<endl;
		//cout<<"----------------------------------------"<<endl;
		r=rSuivant;
		beta=r.norm();
		nb_iterat_=nb_iterat_ +1;
		j++;
	}
	cout<<"le nombre d'itération : "<<nb_iterat_<<endl;
    cout<<"----------------Gradient à pas optimal------------------------"<<endl;
	return x;
}