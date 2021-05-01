#ifndef _SOLVER_H

#include "Sparse"
#include <SparseLU>

class Solver
{
public:
  Solver();
  virtual ~Solver();
  virtual void setSystemMatrix(Eigen::SparseMatrix<double,Eigen::RowMajor> systemMatrix) = 0;
  virtual Eigen::SparseVector<double> solve(Eigen::SparseVector<double> RHS) = 0;
};

class EigenSolver : public Solver
{
private:
  Eigen::SparseLU<Eigen::SparseMatrix<double, Eigen::ColMajor>, Eigen::COLAMDOrdering<int> >  _solver;
public:
  void setSystemMatrix(Eigen::SparseMatrix<double,Eigen::RowMajor> systemMatrix);
  Eigen::SparseVector<double> solve(Eigen::SparseVector<double> RHS);
};


#define _SOLVER_H
#endif
