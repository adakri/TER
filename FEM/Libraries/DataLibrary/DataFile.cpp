#ifndef _DATA_FILE_CPP

#include "DataFile.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <regex>


using namespace std;

DataFile::DataFile(std::string file_name)
: _file_name(file_name),  _if_mesh_name(false), _if_t0(false), _if_tfinal(false), _if_dt(false),
_if_scheme(false), _if_numerical_flux_choice(false), _if_results(false), _if_mu(false), _N_BC(0),
_print_info(false)
{
}

void DataFile::Read_data_file()
{
  ifstream data_file(_file_name.data());
  if (!data_file.is_open())
  {
    cout << "Unable to open file " << _file_name << endl;
    exit(0);
  }
  else
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Reading data file " << _file_name << endl;
  }

  string file_line;

  while (!data_file.eof())
  {
    getline(data_file, file_line);

    if (file_line.find("#") !=std::string::npos)
    {
      // Ignore this line (comment)
    }
    else
    {
      if (file_line.find("mesh") != std::string::npos)
      {
        data_file >> _mesh_name; _if_mesh_name = true;
      }

      if (file_line.find("numerical_flux") != std::string::npos)
      {
        data_file >> _numerical_flux_choice; _if_numerical_flux_choice = true;
        if ((_numerical_flux_choice != "centered") && (_numerical_flux_choice != "upwind"))
        {
          cout << "Only centered and upwind numerical flows are implemented." << endl;
          exit(0);
        }
      }

      if (file_line.find("BoundaryConditions") != std::string::npos)
      {
        data_file >> _N_BC;
        _BC_ref.resize(_N_BC);
        _BC_type.resize(_N_BC);

        std::cout << "Boundary conditions" << endl;
        for (int bc = 0 ; bc < _N_BC ; bc++)
        {
          data_file >> _BC_ref[bc] >> _BC_type[bc];
          std::cout <<  _BC_ref[bc]  << " " <<  _BC_type[bc]  << std::endl;
         }
      }

      if (file_line.find("t0") != std::string::npos)
      {
        data_file >> _t0; _if_t0 = true;
      }

      if (file_line.find("tfinal") != std::string::npos)
      {
        data_file >> _tfinal; _if_tfinal = true;
      }

      if (file_line.find("which_scenario") != std::string::npos)
      {
        data_file >> _which_scenario;
      }

      if (file_line.find("dt") != std::string::npos)
      {
        data_file >> _dt; _if_dt = true;
      }

      if (file_line.find("scheme") != std::string::npos)
      {
        data_file >> _scheme; _if_scheme = true;
        if ((_scheme != "ExplicitEuler") && (_scheme != "ImplicitEuler"))
        {
          cout << "Only Explicit Euler and Implicit Euler schemes are implemented." << endl;
          exit(0);
        }
      }

      if (file_line.find("mu") != std::string::npos)
      {
        data_file >> _mu; _if_mu = true;
      }

      if (file_line.find("results") != std::string::npos)
      {
        data_file >> _results; _if_results = true;
      }
    }
  }
  if (!_if_t0)
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Beware - The default value (0.) is used for t0." << endl;
    _t0 = 0.;
  }
  if (!_if_tfinal)
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Beware - The default value (0.1) is used for tfinal." << endl;
    _tfinal = 0.1;
  }
  if (!_if_dt)
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Beware - The default value (0.001) is used for dt." << endl;
    _dt = 0.001;
  }
  if (!_if_scheme)
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Beware - The default scheme (Implicit Euler scheme) is used." << endl;
    _scheme = "ImplicitEuler";
  }
  if (!_if_mu)
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Beware - The default value (0.1) is used for mu." << endl;
    _mu = 0.1;
  }
  if (!_if_results)
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Beware - The default results folder name (results) is used." << endl;
    _results = "results";
  }
  cout << "-------------------------------------------------" << endl;
  if (!_if_mesh_name)
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Do not forget to give the mesh name in the data file." << endl;
    exit(0);
  }
  if (!_if_numerical_flux_choice)
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Beware - The default value (centered) is used for the numerical flow." << endl;
    _numerical_flux_choice = "centered";
  }
  // Conditions
  if (_scheme == "ExplicitEuler")
  {
    cout << "Beware to the CFL condition." << endl;
  }

  if ((_which_scenario == "diffusion_hom_neumann") || (_which_scenario == "diffusion_all_BC")
          || (_which_scenario == "advection_hom_neumann") ||  (_which_scenario == "advection_all_BC")
          || (_which_scenario == "diffusion_advection_all_BC") )
  {
    cout << "-------------------------------------------------" << endl;
    cout << "The test case: " << _which_scenario << " has been chosen." << endl;
    cout << "-------------------------------------------------" << endl;
    if (_mesh_name == "Meshes/square_mini.mesh")
    {
      _print_info = true;
    }
  }
  else if (_which_scenario == "none")
  {
    cout << "-------------------------------------------------" << endl;
    cout << "It is not a test case!" << endl;
    cout << "-------------------------------------------------" << endl;
  }
  else
  {
    cout << "-------------------------------------------------" << endl;
    cout << "A scenario has to be chosen. If it is not a test case, consider none." << endl;
    cout << "-------------------------------------------------" << endl;
    exit(0);
  }


  if ( (_which_scenario == "advection_hom_neumann") && (fabs(_mu) > 1e-6) )
  {
    cout << "Only advection: mu has been fixed at 0." << endl;
    _mu = 0;
  }

  if (_which_scenario == "advection_all_BC")
  {
    _which_scenario = "diffusion_advection_all_BC";
    if (fabs(_mu) > 1e-6)
    {
      cout << "Only advection: mu has been fixed at 0." << endl;
      _mu = 0;
    }
  }

  // Créer le dossier de résultats
  system(("mkdir -p ./" +_results).c_str());
  // Supprimer les anciens résultats
  system(("rm -f ./" +_results + "/*.vtk").c_str());
  // Copier le fichier de données dans le dossier résultats
  system(("cp -r ./" + _file_name + " ./"
          + _results + "/params.txt").c_str());

}

#define _DATA_FILE_CPP
#endif
