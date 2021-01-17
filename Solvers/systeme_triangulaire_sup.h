#include "Dense"
#include <fstream>

class systeme_triangulaire_sup
{
private:

	Eigen::MatrixXd A_;
	Eigen::VectorXd b_;

public:

	systeme_triangulaire_sup (Eigen::MatrixXd ,Eigen::VectorXd) ;//: A_(A), b_(b) { };

public:

	Eigen::VectorXd Solve()const;
};
