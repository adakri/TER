#include "SDP.h"
#include "Tridiagonale.h"
#include "GradConj.h"
#include "residu_minimum.h"
#include "FOM_adapte.h"
#include "GMRes_adapte.h"
#include <string>
#include <iostream>
#include <math.h>
#include "Gradpo.h"

using namespace Eigen;


int main()
{
	std::cout << " donner la taille de la matrice " << std::endl ;
	int n ;
	std::cin >> n ;
	 MatrixXd A (n ,n);
	 VectorXd b(n),x(n);

	std::cout<<"entrer les trois valeurs des diagonales " << std:: endl ;
	double a1 , a2 , a3 ;
	std::cin >> a1 >> a2 >> a3  ;

	for (int i=0;i<n;i++)
	{
		b(i)=1 ;
	}

	Tridiagonale M(a1,a2,a3,n);
	A =M.get_matrix() ;

	int i ;
	std::cout << "choisissez entre les  méthodes suivantes :  1) gradient conjugé   2) residu minimum  3) FOM   4) GMRes  5)Gradpo" << std:: endl ;
	std::cin>> i ;
	SDP sdp(A) ;
    bool T=sdp.check () ;



	if (i==1)
	{

		if (T)
		{

		GradConj u(A,b,1000);
		x=u.Solve() ;
		std::cout << " ------------------x-----------------"<< std::endl ;
		std::cout << (A*x-b).norm() << std::endl ;
		return 0 ;

		}
		std::cout << "la matrice n' est pas sdp " << std::endl ;
		return 0 ;

	}

	if (i==2)
	{
		if (T)
			{
			residu_minimum u(A,b,1000) ;
			x=u.Solve();
			std::cout << " ------------------x-----------------"<< std::endl ;
			std::cout << (A*x-b).norm() << std::endl ;
			return 0 ;
			}
		std::cout << "la matrice n' est pas sdp " << std::endl ;
		return 0 ;
		}
		if (i==3)
		{
			std::cout << "donnez la dimension de l'espace de Krylov souhaitée  " << std::endl ;
			int m ;
			std::cin >> m ;
			FOM_adapte u(A, b,m,1000) ;
			x=u.Solve();
			std::cout << " ------------------x-----------------"<< std::endl ;
			std::cout << (A*x-b).norm() << std::endl ;
			return 0 ;
		}
		if (i==4)
		{
			std::cout << "donnez la dimension de l'espace de Krylov souhaitée  " << std::endl ;
			int m ;
			std::cin >> m ;
			GMRes_adapte u(A, b, m,1000) ;
			x=u.Solve();
			std::cout << " ------------------x-----------------"<< std::endl ;
			std::cout << (A*x-b).norm() << std::endl ;
			return 0 ;
		}


			if (i==5)
			{

				if (T)
				{

				Gradpo u(A,b,1000);
				x=u.Solve() ;
				std::cout << " ------------------x-----------------"<< std::endl ;
				std::cout << (A*x-b).norm() << std::endl ;
				return 0 ;

				}
				std::cout << "la matrice n' est pas sdp " << std::endl ;
				return 0 ;

			}

	};
