#include "Dense"
#include <fstream>

class Arnoldi 
{
    private : 
    int m_ ;
    Eigen::VectorXd v_ ;
    Eigen::MatrixXd H_ , A_ , V_  ;
    public :

    Arnoldi (Eigen::VectorXd v , int m ,Eigen :: MatrixXd A ) ;

    public :
    
    void orthogonalisation () ;
    Eigen::MatrixXd get_H () ;
    Eigen::MatrixXd get_V () ;

};