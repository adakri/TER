#include "Residumin.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

//constructeur
Residumin::Residumin(MatrixXd A,VectorXd b) :  A_(A),b_(b)
{	
}


//Residu minimum
VectorXd Residumin::Solve()const								
{
	//init
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
		alpha= (r.dot(z) )  / (z.dot(z)) ;
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
    cout<<"----------------Résidu minimum------------------------"<<endl;
	return x;
}