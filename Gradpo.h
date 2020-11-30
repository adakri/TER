#ifndef GRADPO_H
#define GRADPO_H

#include "Dense"
#include <fstream>

class Gradpo
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	

public:
	Gradpo(Eigen::MatrixXd ,Eigen::VectorXd) ;//: A_(A), b_(b) { };
	Eigen::VectorXd Solve()const;
};
#endif