#ifndef _TIMESCHEME_CPP

#include "TimeScheme.h"
#include <fstream>
#include <iostream>


using namespace std;

TimeScheme::TimeScheme(DataFile* dataFile, Model* model, Solver* solver, Mesh* mesh) :
_dt(dataFile->getdt()), _sigma(dataFile->getSigma()), _t(dataFile->gett0()), _it(0), _nu(dataFile->getnu()), _lambda(dataFile->getlambda()),
_nPeriodToSaveSol(int(ceil(dataFile->getPeriodToSaveSol()/_dt))),
_solver(solver), _model(model), _results(dataFile->getResultsFolder()),
_vertices(mesh->getVertices()), _triangles(mesh->getTriangles())
{
}

TimeScheme::~TimeScheme() {}

void TimeScheme::saveSolution()
{
  saveSolution(_sol_n, "sol");
}

void TimeScheme::saveExactSolution()
{
  saveSolution(_model->computeExactSolution(_t), "exactSol");
}

void TimeScheme::saveErrorBetweenExactAndApproximatedSolutions()
{
  saveSolution(_sol_n-_model->computeExactSolution(_t), "error");
}


void TimeScheme::saveSolution(Eigen::SparseVector<double> sol, std::string name)
{
  if ((_it == 0) || (_it%_nPeriodToSaveSol == 0))
  {
  	std::string name_file = _results + "/" + name + "_" + std::to_string(_it) + ".vtk";
    int nb_vert = _vertices.rows();

  	std::ofstream solution;
  	solution.open(name_file, std::ios::out);
  	solution.precision(7);

    solution << "# vtk DataFile Version 3.0 " << std::endl;
    solution << "2D Unstructured Grid" << std::endl;
    solution << "ASCII" << std::endl;
    solution << "DATASET UNSTRUCTURED_GRID" << std::endl;

    solution << "POINTS " << nb_vert << " float " << std::endl;
    for (int i = 0 ; i < nb_vert ; i++)
    {
      solution << _vertices(i,0) << " " << _vertices(i,1) << " " << "0." << std::endl;
    }

    solution << "CELLS " << _triangles.rows() << " " << _triangles.rows()*4 << std::endl;
    for (int i = 0 ; i < _triangles.rows() ; i++)
    {
      solution << 3 << " " << _triangles(i,0) << " " << _triangles(i,1) << " " << _triangles(i,2) << std::endl;
    }

    solution << "CELL_TYPES " << _triangles.rows() << std::endl;
  	for (int i=0; i< _triangles.rows(); i++)
  	{
  		solution << 5 << std::endl;
  	}

    solution << "POINT_DATA " << nb_vert << std::endl; // Car solution aux sommets
    solution << "SCALARS sol float 1" << std::endl;
    solution << "LOOKUP_TABLE default" << std::endl;
    double eps = 1.0e-10;
  	for (int i = 0 ; i < nb_vert ; i++)
    {
  		solution << fmax(eps,sol.coeffRef(i)) << std::endl;
    }
    solution << std::endl;

  	solution.close();
  }
}

void TimeScheme::computeAndPrintError()
{
  double norml2 = (_sol_n - _model->computeExactSolution(_t)).norm()/_sol_n.size();
  std::cout << "Error: " << norml2 << std::endl;
}

Euler::Euler(DataFile* dataFile, Model* model, Solver* solver, Mesh* mesh) :
  TimeScheme(dataFile, model, solver, mesh)
{
}

void Euler::initialisation()
{
  //change here


  // Construction de la matrice du système

  std::cout << "Assembling" << std::endl;

  _model->assembleMassAndStiffnessMatrices();

  /* Eigen::SparseMatrix<double,Eigen::RowMajor> systemMatrix(_model->getMassMatrix()+_dt*_sigma*_model->getStiffnessMatrix()); */

  Eigen::SparseMatrix<double,Eigen::RowMajor> systemMatrix(_model->getMassMatrix()+_model->getStiffnessMatrix());

  // Application des conditions aux bords sur la matrice
  _model->applyBCToSystemMatrix(systemMatrix);

  // On envoie la matrice du système au solver
  std::cout << "Preprocessing of the matrix" << std::endl;
  _solver->setSystemMatrix(systemMatrix);

  // On recupère la solution initiale
  _model->buildInitialCondition();
  _sol_n = _model->getInitialCondition();
}

void Euler::advance()
{

  //change here

  
  // On avance le temps
  _t += _dt; _it += 1;
  // Construction du second membre
  _model->assembleSourceAndNeumann(_t);
  /* Eigen::SparseVector<double> RHS(_model->getMassMatrix()*_sol_n+_dt*_model->getSourceAndNeumann()); */

  Eigen::SparseVector<double> RHS(_model->getSourceAndNeumann());

  // Application des conditions sur le terme de droite
  _model->applyBCToRHS(RHS,_t);
  // Résolution du système
  std::cout << "Solve system at time " << _t << std::endl;
  _sol_n = _solver->solve(RHS);
  //std::cout<<_sol_n<<std::endl;
}



#define _TIMESCHEME_CPP
#endif
