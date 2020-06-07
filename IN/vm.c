#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pseudocode.h"
#include "tabsym.h"
#include "vm.h"
#include "pile.h"




Pile * VM_STACK;

// inintialise la machine abstraite
void inintialiser_machine_abstraite(){
	VM_STACK = creer_pile();
}

// interprete une pseudo insctruction
void interpreter_pseudo_instruction(struct pseudoinstruction pi, char ** next_label_name){
  Element op1, op2, resultat;
  int* rangvar = (int*) malloc(sizeof(int)); 

  *next_label_name = NULL;

  switch(pi.codop){
    case DATA:
		if (debug) printf("DATA = \n");
		varvalueType nv;
		nv.name_var = (char*) malloc(sizeof(char) * strlen(pi.param.nv.name)+1);
		strcpy(nv.name_var, pi.param.nv.name);
		nv.val_initial = pi.param.nv.value;
		ajouter_nouvelle_variable_a_TS(nv);		// alimenter la mémoire statique de la machien virtuelle
		break;

    case ADD: 
      		    if (debug) printf("ADD = \n");
		    if (debug) etat_pile(VM_STACK) ; 
    		    op1 = depiler(VM_STACK); op2 = depiler(VM_STACK); resultat = op1 + op2; empiler(VM_STACK, resultat);
		    if (debug) etat_pile(VM_STACK) ; 
		    break;

    case _DIV:      if (debug) printf("ADD = \n");
		    if (debug) etat_pile(VM_STACK) ; 
		    op1 = depiler(VM_STACK); op2 = depiler(VM_STACK); resultat = op1 / op2; empiler(VM_STACK, resultat);
                    if (debug) etat_pile(VM_STACK) ; 
                    break;
    case _MULT:  
		if (debug) printf("MULT = \n");
		if (debug) etat_pile(VM_STACK) ; 
		op1 = depiler(VM_STACK); op2 = depiler(VM_STACK); resultat = op1 * op2; empiler(VM_STACK, resultat);
		if (debug) etat_pile(VM_STACK) ; 		
		break;

    case SUB:
		if (debug) printf("SUB = \n");
	        if (debug) etat_pile(VM_STACK) ;
	        op1 = depiler(VM_STACK); op2 = depiler(VM_STACK); resultat = op1 - op2; empiler(VM_STACK, resultat);
		if (debug) etat_pile(VM_STACK) ;
		break;

    case LOAD:  
		if (inTS(pi.param.var, rangvar) != true) if (debug) printf("%s n'est pas :\n", pi.param.var);
		else if (debug) printf("%s est à l'indice %d :\n",  pi.param.var, *rangvar);
		if (debug) printf("LOAD = %s %lf\n", pi.param.var, valinit(*rangvar));
		empiler(VM_STACK, valinit(*rangvar));
		break;

    case STORE:
		if (debug) printf("STORE = \n");
		op1 = depiler(VM_STACK); 
		inTS(pi.param.var, rangvar);
		set_valinit(*rangvar, op1); break;

    case DUPL:
		if (debug) printf("DUPL = \n");
		op1 = depiler(VM_STACK); empiler(VM_STACK, op1); empiler(VM_STACK, op1); break;

    case PUSH:
		if (debug) printf("PUSH = \n");
		empiler(VM_STACK, pi.param._const); break;

    case SWAP:  
		if (debug) printf("SWAP = \n");
		op1 = depiler(VM_STACK); //LAST-IN
		op2 = depiler(VM_STACK); 
		empiler(VM_STACK, op1); //FIRST-IN
		empiler(VM_STACK, op2); break;
    
    case JNE:   
		if (debug) printf("JNE = \n");
		op1 = depiler(VM_STACK);
		op2 = depiler(VM_STACK);
		if (op1 != op2) {*next_label_name = (char*) malloc (strlen(pi.param.label_name)+1);
		strcpy(*next_label_name, pi.param.label_name);}
		else {;}
		break;
    case JG:   
		if (debug) printf("JG = \n");
		op1 = depiler(VM_STACK);
		op2 = depiler(VM_STACK);
		if (op1 > op2) {*next_label_name = (char*) malloc (strlen(pi.param.label_name)+1); strcpy(*next_label_name, pi.param.label_name);}
		else {;}
		break;

    case JMP:   if (debug) printf("JMP = \n");
		*next_label_name = (char*) malloc (strlen(pi.param.label_name)+1);
		strcpy(*next_label_name, pi.param.label_name);
		break; 

    case PRNT:  if (debug) printf("PRNT = \n");
		op1 = depiler(VM_STACK); printf("%lf", op1); break;

    case LABEL: if (debug) printf("LABEL = \n");
		; break; //l'effet de bord de la pasudo-instruction LABEL est de faire brancher à la pseudo-instruction suivante

    // case POP:   IGNORE
  }
}

// precondition pc <> NULL
// interprete un pseudocode
void interpreter_pseudo_code(pseudocode pc){
static pseudocode debut_code = NULL;

// initialisation de la variable statique par la première ligne de pseudocode nécessaire à la recherche du branchement en arrière
if (debut_code == NULL) debut_code = pc;

 char ** next_label_name = (char **) malloc(sizeof (char*));

 if (pc != NULL){
    interpreter_pseudo_instruction(pc->first, next_label_name);    
    if (*next_label_name == NULL) interpreter_pseudo_code(pc->next); // Il n y a pas de branchement !!
    else{ // JNE, JG, ou JMP ==> effectuer un branchement (recherche en O(n) depuis le début du code peut être optimisé par un accès direct au label O(1))
	struct pseudocodenode * compteur_ordinal = debut_code; // pc->next;
	//printf("LE LABEL RECHERCHE == %s", *next_label_name);
	while ( (compteur_ordinal->first.codop != LABEL) || 
		(strcmp(compteur_ordinal->first.param.label_name, *next_label_name) != 0) ) { // (compteur_ordinal ne peut pas == NULL) s'il vient y avoir un JMP/JNE, JG dans le code (par construction)
		//afficher_pseudo_instruction(compteur_ordinal->first);   	
		compteur_ordinal = compteur_ordinal->next;
	}
	// ici : compteur_ordinal->first.codop == LABEL && strcmp(compteur_ordinal->first.param.label_name, next_label_name) == 0)
	interpreter_pseudo_code(compteur_ordinal->next); // branchement
    }
 }
}
