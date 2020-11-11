#ifndef ERREUR_H
#define ERREUR_H


enum Erreurs
{
	BAD_ALLOC,
	BAD_INDEX,
	BAD_SIZE,
	NON_SYM,
	NON_DEF
};

class erreur
{
private:
 	Erreurs e_;
public:
	erreur (Erreurs e) : e_(e) {}
	Erreurs operator()() {return e_;}
};
#endif