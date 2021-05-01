#ifndef _TIMESCHEME_H

#include<vector>
#include<string>
#include "Dense"
#include "Sparse"
#include "DataFile.h"
#include "Model.h"
#include "Solver.h"

class TimeScheme
{
protected:
  Solver* _solver;
  Model* _model;
  const double _dt, _sigma,_nu,_lambda;
  const int _nPeriodToSaveSol;
  double _t, _t0;
  int _it;
  std::string _results;
  Eigen::Matrix<double, Eigen::Dynamic, 2> _vertices;
  Eigen::Matrix<int, Eigen::Dynamic, 4> _triangles;
  Eigen::SparseVector<double> _sol_n;

public:
  TimeScheme(DataFile* dataFile, Model* model, Solver* solver, Mesh* mesh);
  virtual ~TimeScheme();
  virtual void initialisation() = 0;
  virtual void advance() = 0;
  void saveSolution();
  void saveExactSolution();
  void saveErrorBetweenExactAndApproximatedSolutions();
  void saveSolution(Eigen::SparseVector<double> sol, std::string name);
  void SaveSol(Eigen::VectorXd sol, std::string n_sol, int n);
  void computeAndPrintError();
  const Eigen::SparseVector<double> & Get_sol() const {return _sol_n;};
};

class Euler : public TimeScheme
{
public:
  Euler(DataFile* dataFile, Model* model, Solver* solver, Mesh* mesh);
  void initialisation();
  void advance();
};


#define _TIMESCHEME_H
#endif
