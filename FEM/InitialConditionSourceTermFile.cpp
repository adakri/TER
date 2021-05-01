#ifndef _INIT_COND_CPP

#include "InitialConditionSourceTermFile.h"

double initialCondition(Eigen::Vector2d X)
{
  return exactSolution(0.0,X);
}

// Source term (sigma is in case of exact solution)
double sourceTerm(double t, Eigen::Vector2d X, double sigma)
{
/*   return -cos(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0)*1.0/pow(t+1.0,2.0)+(M_PI*sigma*(X(1)*cos(M_PI*X(0)*(X(1)*X(1))*4.0)*cos(M_PI*(X(0)*X(0))*X(1)*5.0)*-1.0E1+X(0)*sin(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0)*8.0+M_PI*(X(0)*X(0)*X(0)*X(0))*cos(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0)*2.5E1+M_PI*(X(1)*X(1)*X(1)*X(1))*cos(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0)*1.6E1+M_PI*(X(0)*X(0))*(X(1)*X(1))*cos(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0)*1.64E2+M_PI*X(0)*(X(1)*X(1)*X(1))*cos(M_PI*(X(0)*X(0))*X(1)*5.0)*sin(M_PI*X(0)*(X(1)*X(1))*4.0)*8.0E1+M_PI*(X(0)*X(0)*X(0))*X(1)*cos(M_PI*(X(0)*X(0))*X(1)*5.0)*sin(M_PI*X(0)*(X(1)*X(1))*4.0)*8.0E1))/(t+1.0); */
  return 0.001;
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

  return 0.001*(dX*nx + dY*ny);
}

// Dirichlet boundary condition g
double dirichletBC(double t, Eigen::Vector2d X, int ref)
{
  //return (cos(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0))/(t+1.0);
  return 0.005;
}

// Exact solution
double exactSolution(double t, Eigen::Vector2d X)
{
  return (cos(M_PI*X(0)*(X(1)*X(1))*4.0)*sin(M_PI*(X(0)*X(0))*X(1)*5.0))/(t+1.0);
}

#define _INIT_COND_CPP
#endif
