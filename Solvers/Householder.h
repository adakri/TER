#include "Dense"
#include <fstream>

class Householder
{
private:

	Eigen::MatrixXd A_,Q_,R_;
	Eigen::VectorXd b_;
    

public:

	Householder(Eigen::MatrixXd A ,Eigen::VectorXd b) ;

public:

    void Decomposition_QR () ;
	Eigen::VectorXd Solve();
	Eigen::MatrixXd get_Q() ;
	Eigen::MatrixXd get_R() ;
};
