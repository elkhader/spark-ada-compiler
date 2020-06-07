// auteur Karim Baïna, ENSIAS, Décembre 2010

#include "pile.h"
#include <stdlib.h>
#include <stdio.h>


Pile * creer_pile(){
	Pile * pile = (Pile *) malloc(sizeof (Pile));

	pile->taille = -1;

	return pile;
}

boolean pile_vide(Pile  *p){
	return (p->taille == -1);
}

void empiler(Pile * p, Element e){
	p->elements[p->taille + 1] = e;
	p->taille ++;
}

// pré-condition : pile_vide(p) = false
Element depiler(Pile * p){
	return p->elements[p->taille--];
}

// affiche l'état de la pile (pour débugguer)
void etat_pile(Pile *p){
	int i = 0;
	printf(" [ ");
	for(i = 0; i<= p->taille; i++)	printf("%lf", p->elements[ i ]);
	printf(" ] ");
}


