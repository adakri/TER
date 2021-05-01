#ifndef _dataFile_CPP

#include "DataFile.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

#define bug std::cout<<"heeeeeeeeeeeere"<<std::endl;
#define show(a) std::cout<<a<<std::endl;

DataFile::DataFile(std::string file_name)
: _fileName(file_name),  _ifMeshName(false), _ifResultsFolder(false),
_ift0(false), _iftfinal(false), _ifdt(false), _ifPeriodToSaveSol(false),
_ifDirichlet(false), _ifNeumann(false),
_ifICAndSTFile(false), _isExactSol(false), _ifIsExactSol(false),
_ifSigma(false), _ifnu(false), _ifnu1(false), _iflambda(false), _iflambda1(false)
{
}

void DataFile::readDataFile()
{
  ifstream dataFile(_fileName.data());
  if (!dataFile.is_open())
  {
    cout << "Unable to open file " << _fileName << endl;
    abort();
  }
  else
  {
    cout << "Reading data file " << _fileName << endl;
  }

  string fileLine;

  while (!dataFile.eof())
  {
    getline(dataFile, fileLine); 
    if (fileLine.find("mesh") != std::string::npos)
    {
      dataFile >> _meshName; _ifMeshName = true;//show( _meshName)
    }

    if (fileLine.find("sigma") != std::string::npos)
    {
      dataFile >> _sigma; _ifSigma = true;//show(_sigma)
    }

    if (fileLine.find("lambda") != std::string::npos)
    {
      dataFile >> _lambda; _iflambda = true;//show(_lambda)
    }

    if (fileLine.find("lambda1") != std::string::npos)
    {
      dataFile >> _lambda1; _iflambda1 = true;//show(_lambda1)
    }

    if (fileLine.find("nu") != std::string::npos)
    {
      dataFile >> _nu; _ifnu = true;//show(_nu)
    }

    if (fileLine.find("nu1") != std::string::npos)
    {
      dataFile >> _nu1; _ifnu1 = true;//show(_nu1)
    }

    if (fileLine.find("t0") != std::string::npos)
    {
      dataFile >> _t0; _ift0 = true;//show(_t0)
    }

    if (fileLine.find("tfinal") != std::string::npos)
    {
      dataFile >> _tfinal; _iftfinal = true;//show(_tfinal)
    }

    if (fileLine.find("dt") != std::string::npos)
    {
      dataFile >> _dt; _ifdt = true;//show(_dt)
    }

    if (fileLine.find("period_to_save_sol") != std::string::npos)
    {
      dataFile >> _periodToSaveSol; _ifPeriodToSaveSol = true;
    }

    if (fileLine.find("results_folder") != std::string::npos)
    {
      dataFile >> _resultsFolder; _ifResultsFolder = true;
      system(("mkdir -p ./" + _resultsFolder).c_str());
      system(("rm -f ./" + _resultsFolder + "/*.vtk").c_str());
    }

    if (fileLine.find("initialCondition_and_sourceTerm_file") != std::string::npos)
    {
      dataFile >> _ICAndSTFile; _ifICAndSTFile = true;
      //system(("cp -r ./" + _ICAndSTFile + " ./InitialConditionSourceTermFile.cpp").c_str());
    }

    if (fileLine.find("is_exact_sol") != std::string::npos)
    {
      getline(dataFile, fileLine);
      if (fileLine == "true")
      {
        _isExactSol = true;
      }
      _ifIsExactSol = true;
    }

    if (fileLine.find("dirichlet") != std::string::npos)
    {
      getline(dataFile, fileLine);
      std::string::size_type sz;   // alias of size_t
      for (int i = 0 ; i < fileLine.size() ; i++)
      {
        if (i%2==0)
        {
          int temp = std::stoi(fileLine.substr(i,1),&sz);
          _dirichlet.push_back(temp);
          _BCReferences.push_back(temp);
        }
      }
      _ifDirichlet = true;
    }

    if (fileLine.find("neumann") != std::string::npos)
    {
      getline(dataFile, fileLine);
      std::string::size_type sz;   // alias of size_t
      for (int i = 0 ; i < fileLine.size() ; i++)
      {
        if (i%2==0)
        {
          int temp = std::stoi(fileLine.substr(i,1),&sz);
          _neumann.push_back(temp);
          _BCReferences.push_back(temp);
        }
      }
      _ifNeumann = true;
    }
  }

  if (!_ifSigma)
  {
    cout << "Beware - The default value (1.) is used for sigma." << endl;
    _sigma = 1.;
  }
  
  if (!_iflambda)
  {
    cout << "Beware - The default value (1.) is used for lambda." << endl;
    _lambda = 1.;
  }
  if (!_iflambda1)
  {
    cout << "Beware - The default value (1.) is used for lambda1." << endl;
    _lambda1 = _lambda/2;
  }
  if (!_ifnu)
  {
    cout << "Beware - The default value (1.) is used for nu." << endl;
    _nu = 1.;
  }

  if (!_ifnu1)
  {
    cout << "Beware - The default value (1.) is used for nu1." << endl;
    _nu1 = _nu/3;
  }
  if (!_ift0)
  {
    cout << "Beware - The default value (0.) is used for t0." << endl;
    _t0 = 0.;
  }
  if (!_iftfinal)
  {
    cout << "Beware - The default value (1) is used for tfinal." << endl;
    _tfinal = 1.;
  }
  if (!_ifdt)
  {
    cout << "Beware - The default value (0.1) is used for dt." << endl;
    _dt = 0.1;
  }
  if (!_ifPeriodToSaveSol)
  {
    cout << "Beware - The default value (dt) is used for the period to save the solution." << endl;
    _periodToSaveSol = _dt;
  }
  if (!_ifResultsFolder)
  {
    cout << "Beware - The default results folder name (results) is used." << endl;
    _resultsFolder = "results";
  }
  if (!_ifMeshName)
  {
    cout << "Do not forget to give the mesh name in the data file." << endl;
    abort();
  }
  if (!_ifICAndSTFile)
  {
    cout << "Do not forget to give the name of the file containing the initial condition and the source term." << endl;
    abort();
  }
  if (!_ifDirichlet)
  {
    cout << "There is no Dirichlet boundary conditions." << endl;
  }
  else
  {
    cout << "Dirichlet BC(s) is (are) imposed on reference(s):";
    for (int i = 0 ; i < _dirichlet.size() ; i++)
    cout << " " << _dirichlet[i];cout << "." << endl;
  }
  if (!_ifNeumann)
  {
    cout << "There is no Neumann boundary conditions." << endl;
  }
  else
  {
    cout << "Neumann BC(s) is (are) imposed on reference(s):";
    for (int i = 0 ; i < _neumann.size() ; i++)
    cout << " " << _neumann[i];cout << "." << endl;
  }


  // Période de sauvegarde de la solution
  if (_periodToSaveSol < _dt+1e-12)
  {
    _periodToSaveSol = _dt;cout << "The period to save the solution has to be superior to the time step. We fix it at dt." << endl;
  }

  // Adaptation du pas de temps
  _nIterations = int(ceil((_tfinal-_t0)/_dt));
  _dt = (_tfinal-_t0) / _nIterations;
  int nPeriodToSaveSol = int(ceil(_periodToSaveSol/_dt));
  _periodToSaveSol = nPeriodToSaveSol*_dt;

  // Copier le fichier de données dans le dossier résultats
  system(("cp -r ./" + _fileName + " ./" + _resultsFolder + "/" + _fileName.substr(_fileName.find_last_of('/'))).c_str());
  // Copier le fichier de condition initiale et de terme source dans le dossier résultats
  system(("cp -r ./" + _ICAndSTFile + " ./" + _resultsFolder + "/" + _ICAndSTFile.substr(_ICAndSTFile.find_last_of('/'))).c_str());

  // Vérifier qu'il n'y a pas des références communes entre Neumann et Dirichlet
  std::vector<int>::iterator it;
  it = std::unique(_BCReferences.begin(), _BCReferences.end());
  int new_size = std::distance(_BCReferences.begin(),it);
  if (new_size != _BCReferences.size())
  {cout << "There are common references between Dirichlet and Neumann BCs. Please correct the data file." << endl;
  abort();
}
}

#define _dataFile_CPP
#endif
