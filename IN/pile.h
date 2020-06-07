#ifndef PILE_H
#define PILE_H

#include "cfg.h"



#define TAILLEPILEMAX 100

typedef double Element;

typedef struct{
	Element elements [TAILLEPILEMAX];
	int taille;
} Pile;

Pile * creer_pile();

// pré-condition : p <> NULL
boolean pile_vide(Pile *p);

// pré-condition : p <> NULL
void empiler(Pile *p, Element e);

// pré-condition : p <> NULL, pile_vide(p) = false
Element depiler(Pile *p);

// affiche l'état de la pile (pour débugguer)
void etat_pile(Pile *p);

#endif
