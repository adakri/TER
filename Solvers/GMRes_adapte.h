#include "Dense"
#include <fstream>

class GMRes_adapte
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	int m_ ,kmax_;
	

public:

	GMRes_adapte(Eigen::MatrixXd ,Eigen::VectorXd, int , int) ;

public:

	Eigen::VectorXd Solve();
	
};