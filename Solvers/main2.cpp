#include "SDP.h"
#include "Tridiagonale.h"
#include "GradConj.h"
#include "residu_minimum.h"
#include "FOM.h"
#include "GMRes.h"
#include "fonctions.h"
#include <string>
#include <iostream>
#include <math.h>
#include "Gradpo.h"

using namespace Eigen;
using namespace std ;

int main ()
{
    int n ,m ,kmax;
    cout<<"donnez la taille de la matrice "<< endl ;
    cin >> n ;
    cout<< "donnez le nombre d'itération maximal "<< endl ;
    cin>> kmax ;
    MatrixXd A(n,n) , B= MatrixXd::Random(n,n) ;
    VectorXd b (n) , x (n) , r(n) ;
     for (int i=0 ;i<n ; i++)
	 {
		 b(i)=1 ;
	 }
    MatrixXd I=MatrixXd::Identity(n,n) ;
    B=B.cwiseAbs() ;
    double alpha = Norme1(B.transpose()*B) ;
    A=I+alpha*B.transpose()*B ;
    ofstream fichier ;
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
