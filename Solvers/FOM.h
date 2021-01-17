#include "Dense"
#include <fstream>

class FOM
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	int m_ , kmax_;
	

public:

	FOM(Eigen::MatrixXd ,Eigen::VectorXd, int , int ) ;

public:

	Eigen::VectorXd Solve()const;
};
