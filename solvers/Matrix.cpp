/*!
 * \file Matrix.cpp
 * \brief D&eacute;claration des m&eacute;thodes de la classe Matrix.
 * \author Zangul
 * \version 0.1
 * \date 01 juin 2009
 */


#include <iostream>
#include <cmath>
#include "Matrix.h"
#include "Erreur.h"
#define MAX(A,B) ( A > B ? A : B)
#define MIN(A,B) ( A < B ? A : B)

Matrix::Matrix (int l, int c)
{ 
	if (l < 0 || c < 0)
		throw erreur(BAD_INDEX);
	l_ = l;
	c_ = c;
	data = new double[ l*c ]; 
	if (!data)
		throw erreur(BAD_ALLOC);
};


Matrix::Matrix (int l)
{ 
	if (l < 0)
		throw erreur(BAD_INDEX);
	l_ = l;
	c_ = l;
	data = new double[ l*l ]; 
	if (!data)
		throw erreur(BAD_ALLOC);
};

Matrix::Matrix(const Matrix &m)
{
	l_ = m.l_;
	c_ = m.c_;
	data = new double[ m.l_*m.c_ ]; 
	for (int i=0 ;i<m.l_*m.c_;i++)
	{
		
			data[i]=m.data[i];
		
	}
}

Matrix::~Matrix ()
{ 
	delete[] data;
};

int Matrix::RowsCountGet() const
{ 
	return l_;
};

int Matrix::ColsCountGet() const
{ 
	return c_;
};

double Matrix::operator() (int i, int j)const
{ 
	if (i < 0 || i >= l_ || j < 0 || j >= c_ )
		throw erreur(BAD_INDEX);
	return data[i*c_+j];
};

void Matrix::operator() (int i, int j, double const & d)
{ 
	if (i < 0 || i >= l_ || j < 0 || j >= c_ )
		throw erreur(BAD_INDEX);
	data[i*c_+j] = d;
};

//transpos�e d'une matrice
Matrix Matrix::transpose()const
{
	Matrix m(c_,l_);
	for (int i = 0; i < c_; i++)
		for (int j = 0; j < l_; j++)
		{
			m.data[i*l_+j] = data[j*c_+i];
		}
	return m;
}

//Operateur de sortie surcharg� pour la classe Matrix
ostream &operator<< (ostream &sortie, const Matrix &m)
{
	std::cout << std::endl;
	for (int i = 0; i < m.l_; i++)
	{ 
		for(int j = 0; j < m.c_; j++)
		{ 
			sortie << m.data[i*m.c_+j] <<" "; 
		} 
		sortie << std::endl;
	}
	return sortie;
};


//Operateur d'entr�e surcharg� pour la classe Matrix
istream &operator>> (istream &entree, Matrix &m)
{
	double k;
	for (int i = 0; i < m.l_; i++)
	{ 
		for(int j = 0; j < m.c_; j++)
		{ 
			entree >> k;
			m(i,j,k); 
		} 
	}
	return entree;
};

//Operateur de comparaison surcharg� pour la classe Matrix
bool Matrix::operator== (const Matrix &m) const
{
	if ((l_ != m.l_) || (c_ != m.c_))
		return false;

	for (int i = 0; i < (l_*c_);i++)
		if (data[i] != m.data[i])
			return false;

	return true;
};


//Operateur += surcharg� pour la classe Matrix
Matrix &Matrix::operator+= (const Matrix &m)
{
	if ((l_ != m.l_) || (c_ != m.c_))
		throw erreur(BAD_SIZE);	
	else
		for (int i = 0; i < l_*c_; i++)
			data[i] += m.data[i];
	
	return *this;
}

//Operateur -= surcharg� pour la classe Matrix
Matrix &Matrix::operator-= (const Matrix &m)
{
	if ((l_ != m.l_) || (c_ != m.c_))
		throw erreur(BAD_SIZE);	
	else
		for (int i = 0; i < l_*c_; i++)
			data[i] -= m.data[i];
	
	return *this;
}

//Operateur d'addition surcharg� pour la classe Matrix
Matrix operator+ (const Matrix &n,const Matrix &m)
{
	Matrix matrice = m;
	return matrice +=n;
}

//Operateur de soustraction surcharg� pour la classe Matrix
Matrix operator- (const Matrix &n,const Matrix &m)
{
	Matrix matrice = n;
	return matrice -= m;	
}

//Operateur de multiplication surcharg� pour la classe Matrix
Matrix operator* (const Matrix &n,const Matrix &m)
{
	
	if (n.c_ != m.l_)
	{
		throw erreur(BAD_SIZE);
	}
	else
	{
		double s;
		Matrix matrice(n.l_,m.c_);
		for (int i = 0; i < n.l_; i++)
			for (int j = 0; j < m.c_; j++)
			{
					 s = 0;
					for (int k = 0; k < n.c_; k++)					   
						s += n(i,k)*m(k,j);
					
					matrice(i,j,s);
			}

	return matrice;
	}
}

//Decomposition de Cholesky de la matrice. 
Matrix Matrix::Cholesky()
{
	if (c_ != l_)
		throw erreur(BAD_SIZE);	
	Matrix m(l_,c_);
	for (int i = 0; i < l_; i++)
	{
		for (int j = i + 1; j < l_; j++)
		{
			m(i,j,0);
		}
	}
	double sum1 = 0,sum2 = 0,sum3 = 0;
	
	m(0,0,sqrt(data[0]));
	
	for (int i = 1; i < l_; i++)
		m(i,0,data[i*c_+0]/m(0,0));
	
	for (int k = 1; k < l_- 1; k++)
	{
		for (int j = 0; j < k - 1; j++)
			sum1 += m(k,j)*m(k,j);
		m(k,k,sqrt(data[k*c_ + k] - sum1));
		
		for (int i = k + 1; i < l_; i++)
		{
			for (int j = 0; j < k - 1; j++)
				sum2 += m(i,j)*m(k,j);
			m(i,k,((data[i*c_ + k] - sum2)/m(k,k)));
		}
	}
	
	for (int j = 0; j < l_ - 1; j++)
			sum3 += m(l_ - 1,j)*m(l_ - 1,j);
	m(l_ - 1,l_ - 1,sqrt(data[(l_ - 1)*c_ + (l_ - 1)] - sum3));
	
	return m;
}

//Operateur *= surcharg� pour la classe Matrix
Matrix &Matrix::operator*= ( const Matrix &m)
{
	
	if ((c_ != m.l_) && (m.c_!= m.l_) && (m.l_!= m.c_))
		throw erreur(BAD_SIZE);	
	double s;
	Matrix matrice(l_,m.c_);
		for (int i = 0; i < l_; i++)
			for (int j = 0; j < m.c_; j++)
			{
				s = 0;
				for (int k = 0; k < c_; k++)					   
					s += data[i*c_+k]*m(k,j);
				matrice.data[i*c_ +j] = s;
			}
	*this = matrice;
	return *this;
}
	


//operateur d'affectation surcharg� pour la classe Matrix
Matrix & Matrix::operator= (const Matrix &m)
{
	if (&m != this)
	{
		delete[] data;
		l_ =  m.l_;
		c_ =  m.c_;
		data = new double[ l_*c_ ];
		for (int i=0; i < l_*c_; i++)
			data[i] = m.data[i];
	}
	return *this;
}

//Multiplication d'une matrice par un r�el
Matrix operator* (const Matrix& m,const double& x)
{
	for (int i = 0; i < m.l_*m.c_; i++)
	{
		m.data[i]  *= x;
	}
	return m;
}
