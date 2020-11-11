/*!
 * \file GradConj.h
 * \brief D&eacute;finition de la classe GradConj.
 *
 */

#ifndef GRADCONJ_H
#define GRADCONJ_H

#include "Vecteur.h"

/*!
 * \class GradConj
 *  Classe repr&eacute;sentant une matrice
 *  Impl&eacute;mente la r&eacute;solution de syst&egrave;mes d'&eacute;quations matricielles
 *  
*/
class GradConj
{
private:
	Matrix A_;
	Vecteur b_;

public:

	/*!
 * \fn GradConj(Matrix A,Vecteur b).
 * \brief Constructeur de la classe GradConj. 
 * Jette une exception si A n'est pas sym&eacute;trique d&eacute;finie positive.
 * \param A : instance de la classe Matrix. Doit &ecirc;tre sym&eacute;trique d&eacute;finie positive. 
 * \param b : instance de la classe Vecteur.
 */
	GradConj(Matrix ,Vecteur) ;//: A_(A), b_(b) { };

public:

	/*!
  * \fn Vecteur Solve()const
  * \brief R&eacute;soud les equations matricielles du type : Ax = b.
  * \return le vecteur x v&eacute;rifiant Ax = b.
	*/
	Vecteur Solve()const;
};
#endif
