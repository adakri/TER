#include <iostream>
#include <fstream>
#include <chrono>
#include "DataFile.h"
#include "Mesh.h"
#include "Geometry.h"
#include "Model.h"
#include "TimeScheme.h"



using namespace std;


int main(int argc, char** argv)
{

  if (argc < 2)
  {
    cout << "Please, enter the name of your data file." << endl;
    abort();
  }
  const string dataFile_name = argv[1];

  std::cout << "-------------------------------------------------" << std::endl;
  // ----------------------- Fichier de données --------------------------------
  DataFile* dataFile = new DataFile(dataFile_name);
  dataFile->readDataFile();
  std::cout << "-------------------------------------------------" << std::endl;
  // ---------------------------------------------------------------------------

  // ----- Lecture du maillage et construction des entités géométriques --------
  Mesh* mesh = new Mesh(dataFile->getMeshName(),dataFile->getDirichletReferences(),
                                                 dataFile->getNeumannReferences());
  Geometry* geometry = new Geometry(mesh);
  std::cout << "-------------------------------------------------" << std::endl;
  // ---------------------------------------------------------------------------

  // ------------------------- Création du modèle ------------------------------
  Model* model = new Model(mesh, geometry, dataFile);
  // ---------------------------------------------------------------------------

  // --------------------------- Choix du solver --------------------------------
  Solver* solver = new EigenSolver();
  // ---------------------------------------------------------------------------

  // ------------------ Création du schéma en temps ----------------------------
  TimeScheme* timeScheme = new Euler(dataFile, model, solver, mesh);
  // ---------------------------------------------------------------------------

  timeScheme->initialisation();
  timeScheme->saveSolution();
  if (dataFile->getIsExactSol())
  {
    timeScheme->saveExactSolution();
    timeScheme->saveErrorBetweenExactAndApproximatedSolutions();
    timeScheme->computeAndPrintError();
  }
  std::cout << "-------------------------------------------------" << std::endl;
  
  std::ofstream solution;

  // Boucle en temps
  for (int n = 1; n <= dataFile->getNumberOfIterations(); n++)
  {
    timeScheme->advance();
    timeScheme->saveSolution();
    if (dataFile->getIsExactSol())
    {
      timeScheme->saveExactSolution();
      timeScheme->saveErrorBetweenExactAndApproximatedSolutions();
      timeScheme->computeAndPrintError();
    }
    std::string name_file = "norm_results.txt";

  	solution.open(name_file, std::ios::out);

    Eigen::SparseVector<double> v=timeScheme -> Get_sol();

    solution<<n<<" "<<v.norm()<<endl;
  }
  solution.close();
  std::cout << "-------------------------------------------------" << std::endl;

  return 0;
}
