

#include "SDP.h"
#include "GradConj.h"
#include "Gradpo.h"
#include "Residumin.h"
#include "FOM.h"
#include <string>
#include <iostream>
#include <math.h>

using namespace Eigen;
using namespace std;

int main() 
{ 

	//initiation de test par matrice sdp
	int n=6;
	MatrixXd A (n,n);                                              
	VectorXd b(n);
	
	for (int i=0; i<n; i++)
	{
		b[i]=i+1;
		for (int j=0; j<n; j++)
		{
			if(i==j)
				{
					A(i,i)=2.;
				}
			else if((j==i+1) || (j==i-1) )
				{
					A(i,j)=-1.;
				}
			else 
				{A(i,j)=0.;}
		}

	}
	cout<<"----------------la matrice A---------------------"<<endl;
	cout<<A<<endl;
	cout<<"----------------le second terme b---------------------"<<endl;
	cout<<b<<endl;
	cout<<"-------------------------------------"<<endl;
	SDP sdp(A) ;
    bool T=sdp.check () ;




	//Test Arnoldi et Fom
	FOM v(A,b);
	MatrixXd H(n+1,n),V(n,n+1);
	v.Arnoldi(b,V,H);
	cout<<"----------------la matrice V---------------------"<<endl;
	cout<<V<<endl;
	cout<<"----------------la matrice H---------------------"<<endl;
	cout<<H<<endl;
	cout<<"----------------solution---------------------"<<endl;
	cout<<v.Solve()<<endl;;




	//test des méthodes gconj,gop,rm
	if (T)
	{
	Residumin u(A,b);
	VectorXd x=u.Solve();
	cout<<"----------------la solution---------------------"<<endl;
	cout<<x<<endl;
	cout<<"--------------------le produit------------"<<endl;
	cout<<A*x<<endl;
	return 0 ;
	}
	else{
	std:: cout << "la matrice n'est pas sdp"<< std::endl ;
	return 0 ;
	}
};
