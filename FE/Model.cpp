#ifndef _MODEL_CPP

#include "Model.h"
#include "InitialConditionSourceTermFile.h"

Model::Model(Mesh* mesh, Geometry* geometry, DataFile* dataFile) :
_geometry(geometry),
_triangles(mesh->getTriangles()), _vertices(mesh->getVertices()),
_edgesNeumann(mesh->getNeumannEdges()),
_refVerticesDirichletWithRef(mesh->getRefVerticesDirichlet()),
_mass(_vertices.rows(),_vertices.rows()), _stiffness(_vertices.rows(),_vertices.rows()),
_sourceAndNeumann(_vertices.rows()),
_sigma(dataFile->getSigma()), _isExactSol(dataFile->getIsExactSol()),
_results(dataFile->getResultsFolder())
{
  // Weights and Points
  _geometry->quadraturePointsAndWeightsMidpointFormula(_weightsMidPoints, _pointsMidPoints);
  _pointsSimpson.resize(3);
  _geometry->quadraturePointsAndWeightsSimpsonFormula(_weightsSimpson, _pointsSimpson[0], 1);
  _geometry->quadraturePointsAndWeightsSimpsonFormula(_weightsSimpson, _pointsSimpson[1], 2);
  _geometry->quadraturePointsAndWeightsSimpsonFormula(_weightsSimpson, _pointsSimpson[2], 3);
}

void Model::buildInitialCondition()
{
  _sol0.resize(_vertices.rows());
  for (int i = 0; i < _vertices.rows() ; i++)
  {
    _sol0.coeffRef(i) = initialCondition(_vertices.row(i));
  }
}

Eigen::SparseVector<double> Model::computeExactSolution(double t)
{
  Eigen::SparseVector<double> exactSol(_vertices.rows());
  for (int i = 0; i < _vertices.rows() ; i++)
  {
    exactSol.coeffRef(i) = exactSolution(t,_vertices.row(i));
  }
  return exactSol;
}

void Model::assembleMassAndStiffnessMatrices()
{
  std::vector<Eigen::Triplet<double>> tripletsMass, tripletsStiffness;
  _mass.setZero();
  _stiffness.setZero();

  // Boucle sur les éléments K dans T_h
  for (int K = 0; K < _triangles.rows() ; K++)
  {
    // std::cout << "Progression de l'assemblage " << (K+1)/(1.0*_triangles.rows())*100 << "%" << std::endl;
    // Boucle sur les points de quadrature w_q, hatX_q
    for (int q = 0 ; q < _weightsMidPoints.rows() ; q++)
    {

      // Boucle sur les noeuds de l'élément de référence géométrique
      for (int hati = 0 ; hati < 3 ; hati++)
      {
        // Boucle sur les noeuds de l'élément de référence géométrique
        for (int hatj = 0 ; hatj < 3 ; hatj++)
        {
          // i = numéro global du noeud hati de K
          int i(_triangles(K,hati));
          // j = numéro global du noeud hatj de K
          int j(_triangles(K,hatj));

          // M(i,j) = M(i,j) + contribution de la _masse (hati,hatj) en hatX_q
          double contribMass(0.5 // aire du triangle de référence
                *_weightsMidPoints(q)
                *_geometry->phihat(hati,_pointsMidPoints.row(q))
                *_geometry->phihat(hatj,_pointsMidPoints.row(q))
                *_geometry->measK(K));
          tripletsMass.push_back({i,j,contribMass});

          // K(i,j) = K(i,j) + contribution de la rigidité (hati,hatj) en hatX_q
          double contribStiffness(0.5 // aire du triangle de référence
                *_weightsMidPoints(q)
                *_geometry->gradphi(hati,K).dot(_geometry->gradphi(hatj,K))
                *_geometry->measK(K));
          tripletsStiffness.push_back({i,j,contribStiffness});
        }
      }
    }
  }

	_mass.setFromTriplets(tripletsMass.begin(), tripletsMass.end());
  _stiffness.setFromTriplets(tripletsStiffness.begin(), tripletsStiffness.end());
}

