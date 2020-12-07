#ifndef FOM_H
#define FOM_H

#include "Dense"
#include <fstream>

class FOM
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	

public:
	FOM(Eigen::MatrixXd ,Eigen::VectorXd) ;//: A_(A), b_(b) { };
    void Arnoldi1(Eigen::VectorXd,Eigen::MatrixXd&,Eigen::MatrixXd&); //A devient Hm
	Eigen::VectorXd Solve();
};
#endif