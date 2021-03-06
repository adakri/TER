

#ifndef GRADCONJ_H
#define GRADCONJ_H

#include "Dense"
#include <fstream>

class GradConj
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	int jmax_ ;

public:

	GradConj(Eigen::MatrixXd ,Eigen::VectorXd, int ) ;

public:

	Eigen::VectorXd Solve()const;
};
#endif