void Model::assembleSourceAndNeumann(double t)
{
  _sourceAndNeumann.setZero();
  // Boucle sur les éléments K dans T_h
  for (int K = 0; K < _triangles.rows() ; K++)
  {
    // Boucle sur les points de quadrature w_q, hatX_q
    for (int q = 0 ; q < _weightsMidPoints.rows() ; q++)
    {
      // Boucle sur les noeuds de l'élément de référence géométrique
      for (int hati = 0 ; hati < 3 ; hati++)
      {
          // i = numéro global du noeud hati de K
          int i(_triangles(K,hati));

          // RHS(i) = RHS(i) + contribution du terme source en hatX_q
          double sourceTermForKInQuadPtInt = 0.;
          if (_isExactSol)
            sourceTermForKInQuadPtInt = sourceTerm(t, _geometry->FK(K, _pointsMidPoints.row(q)),_sigma);
          else
            sourceTermForKInQuadPtInt = sourceTerm(t, _geometry->FK(K, _pointsMidPoints.row(q)));

          double contribB(0.5 // aire du triangle de référence
                *_weightsMidPoints(q)
                *_geometry->phihat(hati,_pointsMidPoints.row(q))
                *sourceTermForKInQuadPtInt
                *_geometry->measK(K));

          _sourceAndNeumann.coeffRef(i) = _sourceAndNeumann.coeffRef(i)+contribB;
      }
    }
  }
  // Conditions aux bords de Neumann non homogène
  // Boucle sur les éléments E dans T_h
  Eigen::SparseVector<double> neumann(_sourceAndNeumann.size());
  neumann.setZero();

  for (int E = 0; E < _edgesNeumann.rows() ; E++)
  {
    int ref = _edgesNeumann(E,2);
    int K = _edgesNeumann(E,3);
    int numedge = _edgesNeumann(E,4);

    // Boucle sur les points de quadrature w_q, hatX_q
    for (int q = 0 ; q < _weightsSimpson.rows() ; q++)
    {
      // Boucle sur les noeuds de l'élément de référence géométrique
      int hati;
      for (int temphati = 0 ; temphati < 2 ; temphati++)
      {
          // i = numéro global du noeud hati de K
          if (numedge==1) {hati = temphati;}
          else if (numedge==2) {if (temphati == 0) hati=1; if (temphati == 1) hati=2;}
          else if (numedge==3) {if (temphati == 0) hati=2; if (temphati == 1) hati=0;}

          int i(_triangles(K,hati));

          double neumannForKInQuadPtInt = 0.;
          if (_isExactSol)
            neumannForKInQuadPtInt = neumannBC(t, _geometry->FE(E, _pointsSimpson[0].row(q)), ref, _sigma);
          else
            neumannForKInQuadPtInt = neumannBC(t, _geometry->FE(E, _pointsSimpson[0].row(q)), ref);

          double contribB(1.0 // longueur de l'arête de référence
                *_weightsSimpson(q)
                *neumannForKInQuadPtInt
                *_geometry->phihat(hati,_pointsSimpson[numedge-1].row(q))
                *_geometry->measE(E));
          neumann.coeffRef(i) = neumann.coeffRef(i)+contribB;
      }
    }
  }
  _sourceAndNeumann += neumann;
}

void Model::applyBCToSystemMatrix(Eigen::SparseMatrix<double,Eigen::RowMajor> & systemMatrix)
{
  Eigen::SparseVector<double> zeroRow(systemMatrix.cols());
  for (int i = 0; i < _refVerticesDirichletWithRef.size() ; i++)
  {
    int refV = _refVerticesDirichletWithRef[i].first;
    systemMatrix.row(refV) = zeroRow;
    systemMatrix.coeffRef(refV,refV) = 1.;
  }
}

void Model::applyBCToRHS(Eigen::SparseVector<double> & RHS, double t)
{
  for (int i = 0; i < _refVerticesDirichletWithRef.size() ; i++)
  {
    int refV = _refVerticesDirichletWithRef[i].first;
    RHS.coeffRef(refV) = dirichletBC(t, _vertices.row(refV), _refVerticesDirichletWithRef[i].second);
  }
}

#define _MODEL_CPP
#endif
