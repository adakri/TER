#include "FOM.h"
#include <iostream>
#include <math.h>
#include <iostream>
#include <Eigen>
 
using Eigen::MatrixXd;


using namespace Eigen;
using namespace std;

//constructeur
FOM::FOM(MatrixXd A,VectorXd b) :  A_(A),b_(b)
{	
}
//Méthode de arnoldi
void FOM::Arnoldi(VectorXd u, MatrixXd& VV, MatrixXd& HH)
{
	//cout<<"started"<<endl;
    int m=u.size();
	//cout<<m<<endl;
    VectorXd v1(m),w(m);
	MatrixXd V(m,m+1),H(m+1,m);
	//cout<<"done"<<endl;
    V.col(0)=u/(u.norm());
    double h;
    //cout<<"loop"<<endl;
    for (int j=0; j<m; j++)
    {
        
        for (int i=0; i<j; i++)
        {
            H(i,j)=(A_*V.col(j)).dot(V.col(j));;
        }
		VectorXd sum(m);

		for (int i = 0; i < m; i++)
		{
			sum[i]=0.;
		}

		for (int i=0; i<m; i++)
		{
			sum=sum+H(i,j)*V.col(i);
		}
		w=A_*V.col(j)-sum;
        H(j+1,j)=w.norm();
		if (H(j+1,j)==0.)
		{
			break;
		}
		V.col(j+1)=w/H(j+1,j);

    }
	VV=V;
	HH=H;

}

//gradient à pas optimal
VectorXd FOM::Solve()								
{
	
	MatrixXd A=A_;
	const int n = A.rows();
	VectorXd x(n);											
	for (int i = 0; i < n; i++)
	{
		x[i]=0.;
		
	}

	VectorXd r(n),b(b_);
	//cout<<b<<endl;
	r = b -A*x;
	VectorXd v1(n);
	v1=r/r.norm();
	VectorXd rSuivant(n);
	VectorXd xSuivant(n);
	rSuivant=r;
	xSuivant=x;
	int j = 0;
	double beta=r.norm();
	int nb_iterat_=0;
	while (beta > pow(10,-10) && (nb_iterat_<1000))
	{
		//Arnoldi
		MatrixXd HH(n+1,n),VV(n,n+1);
		this -> Arnoldi(v1,HH,VV);
		MatrixXd H(n,n),V(n,n);
		
		H = HH.block(0,0,n,n);
		V= VV.block(0,0,n,n);
		
		//résolution QR
		
		HouseholderQR<MatrixXd> qr(n,n);
		qr.compute(H);
		MatrixXd Q = qr.householderQ();
		cout<<"----------------La matrice Q------------------------"<<endl;
		cout<<Q<<endl;
		//itération
		VectorXd e= V.transpose()*v1;
		VectorXd y(n);
		y = H.householderQr().solve(beta*e);
		cout<<"----------------La solution de Q------------------------"<<endl;
		cout<<y<<endl;
		
		xSuivant = xSuivant- V*y;
		rSuivant = b_ -A_*xSuivant;

		x=xSuivant ;
		r=rSuivant;


		


		
		beta=r.norm();
		nb_iterat_=nb_iterat_ +1;
		j++;
	}
	cout<<"le nombre d'itération : "<<nb_iterat_<<endl;
    cout<<"----------------FOM------------------------"<<endl;
	return x;
}