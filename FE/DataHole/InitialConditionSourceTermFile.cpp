#ifndef _INIT_COND_CPP

#include "InitialConditionSourceTermFile.h"

double initialCondition(Eigen::Vector2d X)
{
    return 0.;
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
  return 1.0;
}

// Exact solution
double exactSolution(double t, Eigen::Vector2d X)
{
  return 0.0;
}

#define _INIT_COND_CPP
#endif
