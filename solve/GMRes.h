#ifndef GMRES_H
#define GMRES_H

#include "Dense"
#include <fstream>

class GMRes
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
	

public:
	GMRes(Eigen::MatrixXd ,Eigen::VectorXd) ;//: A_(A), b_(b) { };
    void Arnoldi(Eigen::VectorXd,Eigen::MatrixXd&,Eigen::MatrixXd&); //A devient Hm
    void QR(Eigen::MatrixXd&,Eigen::MatrixXd&);
    Eigen::MatrixXd ExtractR(Eigen::MatrixXd);
	Eigen::VectorXd Solve1();
};
#endif