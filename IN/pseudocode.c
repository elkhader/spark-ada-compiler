#include "pseudocode.h"
#include <stdio.h>




#define debug 0

#ifndef NULL
#define NULL ((void *) 0)
#endif

void afficher_pseudo_instruction(struct pseudoinstruction pi){
  switch(pi.codop){
  case ADD:    printf("ADD\n");  break;
  case _DIV:   printf("DIV\n"); break;
  case DUPL:   printf("DUPL\n"); break;
  case LABEL:  printf("%s:\n",pi.param.label_name); break;
  case LOAD:   printf("LOAD "); printf("%s\n",pi.param.var); break;
  case _MULT:  printf("MULT\n"); break;
  case POP:    printf("POP\n");  break;
  case PUSH:   printf("PUSH "); printf("%lf\n",pi.param._const); break;
  case SUB:    printf("SUB\n");  break;
  case STORE:  printf("STORE "); printf("%s\n",pi.param.var); break;
  case SWAP:   printf("SWAP\n"); break;
  case PRNT:   printf("PRINT\n");break;
  case JNE:    printf("JNE "); printf("%s\n",pi.param.label_name);break; 
  case JG:    printf("JG "); printf("%s\n",pi.param.label_name);break; 
  case JMP:    printf("JMP "); printf("%s\n",pi.param.label_name);break; 
  case DATA:   printf("%s ",pi.param.nv.name); printf("%lf\n",pi.param.nv.value);break; 
  }
}

// precondition pc <> NULL
void  afficher_pseudo_code(pseudocode  pc){
  
   if (pc != NULL){

    afficher_pseudo_instruction(pc->first);    
    afficher_pseudo_code(pc->next);
  }
}

// precondition pc1 <> NULL et pc2 <> NULL
void inserer_code_en_queue(pseudocode  pc1, pseudocode pc2){
  if (debug) {   
    afficher_pseudo_code(pc1);
    afficher_pseudo_code(pc2);
  }

  if (pc1->next == NULL) {
    pc1->next = pc2;
  }else{
    pseudocode pc = pc1;
    while(pc->next != NULL) {
      pc = pc->next;
    }
    
    pc->next = pc2;
  }
  if (debug) { 
    afficher_pseudo_code(pc1);
    printf("\n");
  }
}



