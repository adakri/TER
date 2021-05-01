#ifndef _DATA_FILE_H

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
// Définition de la classe

class DataFile {
private:
  std::string _fileName;
  double _t0, _tfinal, _dt, _periodToSaveSol;
  std::string _meshName;
  std::string _resultsFolder;
  std::string _ICAndSTFile;
  std::vector<int> _dirichlet;
  std::vector<int> _neumann;
  std::vector<int> _BCReferences;
  bool _isExactSol;
  double _sigma;
  double _nu;
  double _nu1;
  double _lambda;
  double _lambda1;
  int _nIterations;

  bool  _ifMeshName, _ift0, _iftfinal, _ifdt, _ifResultsFolder, _ifDirichlet, _ifNeumann, _ifExactSol , _ifICAndSTFile, _ifIsExactSol, _ifSigma, _ifnu, _ifnu1, _iflambda, _iflambda1, _ifPeriodToSaveSol;

public: // Méthodes et opérateurs de la classe
  DataFile(std::string file_name);
  void readDataFile();
  const double & gett0() const {return _t0;};
  const double & gettfinal() const {return _tfinal;};
  const double & getdt() const {return _dt;};
  const int & getNumberOfIterations() const {return _nIterations;};
  const double & getPeriodToSaveSol() const {return _periodToSaveSol;};
  const double & getSigma() const {return _sigma;};
  const double & getnu() const {return _nu;};
  const double & getnu1() const {return _nu1;};
  const double & getlambda() const {return _lambda1;};
  const double & getlambda1() const {return _lambda;};
  const std::string & getMeshName() const {return _meshName;};
  const std::string & getResultsFolder() const {return _resultsFolder;};
  const std::vector<int> & getDirichletReferences() const {return _dirichlet;};
  const std::vector<int> & getNeumannReferences() const {return _neumann;};
  const bool & getIsExactSol() const {return _isExactSol;};
};

#define _DATA_FILE_H
#endif
