/*!
 * \file Matrix.h
 * \brief D&eacute;finition de la classe Matrix.
 
 */

#ifndef MATRIX_H
#define MATRIX_H

#include<iostream>
using std::ostream;
using std::istream;

/*!
 * \class Matrix
 *  Classe repr&eacute;sentant une matrice.
 *
 *  
*/
class Matrix
{ 
	 friend ostream &operator<< (ostream &, const Matrix &);		
	 friend istream &operator>> (istream &, Matrix &);				  
	 friend Matrix operator+ (const Matrix&,const Matrix&);			
	 friend Matrix operator- (const Matrix&,const Matrix&);			
	 friend Matrix operator* (const Matrix&,const Matrix&);			
	 friend Matrix operator* (const Matrix&,const double&);			
	
protected: 
	int l_;					  /*!< nombre de lignes de la matrice*/										
	int c_;					  /*!< nombre de colonnes de la matrice*/										
	double* data;			/*!< Tableau contenant les valeurs de la matrice. 
										     data[i*c_ + j ] rep&eacute;sente l'&eacute;l&eacute;ment (i,j) de la matrice*/										

public: 

	/*!
 * \fn Matrix (int l, int c).
 * \brief Constructeur de la classe Matrix &agrave; 2 param&egrave;tres. 
          Jette une exception en cas d'&eacute;chec d'allocation de m&eacute;moire
 * \param l : nombre de lignes; ne peut &ecirc;tre n&eacute;gatif.
 * \param c : nombre de colonnes; ne peut &ecirc;tre n&eacute;gatif.
 */
	Matrix (int l, int c);											

	/*!
 * \fn Matrix (int l).
 * \brief Constructeur de la classe Matrix &agrave; 1 param&egrave;tre. 
          Jette une exception en cas d'&eacute;chec d'allocation de m&eacute;moire.
 * \param l : nombre de lignes et de colonnes; ne peut &ecirc;tre n&eacute;gatif 	
 */
	Matrix (int l);													

	/*!
 * \fn Matrix (const Matrix & ).
 * \brief Constructeur de copie la classe Matrix &agrave; 1 param&egrave;tre. 
 * Cr&eacute;e une nouvelle instance de la classe Matrix &agrave; partir d'une instance existante.
 * Jette une exception en cas d'&eacute;chec d'allocation de m&eacute;moire.
 */

	Matrix (const Matrix & );										

	/*!
 * \fn ~Matrix ().
 * \brief Destructeur de la classeur Matrix. 
 */
	~Matrix();														

	/*!
 *  \fn Matrix &operator= (const Matrix&);
 *  \brief Surchage de l'op&eacute;rateur = pour la classe Matrix.
 *  D&eacute;finit l'operateur d'affectation = pour deux instances de la classe Matrix 
	*/
	Matrix &operator= (const Matrix&);								

	/*!
 *  \fn Matrix &operator+= (const Matrix&);
 *  \brief Surchage de l'op&eacute;rateur += pour la classe Matrix.
 *  D&eacute;finit l'operateur d'affectation d'addition += pour deux instances de la classe Matrix 
	*/
	Matrix &operator+= (const Matrix&);								

	/*!
 *  \fn Matrix &operator-= (const Matrix&);
 *  \brief Surchage de l'op&eacute;rateur -= pour la classe Matrix.
 *  D&eacute;finit l'operateur d'affectation de soustraction -= pour deux instances de la classe Matrix 
	*/
	Matrix &operator-= (const Matrix&);								

	/*!
 *  \fn Matrix &operator*= (const Matrix&);
 *  \brief Surchage de l'op&eacute;rateur *= pour la classe Matrix.
 *  D&eacute;finit l'operateur d'affectation de multiplication *= pour deux instances de la classe Matrix 
	*/
	Matrix &operator*= (const Matrix&);								

	/*!
 *  \fn bool operator== (const Matrix&) const;
 *  \brief Surchage de l'op&eacute;rateur == pour la classe Matrix.
 *  D&eacute;finit l'operateur d'&eacute;galit&eacute; == pour deux instances de la classe Matrix. 
 *  \return true si &eacute;galit&eacute;, false sinon 
	*/
	bool operator== (const Matrix &) const ;						

	/*!
 *  \fn bool operator!= (const Matrix&) const;
 *  \brief Surchage de l'op&eacute;rateur != pour la classe Matrix.
 *  D&eacute;finit l'operateur diff&eacute;rence != pour deux instances de la classe Matrix. 
 *  \return true si diff&eacute;rentes, false sinon
	*/
	bool operator!= (const Matrix &m) const                         
	{ return !(*this == m);}										

	/*!
 *  \fn Matrix transpose()const;
 *  \brief D&eacute;termine la transpos&eacute;e d'une matrice.
	*/
	Matrix transpose()const;												
	

	/*!
 *  \fn Matrix Cholesky();
 *  \brief D&eacute;termine la triangulaire inf&eacute;rieure &agrave; valeurs positives d'une matrice.
	*/
	Matrix Cholesky();												

	/*!
 *  \fn int RowsCountGet() const;
 *  \return le nombre de lignes de la matrice.
	*/
	int RowsCountGet() const;										

	/*!
 *  \fn int ColsCountGet() const;
 *  \return le nombre de colonnes de la matrice.
	*/
	int ColsCountGet() const;										

	/*!
 *  \fn double operator() (int i, int j) const;	
 *  \brief Surchage de l'op&eacute;rateur () pour la classe Matrix.
 *  \param i : num&eacute;ro de la ligne;
 *  \param j : num&eacute;ro de la colonne;
 *  \return l'&eacute;l&eacute;ment (i,j) de la matrice
	*/
	double operator() (int i, int j) const;							

	/*!
 *  \fn void operator() (int i, int j, double const & d);	
 *  \brief Surchage de l'op&eacute;rateur () pour la classe Matrix.
 *  Positionne la valeur d en position (i,j) dans la matrice.
 *  \param i : num&eacute;ro de la ligne;
 *  \param j : num&eacute;ro de la colonne;
 *	\param d : valeur &agrave; placer dans la matrice.	
	*/	
	void operator() (int i, int j, double const & d);	

};
#endif
