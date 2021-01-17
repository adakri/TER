#include "Dense"
#include <fstream>

class Tridiagonale
{
    private :
    
    double a_ ,b_,c_ ;  // valeurs constituants les trois diagonales 
    int n_ ; // taille de la matrice 
    Eigen::MatrixXd M_ ; // la matrice 
     
    public :

    Tridiagonale (double a , double b, double c ,int n ) ;
    Eigen::MatrixXd get_matrix () ; 

};