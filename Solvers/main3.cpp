#include "SDP.h"
#include "Tridiagonale.h"
#include "GradConj.h"
#include "residu_minimum.h"
#include "FOM.h"
#include "GMRes.h"
#include <string>
#include <iostream>
#include <math.h>
#include "Gradpo.h"

using namespace Eigen;
using namespace std ;



MatrixXd fill_matrix()
{
	int dim=5357;
	MatrixXd Mtx(dim,dim);
    Mtx=MatrixXd::Zero(dim,dim);
	cout<<"started"<<endl;




double i,j,val;

    // le constructeur de ifstream permet d'ouvrir un fichier en lecture
    std::ifstream fichier;
		fichier.open("matriceq5.txt");
cout<<"b2"<<endl;
    if ( fichier ) // ce test échoue si le fichier n'est pas ouvert
    {
			cout<<"b3"<<endl;
        std::string ligne; // variable contenant chaque ligne lue

				fichier>>i>>j>>val;
				Mtx(i-1,j-1)=val;

				Mtx(j-1,i-1)=val;
				cout<<"val="<<val<<endl;
				cout<<"i="<<i<<endl;
				cout<<"j="<<j<<endl;
        // cette boucle s'arrête dès qu'une erreur de lecture survient
        while ( getline( fichier, ligne ) )
        {

            // afficher la ligne à l'écran

						//fichier<<i<<j<<val<<endl;

						fichier>>i>>j>>val;

						//cout<<"val="<<val<<endl;
						//cout<<"i="<<i<<endl;
						//cout<<"j="<<j<<endl;
						Mtx(i-1,j-1)=val;
						//cout<<"b9"<<endl;
						Mtx(j-1,i-1)=val;
						//cout<<"b10"<<endl;
						//cout<<ligne<<endl;



        }
    }

cout<<"b1"<<endl;


    //     k=k+1;

		//cout<<"Mtx"<<Mtx<<endl;

	fichier.close();
	return Mtx;

}

int main()
{
	int n=5357, kmax ,m ;
    cout<< "donnez le nombre d'itération maximal "<< endl ;
    cin>> kmax ;
	MatrixXd A(n,n),A1(n,n),TA(n,n);
	A1=fill_matrix();
    TA=A.transpose();
    A=A1+TA ;
    VectorXd b (n) , x (n) , r(n) ;
    for (int i=0 ;i<n ; i++)
	{
		 b(i)=1 ;
	}
    MatrixXd I=MatrixXd::Identity(n,n) ;
    cout << " choisir pour quelle méthode vous voulez tracer la courbe des residu en fonction du nombre d'itérations " << endl ;
    cout << " 1) gradien conjugé " << endl ;
    cout << "  2) résidu minimum " << endl ;
    cout << "3) FOM " << endl ;
    cout << " 4) GMRes " << endl ;
		cout << " 5) Gradpo " << endl ;
    int choix ;
    cin >> choix ;
    if ( choix== 1 )
    {

        for (int k=20 ; k < kmax ; k=k+10)
        {
            GradConj u (A, b , k );
            x=u.Solve() ;
            r=b-A*x ;

            cout  << k << " "<< r.norm() << endl ;

        }


    }
    if (choix==2 )
    {

        for (int k=20 ; k < kmax ; k=k+10)
        {
            residu_minimum u (A, b , k );
            x=u.Solve() ;
            r=b-A*x ;

            cout  << k << " "<< r.norm() << endl ;
        }
    }
    if (choix==3)
    {
        cout << "donnez la dimension de l'espace krylov "<< endl ;
        cin >> m ;
        for (int k=20 ; k < kmax ; k=k+10)
        {

            FOM u (A, b ,m,k );

            x=u.Solve() ;

            r=b-A*x ;
						
            cout  << k << " "<< r.norm() << endl ;


        }

    }
    if (choix==4)
    {
        cout << "donnez la dimension de l'espace krylov "<< endl ;
        cin >> m ;
      for (int k=20 ; k < kmax ; k=k+10)
        {
           GMRes u (A, b , m , k );
            x=u.Solve() ;
            r=b-A*x ;
            cout << k << " "<< r.norm() << endl ;

        }

    }

		if ( choix== 5 )
    {

        for (int k=20 ; k < kmax ; k=k+10)
        {
            Gradpo u (A, b , k );
            x=u.Solve() ;
            r=b-A*x ;

            cout  << k << " "<< r.norm() << endl ;

        }
			}

return 0 ;
}
