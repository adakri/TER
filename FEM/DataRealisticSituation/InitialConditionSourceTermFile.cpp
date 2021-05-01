#ifndef _INIT_COND_CPP

#include "InitialConditionSourceTermFile.h"

double initialCondition(Eigen::Vector2d X)
{
    return 20.;
}

// Source term (sigma is in case of exact solution)
double sourceTerm(double t, Eigen::Vector2d X, double sigma)
{
  return 0.;
}

// Neumann boundary condition h
double neumannBC(double t, Eigen::Vector2d X, int ref, double sigma)
{
  return 0.0;
}

// Dirichlet boundary condition g
double dirichletBC(double t, Eigen::Vector2d X, int ref)
{
  if (ref == 2)
  {
    return 10.0;
  }
  else if (ref == 4)
  {
    return 300*(cos(0.2*M_PI*t)+1)*exp(-20*pow(X(0)-0.5,2));
  }
  else
  {
    std::cout << "The mesh is not compatible with the dirichlet boundary condition !" << std::endl;
    abort();
  }
}

// Exact solution
double exactSolution(double t, Eigen::Vector2d X)
{
  return 0.0;
}

#define _INIT_COND_CPP
#endif
