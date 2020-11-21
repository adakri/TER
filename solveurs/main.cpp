

#include "SDP.h"
#include "GradConj.h"
#include <string>
#include <iostream>
#include <math.h>

using namespace Eigen;


int main() 
{ 
	 MatrixXd A (2, 2);                                              
	 VectorXd b(2);
	std::cout<<"entrez 6 valeurs" << std:: endl ;
	for (int i=0;i<A.rows();i++)
	{
    for (int j=0 ;j<A.cols();j++)
	{
	std::cin >> A(i,j) ;
	}
	}
	for (int i=0;i<A.rows();i++)
	{
		std::cin >> b(i);
	}
	SDP sdp(A) ;
    bool T=sdp.check () ;
	if (T)
	{
	GradConj u(A,b);
	for (int i=0;i<A.rows();i++)
	{
	std::cout << u.Solve()(i) << std::endl ;
	}
	return 0 ;
	}
	std:: cout << "la matrice n'est pas sdp"<< std::endl ;
	return 0 ;
};
