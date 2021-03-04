#ifndef _TIME_SCHEME_CPP

#include "TimeScheme.h"
#include <iostream>

using namespace Eigen;
using namespace std;

// Constructeur par défaut (ne pas oublier de mettre votre pointeur à 0 !!)
TimeScheme::TimeScheme(DataFile* data_file, FiniteVolume* adv) :
_df(data_file), _fin_vol(adv), _t(_df->Get_t0()), _sol(adv->Initial_condition())
{
}

EulerScheme::EulerScheme(DataFile* data_file, FiniteVolume* adv) :
TimeScheme(data_file, adv)
{
}

ImplicitEulerScheme::ImplicitEulerScheme(DataFile* data_file, FiniteVolume* adv) :
TimeScheme(data_file, adv)
{
  std::cout << "Build time scheme class." << std::endl;
  std::cout << "-------------------------------------------------" << std::endl;
}

// Destructeur (car on a des fonctions virtuelles)
TimeScheme::~TimeScheme()
{
}

// Renvoie _sol (pratique pour vérifier la résolution)
const VectorXd & TimeScheme::Get_sol() const
{
  return _sol;
}

// Euler Explicite
void EulerScheme::Advance()
{
  // TODO
}

// Euler Implicite
void ImplicitEulerScheme::Advance()
{
  // TODO
}

#define _TIME_SCHEME_CPP
#endif
