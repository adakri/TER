#ifndef _SOLVER_CPP

#include "Solver.h"
#include "Gradpo.h"
#include <iostream>

Solver::Solver() {}

Solver::~Solver() {}

void EigenSolver::setSystemMatrix(Eigen::SparseMatrix<double,Eigen::RowMajor> systemMatrix, Eigen::VectorXd&)
{
  Eigen::SparseMatrix<double,Eigen::ColMajor> A(systemMatrix);
  // Compute the ordering permutation vector from the structural pattern of A
  Gradpo Gradpo(MatrixXd A,VectorXd b,int kmax)
  // Compute the numerical factorization
  Eigen::VectorXd x(A.shape());
  x=Gradpo.solve(A);
}

Eigen::SparseVector<double> EigenSolver::solve(Eigen::SparseVector<double> RHS)
{
  //Use the factors to solve the linear system
  return Gradpo.solve(RHS);
}

#define _SOLVER_CPP
#endif
