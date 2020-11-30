

#ifndef GRADCONJ_H
#define GRADCONJ_H

#include "Dense"
#include <fstream>

class GradConj
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	

public:
	GradConj(Eigen::MatrixXd ,Eigen::VectorXd) ;//: A_(A), b_(b) { };
	Eigen::VectorXd Solve()const;
};
#endif