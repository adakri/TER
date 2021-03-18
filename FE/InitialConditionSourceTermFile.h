#ifndef _INIT_COND_H

#include<cmath>
#include<iostream>
#include<vector>
#include "Dense"
#include "Sparse"

// Initial condition
double initialCondition(Eigen::Vector2d X);
double sourceTerm(double t, Eigen::Vector2d X, double sigma = 0);
double neumannBC(double t, Eigen::Vector2d X, int ref, double sigma = 0);
double dirichletBC(double t, Eigen::Vector2d X, int ref);
double exactSolution(double t, Eigen::Vector2d X);

#define _INIT_COND_H
#endif
