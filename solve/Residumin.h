#ifndef Residumin_H
#define Residumin_H

#include "Dense"
#include <fstream>

class Residumin
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	

public:
	Residumin(Eigen::MatrixXd ,Eigen::VectorXd) ;//: A_(A), b_(b) { };
	Eigen::VectorXd Solve()const;
};
#endif