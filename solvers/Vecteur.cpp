/*!
 * \file Vecteur.cpp
 * \brief D&eacute;claration des m&eacute;thodes de la classe Vecteur.
 * \author Zangul
 * \version 0.1
 * \date 01 juin 2009
 */


#include "Vecteur.h"
#include<cmath>

Vecteur::Vecteur(int nbligne) : Matrix(nbligne,1) { }


Vecteur prodMatVect(const Matrix& m,const  Vecteur& v)
{ 
	Matrix res= m * v;
	for(int i=0;i<v.l_;i++)
	{
	v.data[i]=res(i,0);
	}	
	return v;
}


double Vecteur::Norme()
{ 
	double s = 0;
	for(int i = 0; i<l_ ; i++)
		s = s + data[i]*data[i];
	return sqrt(s);
}
	
double Vecteur::operator() (int i)const
{ 
	return data[i];
};

void Vecteur::operator() (int i,double const & d)
{ 
	data[i] = d;
};

double operator* (const Vecteur  &v,const Vecteur &w)
{ 
	if (v.l_!= w.l_)
	{
		return 0;
	}
	else
	{
	double sum = 0; 
	for (int i = 0; i < v.l_ ; i++)
		sum += v.data[i]*w.data[i];
	return sum;
	}
};

Vecteur operator- (const Vecteur  &v,const Vecteur &w)
{ 
	if (v.l_ != w.l_)
		return 0;
	else
	{
		for (int i=0;i<v.l_; i++)
		{
			v.data[i]=v.data[i]-w.data[i];
		}
	}
return v ;
};

Vecteur operator* (const Vecteur  &v,const double &d)
{ 
		for (int i=0;i<v.l_; i++)
		{
			v.data[i]=d*v.data[i];
		}
		return v ;
};