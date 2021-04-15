#ifndef _INIT_COND_CPP

#include "InitialConditionSourceTermFile.h"


// Initial condition
double initialCondition(Eigen::Vector2d X)
{
  return exactSolution(0.0,X);
}

// Source term (sigma is in case of exact solution)
double sourceTerm(double t, Eigen::Vector2d X, double sigma)
{
  return 0.
}

// Neumann boundary condition h
double neumannBC(double t, Eigen::Vector2d X, int ref, double sigma)
{
  double dX = (M_PI*X(1)*(X(0)*cos(M_PI*X(0)*(X(1)*X(1))*4.0)*cos(M_PI*(X(0)*X(0))*X(1)*5.0)*5.0-X(1)*sin(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0)*2.0)*2.0)/(t+1.0);
  double dY = (M_PI*X(0)*(X(0)*cos(M_PI*X(0)*(X(1)*X(1))*4.0)*cos(M_PI*(X(0)*X(0))*X(1)*5.0)*5.0-X(1)*sin(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0)*8.0))/(t+1.0);

  double nx = 0;
  double ny = 0;
  if (abs(X(1))   < 1.0e-10) {ny = -1;}
  if (abs(X(0)-1) < 1.0e-10) {nx =  1;}
  if (abs(X(0))   < 1.0e-10) {nx = -1;}
  if (abs(X(1)-1) < 1.0e-10) {ny =  1;}
  nx = nx/sqrt(nx*nx+ny*ny);
  ny = ny/sqrt(nx*nx+ny*ny);

  return 0.;
}

// Dirichlet boundary condition g
double dirichletBC(double t, Eigen::Vector2d X, int ref)
{
  return exactSolution(t,X);
}

// Exact solution
double exactSolution(double t, Eigen::Vector2d X)
{
  return 0.;
}

#define _INIT_COND_CPP
#endif
