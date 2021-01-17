#include "Dense"
#include <fstream>

class FOM_adapte
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	int m_ , kmax_;
	

public:

	FOM_adapte(Eigen::MatrixXd ,Eigen::VectorXd, int , int ) ;

public:

	Eigen::VectorXd Solve();
};
