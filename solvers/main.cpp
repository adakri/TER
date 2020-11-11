/*!
 * \file main.cpp

 * Programme de test pour la resolution de systemes d'&eacute;quations 
   lin&eacute;aires par la m&eacute;thode du gradient conjugu&eacute;.
 *
 */

#include <iostream>
#include "GradConj.h"
#include "Erreur.h"

int main() 
{ 
	Matrix A (2, 2);                                              
	Vecteur b(2);
	std::cout << "entrez  6 valeurs : "<<std::endl;
	std::cin >> A >> b;
	std::cout << A << b << std::endl;
	GradConj u(A,b);
	try 
	{
	std::cout << u.Solve() << std::endl;
	
	std::cout << prodMatVect(A,u.Solve()) << std::endl ;
	}
	catch (erreur &e)
	{
		std::cout << "exception" << std::endl;
		switch(e())
		{
		case BAD_ALLOC:
			std::cout << "Bad alloc" << std::endl;
			break;
		case BAD_SIZE:
			std::cout << "Bad size" << std::endl;
			break;
		case BAD_INDEX:
			std::cout << "Bad index" << std::endl;
			break;
		case NON_SYM:
			std::cout << "Non sym" << std::endl;
			break;
		case NON_DEF:
			std::cout << "Non def" << std::endl;
			break;
		}
		return -1;
	}
	catch (...)
	{
		std::cout << "erreur non repertori�e";
	}
 	return 0;
};
