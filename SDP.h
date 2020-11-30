
#include "Dense"
#include <fstream>

class SDP
{
private:
    Eigen::MatrixXd  _A ;
public:
    SDP(const Eigen::MatrixXd& A);
    bool check () ;
};


