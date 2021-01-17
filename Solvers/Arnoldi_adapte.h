#include "Dense"
#include <fstream>

class Arnoldi_adapte 
{
    private : 
    int m_ ;
    Eigen::VectorXd v_ ;
    Eigen::MatrixXd H_ , A_ , V_  ;
    public :

    Arnoldi_adapte (Eigen::VectorXd v , int m ,Eigen :: MatrixXd A ) ;

    public :
    
    void orthogonalisation () ;
    Eigen::MatrixXd get_H () ;
    Eigen::MatrixXd get_V () ;

};