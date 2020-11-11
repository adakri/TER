/*!
 * \file Vecteur.h
 * \brief D&eacute;finition de la classe Vecteur.
 * \author Zangul
 * \version 0.1
 * \date 01 juin 2009
 */


#ifndef VECTEUR_H
#define VECTEUR_H

#include "Matrix.h"


/*!
 * \class Vecteur
 *  Classe repr&eacute;sentant un vecteur colonne. D&eacute;rive de la classe Matrix. 
*/

class Vecteur : public Matrix
{ 
	friend Vecteur prodMatVect(const Matrix &, const Vecteur &);
	friend double operator* ( const Vecteur&,const Vecteur& );
	friend Vecteur operator- ( const Vecteur&,const Vecteur& );
	friend Vecteur operator* ( const Vecteur&,const double & );
public:

	/*!
 * \fn Vecteur(int nbligne);
 * \brief Constructeur de la classe Vecteur. 
 * \param nbligne : nombre de lignes; ne peut &ecirc;tre n&eacute;gatif.
 */
	Vecteur(int nbligne);

	
	/*!
 * \fn double Norme();
 * \brief M&eacute;thode qui calcule la norme d'un vecteur. 
 * \return la norme d'un vecteur.
 */
	double Norme();	

	/*!
 * \fn double operator() (int i) const;	
 * \brief Surchage de l'o&eacute;rateur () pour la classe Vecteur. 
 * \param i : position dans le vecteur. 
 * \return le i&egrave;me &eacute;l&eacute;ment du vecteur.
 */
	double operator() (int i) const;							


	/*!
 * \fn void operator() (int i,double const & d);	
 * \brief Surchage de l'o&eacute;rateur () pour la classe Vecteur. 
 * Place la valeur d en i&egrave;me position dans le vecteur. 
 * \param i : position dans le vecteur; 
 * \param d : valeur &agrave; placer dans le vecteur.
 */
	void operator() (int i,double const & d);		     		
};
#endif
