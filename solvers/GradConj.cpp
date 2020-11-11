/*!
 * \file GradConj.cpp
 * \brief D&eacute;claration des m&eacute;thodes de la classe GradConj.

 */



#include "Vecteur.h"
#include "GradConj.h"
#include "Erreur.h"

GradConj::GradConj(Matrix A,Vecteur b) :  A_(A),b_(b)
{	
	if ((A.RowsCountGet() != A.ColsCountGet()) || (A.transpose() != A)){
		throw erreur(NON_SYM);
	}
}

Vecteur GradConj::Solve()const								
{
	int p = A_.RowsCountGet();
	Vecteur u(p);											
	for (int i = 0; i < p; i++)
	{
		u(i,1.);
		
	}

	Vecteur d(p),d0(p);
	d = b_ - prodMatVect(A_,u) ;
	d0 = d  ;	// calcul du residu
	double alpha;
	double mu;
	Vecteur dSuivant(p);
	Vecteur uSuivant(p);
	int j = 0;
	while ((j < p) && (d.Norme() > 0.0001) )
	{
		alpha = (d * d )  / (prodMatVect(A_,d0) * d0) ;
		uSuivant = u - d0 * (-alpha);
        dSuivant =d-prodMatVect(A_,d0)*alpha;
		mu = (dSuivant*dSuivant)/(d*d);
		d0 = dSuivant- d0*(-mu);
		u=uSuivant ;
		d=dSuivant;
		j++;
	}
	return u;
}
	




		

	


