#ifndef _GEOMETRY_CPP

#include<iostream>
#include "Geometry.h"

Geometry::Geometry(Mesh* mesh) : _vertices(mesh->getVertices()),
_triangles(mesh->getTriangles()), _edgesNeumann(mesh->getNeumannEdges())
{
}

// Construction des 3 fonctions de base
// (phihat0, phihat1, phihat2)
double Geometry::phihat(int hati, Eigen::Vector2d hatX)
{
  double value;
  switch (hati)
  {
    case 0:
      value=1-hatX(0)-hatX(1);
      break;
    case 1:
      value=hatX(0);
      break;
    case 2:
      value=hatX(1);
      break;
    default:
      std::cout << "Please choose a valid number of hati for the reference triangle (0, 1 or 2)" << std::endl;
      abort();
  }
  return value;
}

// Construction des 3 gradients des fonctions de base
// (gradphihat0, gradphihat1, gradphihat2)
// (indépendant du vecteur hatX)
Eigen::Vector2d Geometry::gradphihat(int hati)
{
  Eigen::Vector2d gradphi;
  switch (hati)
  {
    case 0:
      gradphi(0) = -1; gradphi(1) = -1;
      break;
    case 1:
      gradphi(0) = 1; gradphi(1) = 0;
      break;
    case 2:
      gradphi(0) = 0; gradphi(1) = 1;
      break;
    default:
      std::cout << "Please choose a valid number of hati for the reference triangle (0, 1 or 2)" << std::endl;
      abort();
    }
    return gradphi;
  }

// Construction de la fonction de transformation
// du triangle de référence hatK en K
Eigen::Vector2d Geometry::FK(int refTriangleK, Eigen::Vector2d hatX)
{
  int n1(_triangles(refTriangleK,0)), n2(_triangles(refTriangleK,1)), n3(_triangles(refTriangleK,2));
  double k11(_vertices(n1,0)), k12(_vertices(n1,1));
  double k21(_vertices(n2,0)), k22(_vertices(n2,1));
  double k31(_vertices(n3,0)), k32(_vertices(n3,1));

  Eigen::Vector2d X;
  X(0) = k11*phihat(0,hatX)+k21*phihat(1,hatX)+k31*phihat(2,hatX);
  X(1) = k12*phihat(0,hatX)+k22*phihat(1,hatX)+k32*phihat(2,hatX);

  return X;
}

// Construction de la fonction de transformation
// de l'arête de référence hatE en E
Eigen::Vector2d Geometry::FE(int refEdgeE, Eigen::Vector2d hatX)
{
  int n1(_edgesNeumann(refEdgeE,0)), n2(_edgesNeumann(refEdgeE,1));
  double x1(_vertices(n1,0)), y1(_vertices(n1,1));
  double x2(_vertices(n2,0)), y2(_vertices(n2,1));

  Eigen::Vector2d X;
  X(0) = (x2-x1)*hatX(0)-(y2-y1)*hatX(1)+x1;
  X(1) = (y2-y1)*hatX(0)+(x2-x1)*hatX(1)+y1;

  return X;
}

// Construction de la jacobienne de la fonction de transformation
// du triangle de référence hatK en K
// (indépendant du vecteur hatX)
Eigen::Matrix2d Geometry::JFK(int refTriangleK)
{
  int n1(_triangles(refTriangleK,0)), n2(_triangles(refTriangleK,1)), n3(_triangles(refTriangleK,2));
  double k11(_vertices(n1,0)), k12(_vertices(n1,1));
  double k21(_vertices(n2,0)), k22(_vertices(n2,1));
  double k31(_vertices(n3,0)), k32(_vertices(n3,1));

  Eigen::Matrix2d X;
  X(0,0) = -k11+k21; X(0,1) = -k11+k31; X(1,0) = -k12+k22; X(1,1) = -k12+k32;

  return X;
}

// Construction de la jacobienne de la fonction de transformation
// de l'arête de référence hatE en E
// (indépendant du vecteur hatX)
Eigen::Matrix2d Geometry::JFE(int refEdgeE)
{
  int n1(_edgesNeumann(refEdgeE,0)), n2(_edgesNeumann(refEdgeE,1));
  double x1(_vertices(n1,0)), y1(_vertices(n1,1));
  double x2(_vertices(n2,0)), y2(_vertices(n2,1));

  Eigen::Matrix2d X;
  X(0,0) = (x2-x1); X(0,1) = -(y2-y1); X(1,0) = (y2-y1); X(1,1) = (x2-x1);

  return X;
}

// Construction de la valeur absolue du déterminant de la jacobienne
// de la fonction de transformation
// du triangle de référence hatK en K
// (indépendant du vecteur hatX)
double Geometry::measK(int refTriangleK)
{
  return fabs((JFK(refTriangleK)).determinant());
}

// Construction de la valeur absolue du déterminant de la jacobienne
// de la fonction de transformation
// de l'arête de référence hatE en E
// (indépendant du vecteur hatX)
double Geometry::measE(int refEdgeE)
{
  return sqrt(fabs((JFE(refEdgeE)).determinant()));
}

// Construction du gradient
// (gradphi_i) avec i qui est le hati-ème sommet de K
// appliqué en F(hatX)
Eigen::Vector2d Geometry::gradphi(int hati, int refTriangleK)
{
  return (((JFK(refTriangleK)).inverse()).transpose())*gradphihat(hati);
}

// Points et poids de quadrature de la formule du milieu (intégration 2D)
void Geometry::quadraturePointsAndWeightsMidpointFormula(Eigen::VectorXd& weights, Eigen::Matrix<double, Eigen::Dynamic, 2>& points)
{
  weights.resize(3);
  weights(0) = 1./3.; weights(1) = 1./3.; weights(2) = 1./3.;
  points.resize(3,2);
  points(0,0) = 0.5; points(0,1) = 0;
  points(1,0) = 0.0; points(1,1) = 0.5;
  points(2,0) = 0.5; points(2,1) = 0.5;
}

// Points et poids de quadrature de la formule du milieu (intégration 1D)
void Geometry::quadraturePointsAndWeightsSimpsonFormula(Eigen::VectorXd& weights, Eigen::Matrix<double, Eigen::Dynamic, 2>& points, int numedge)
{
  weights.resize(3);
  weights(0) = 1./6.; weights(1) = 2./3.; weights(2) = 1./6.;
  points.resize(3,2);
  if (numedge == 1)
  {
    points(0,0) = 0.0; points(0,1) = 0.0;
    points(1,0) = 0.5; points(1,1) = 0.0;
    points(2,0) = 1.0; points(2,1) = 0.0;
  }
  else if (numedge == 2)
  {
    points(0,0) = 1.0; points(0,1) = 0.0;
    points(1,0) = 0.5; points(1,1) = 0.5;
    points(2,0) = 0.0; points(2,1) = 1.0;
  }
  else if (numedge == 3)
  {
    points(0,0) = 0.0; points(0,1) = 1.0;
    points(1,0) = 0.0; points(1,1) = 0.5;
    points(2,0) = 0.0; points(2,1) = 0.0;
  }
}


#define _GEOMETRY_CPP
#endif
