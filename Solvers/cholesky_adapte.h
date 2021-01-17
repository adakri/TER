#include "Dense"
#include <fstream>

class cholesky_adapte
{
    private :
    
    Eigen::MatrixXd A_ , L_;
    Eigen::VectorXd b_ ;

    public : 
    
    cholesky_adapte ( Eigen::MatrixXd , Eigen::VectorXd ) ;

    void Decomposition ( ) ;
    
    Eigen::VectorXd Solve () ;


};