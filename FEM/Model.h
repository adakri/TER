#ifndef _MODEL_H

#include<vector>
#include<fstream>
#include "Dense"
#include "Sparse"
#include "Mesh.h"
#include "Geometry.h"
#include "DataFile.h"

class Model
{
private:
  // Coordonnées des sommets du maillage
  Eigen::Matrix<double, Eigen::Dynamic, 2> _vertices;
  // Liste de tous les triangles et leur référence
  Eigen::Matrix<int, Eigen::Dynamic, 4> _triangles;
  // Liste des références de points concernés par Dirichlet
  std::vector<std::pair<int,int>> _refVerticesDirichletWithRef;
  // Liste de toutes les arêtes avec condition de Neumann, leur référence et le triangle associé
  Eigen::Matrix<int, Eigen::Dynamic, 5> _edgesNeumann;

  // Géométrie
  Geometry* _geometry;
  // Liste des points et des poids de quadrature pour la formule du Milieu (2D)
  Eigen::VectorXd _weightsMidPoints;
  Eigen::Matrix<double, Eigen::Dynamic, 2> _pointsMidPoints;
  // Liste des points et des poids de quadrature pour la formule du Simpson (1D)
  Eigen::VectorXd _weightsSimpson;
  std::vector<Eigen::Matrix<double, Eigen::Dynamic, 2>> _pointsSimpson;

  // Condition Initiale
  Eigen::SparseVector<double> _sol0;
  // Matrice du système à résoudre
  Eigen::SparseMatrix<double,Eigen::RowMajor> _stiffness;
  // Matrice de masse
  Eigen::SparseMatrix<double,Eigen::RowMajor> _mass;
  // Second membre du système à résoudre
  Eigen::SparseVector<double> _sourceAndNeumann;

  // Booléen pour la solution exacte
  bool _isExactSol;
  // Coefficient de diffusion (besoin pour le terme source quand il y a une solution exacte)
  double _sigma;
  double _lambda;
  double _lambda1;
  double _nu;
  double _nu1;

  // Fichier de résultats
  std::string _results;

public:
  Model(Mesh* mesh, Geometry* geometry, DataFile* dataFile);
  void buildInitialCondition();
  Eigen::SparseVector<double> computeExactSolution(double t);
  void assembleMassAndStiffnessMatrices();
  void assembleSourceAndNeumann(double t);
  void applyBCToSystemMatrix(Eigen::SparseMatrix<double,Eigen::RowMajor> & systemMatrix);
  void applyBCToRHS(Eigen::SparseVector<double> & RHS, double t);
  const Eigen::SparseMatrix<double,Eigen::RowMajor> & getMassMatrix() const {return _mass;};
  const Eigen::SparseMatrix<double,Eigen::RowMajor> & getStiffnessMatrix() const {return _stiffness;};
  const Eigen::SparseVector<double> & getSourceAndNeumann() const {return _sourceAndNeumann;};
  const Eigen::SparseVector<double> & getInitialCondition() const {return _sol0;};
};


#define _MODEL_H
#endif
