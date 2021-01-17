#ifndef RESIDU_MINIMUM_H
#define RESIDU_MINIMUM_H

#include "Dense"
#include <fstream>

class residu_minimum 
{
    private :

    Eigen::MatrixXd A_;
	Eigen::VectorXd b_;
    int kmax_ ;

    public :

	residu_minimum(Eigen::MatrixXd ,Eigen::VectorXd, int) ;//: A_(A), b_(b) { };

    public:

	Eigen::VectorXd Solve()const;
};
#endif