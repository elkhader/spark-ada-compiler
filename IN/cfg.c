



#include "cfg.h"
#include "tabsym.h"

//#include "pseudocode.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



instvalueType* creer_instruction_print(int rangvar){
        if (debug) printf("creer_instruction_print()\n");

  instvalueType * printinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

  printinstattribute->typeinst = PrintIdf;
  printinstattribute->node.printnode.rangvar = rangvar;
  
  return printinstattribute;

  if (debug) printf("out of creer_instruction_print()\n");
}

instvalueType* creer_instruction_affectation(int rangvar, AST * past){
        if (debug)  printf("creer_instruction_affectation()\n");

  instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

  pinstattribute->typeinst = (type(*past)==Bool)?AssignBool:AssignArith;
  pinstattribute->node.assignnode.rangvar = rangvar;
  pinstattribute->node.assignnode.right = * past;

        if (debug) printf("out of creer_instruction_affectation()\n");

  return pinstattribute;
}


//instvalueType* creer_instruction_if(int rangvar, constvalueType constattribute, listinstvalueType * plistthen, listinstvalueType * plistelse){
instvalueType* creer_instruction_if(int rangvar, AST * past, listinstvalueType * plistthen, listinstvalueType * plistelse){

        if (debug) printf("creer_instruction_if()\n");

  instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

  pinstattribute->typeinst = ((plistelse != NULL)?IfThenElseArith:IfThenArith);
  pinstattribute->node.ifnode.rangvar = rangvar;
  pinstattribute->node.ifnode.right = * past;
  pinstattribute->node.ifnode.thenlinst = plistthen;
  pinstattribute->node.ifnode.elselinst = plistelse;

        if (debug) printf("out of creer_instruction_if()\n"); 

  return pinstattribute;
}

instvalueType* creer_instruction_for(int rangvar, int borneinf, int bornesup, listinstvalueType *pplistfor){

        if (debug) printf("creer_instruction_for()\n");

  instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

  pinstattribute->typeinst = For;
  pinstattribute->node.fornode.rangvar = rangvar;
  pinstattribute->node.fornode.borneinf = borneinf;
  pinstattribute->node.fornode.bornesup = bornesup;
  pinstattribute->node.fornode.forbodylinst = pplistfor;

        if (debug) printf("out of creer_instruction_for()\n");  

  return pinstattribute;
}


void afficher_inst(instvalueType instattribute){
  if (debug) printf("afficher_inst()\n");

  switch(instattribute.typeinst){

  case PrintIdf :
    printf("Print "); printf("%s\n", name(instattribute.node.printnode.rangvar));
    break;
    
  case AssignArith :
    printf("AssignArith ");
    printf("%s", name(instattribute.node.assignnode.rangvar));
    afficher_infixe_arbre(instattribute.node.assignnode.right);
    printf("\n");
    break;

  case AssignBool :
    printf("AssignBool ");
    printf("%s", name(instattribute.node.assignnode.rangvar));
    afficher_infixe_arbre( instattribute.node.assignnode.right );
    // printf(" := %s",(instattribute.node.assignnode.right0==true)?"true":"false");
    printf("\n");
    break;
    
  case IfThenArith :
    printf("If ");
    printf("( %s ==",name(instattribute.node.ifnode.rangvar));
    // printf("%lf )", instattribute.node.ifnode.right);
    afficher_infixe_arbre( instattribute.node.ifnode.right );
    printf(" )\n Then ");
    afficher_list_inst( instattribute.node.ifnode.thenlinst );
    printf(" endIf\n");
    break;

 case IfThenElseArith :
   printf("If ");
   printf("( %s == ",name(instattribute.node.ifnode.rangvar));
   // printf("%lf )", instattribute.node.ifnode.right);
   afficher_infixe_arbre( instattribute.node.ifnode.right );
   printf(" )\n Then ");
   afficher_list_inst( instattribute.node.ifnode.thenlinst );
   printf(" Else ");
   afficher_list_inst( instattribute.node.ifnode.elselinst );
   printf(" endIf\n");
   break;

 case For :
   printf("For ");
   printf("( %s = ",name(instattribute.node.fornode.rangvar));
   printf(" %d to ",instattribute.node.fornode.borneinf );
   printf(" %d ) ",instattribute.node.fornode.bornesup );

   afficher_list_inst( instattribute.node.fornode.forbodylinst );
   printf(" endFor\n");
   break;
  }

  if (debug) printf("out of afficher_inst()\n");
}

void afficher_list_inst(listinstvalueType * plistinstattribute){
  if (debug) printf("afficher_list_inst()\n");

  if (plistinstattribute != NULL){
    afficher_inst(plistinstattribute->first);    
    afficher_list_inst(plistinstattribute->next);
  }

  if (debug) printf("out of afficher_list_inst()\n");

}

void inserer_inst_en_tete(listinstvalueType ** pplistinstattribute, instvalueType instattribute){
  if (debug) printf("inserer_inst_en_tete()\n");

  listinstvalueType * liste = (listinstvalueType *) malloc(sizeof(listinstvalueType));
  liste->first = instattribute;
  liste->next = *pplistinstattribute;

  *pplistinstattribute = liste;

  if (debug) printf("out of inserer_inst_en_tete()\n");
}

void inserer_inst_en_queue(listinstvalueType * plistinstattribute, instvalueType instattribute){
  
  listinstvalueType * liste = (listinstvalueType *) malloc(sizeof(listinstvalueType));
  liste->first = instattribute;
  liste->next = NULL;

  if (plistinstattribute->next == NULL) {
    plistinstattribute->next = liste;
  }else{
    listinstvalueType * pliste = plistinstattribute;
    
    while(pliste->next != NULL) {
      pliste = pliste->next;
    }
    
    pliste->next = liste;
  }
}

pseudocode generer_pseudo_code_inst(instvalueType instattribute){
  static label_index = 0;
  pseudocode  pc, pc1,pc2,pc3,pc31,pc4,pc5,pc6,pc7, pc8, pc9, pc10, pc11, pc12;
  pseudocode  rexpcode;
  char * label_name;
  char *label_num;

  switch(instattribute.typeinst){
  case PrintIdf :
    if (debug) printf("Print");
    // NOP
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = LOAD;
    pc->first.param.var = name(instattribute.node.printnode.rangvar);
    rexpcode = (pseudocode)malloc(sizeof (struct pseudocodenode));
    rexpcode->first.codop = PRNT;
    rexpcode->next = NULL;
    pc->next = rexpcode;
    break;
    
  case AssignArith :
    if (debug) printf("AssignArith");
    rexpcode = generer_pseudo_code_ast(instattribute.node.assignnode.right);
    if (debug) afficher_pseudo_code(rexpcode);
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = STORE;
    pc->first.param.var = name(instattribute.node.assignnode.rangvar);
    pc->next = NULL;
    inserer_code_en_queue(rexpcode, pc);
    pc = rexpcode;
    if (debug) afficher_pseudo_code(pc);
    break;

  case AssignBool :
    if (debug) printf("AssignBool");
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = PUSH;
    pc->first.param._const = (double) (((instattribute.node.assignnode.right)->noeud).bool);

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc1->first.codop = STORE;
    pc1->first.param.var = name(instattribute.node.assignnode.rangvar);
    pc1->next = NULL;

    pc->next = pc1;
   
    if (debug) afficher_pseudo_code(pc);
    break;
    
  case IfThenArith :
    if (debug) printf("If");
    
    pc = generer_pseudo_code_ast(instattribute.node.ifnode.right);

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc1->first.codop = LOAD;
    pc1->first.param.var = name(instattribute.node.ifnode.rangvar);
    pc1->next = NULL;

    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = JNE;
    label_num=itoa(label_index++);
    pc2->first.param.label_name = (char*) malloc(6+strlen(label_num));
    strcpy( pc2->first.param.label_name, "endif");
    strcat( pc2->first.param.label_name, label_num);

    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc2->next = NULL;

    pc3 = generer_pseudo_code_list_inst(instattribute.node.ifnode.thenlinst );

    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc4->first.codop = LABEL;
    pc4->first.param.label_name = pc2->first.param.label_name;
    pc4->next = NULL;

    inserer_code_en_queue(pc3, pc4);
    pc2->next = pc3;
    pc1->next = pc2;
    inserer_code_en_queue(pc, pc1);

    if (debug)   {printf("debut-- ");afficher_pseudo_code(pc);printf(" --fin");}
    break;

  case IfThenElseArith :
    if (debug) printf("If");
    
    pc = generer_pseudo_code_ast(instattribute.node.ifnode.right);

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc1->first.codop = LOAD;
    pc1->first.param.var = name(instattribute.node.ifnode.rangvar);
    pc1->next = NULL;

    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = JNE;
    label_num=itoa(label_index++);
    pc2->first.param.label_name = (char*) malloc(6+strlen(label_num));
    strcpy( pc2->first.param.label_name, "else");
    strcat( pc2->first.param.label_name, label_num);
    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc2->next = NULL;

    pc3 = generer_pseudo_code_list_inst(instattribute.node.ifnode.thenlinst );

    pc31 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc31->first.codop = JMP;
    pc31->first.param.label_name = (char*) malloc(6+strlen(label_num));
    strcpy( pc31->first.param.label_name, "endif");
    strcat( pc31->first.param.label_name, label_num);

    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc4->first.codop = LABEL;
    pc4->first.param.label_name = pc2->first.param.label_name;
    pc4->next = NULL;

    pc31->next = pc4;

    pc5 = generer_pseudo_code_list_inst(instattribute.node.ifnode.elselinst );   
    pc4->next = pc5;
   
    pc6 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc6->first.codop = LABEL;
    pc6->first.param.label_name = (char*) malloc(strlen( pc31->first.param.label_name)+1);
    strcpy( pc6->first.param.label_name,  pc31->first.param.label_name);
    pc6->next = NULL;

    inserer_code_en_queue(pc5, pc6);
    inserer_code_en_queue(pc3, pc31);
    pc2->next = pc3;
    pc1->next = pc2;
    inserer_code_en_queue(pc, pc1);

    if (debug) {printf("debut-- ");afficher_pseudo_code(pc);printf(" --fin");}

    break;
  
    case For:

    // affectation i = borneinf    
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = PUSH;
    pc->first.param._const = (double) (instattribute.node.fornode.borneinf);

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc1->first.codop = STORE;
    pc1->first.param.var = name(instattribute.node.fornode.rangvar);

    pc->next = pc1;

    // le début du for
    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = LABEL;
    label_num=itoa(label_index++);    
    pc2->first.param.label_name = (char*) malloc(4+strlen(label_num));
    strcpy( pc2->first.param.label_name, "for");
    strcat( pc2->first.param.label_name, label_num);

    pc1->next = pc2;

    // test si i > bornesup
    pc3 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc3->first.codop = PUSH;
    pc3->first.param._const = (double) (instattribute.node.fornode.bornesup);

    pc2->next = pc3;

    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc4->first.codop = LOAD;
    pc4->first.param.var = name(instattribute.node.fornode.rangvar);
    
    pc3->next = pc4;

    pc5 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc5->first.codop = JG;
    //label_num=itoa(label_index++);   
    pc5->first.param.label_name = (char*) malloc(7+strlen(label_num));
    strcpy( pc5->first.param.label_name, "endfor");
    strcat( pc5->first.param.label_name, label_num);

    pc4->next = pc5;

    // le corps du for
    pc6 = generer_pseudo_code_list_inst(instattribute.node.fornode.forbodylinst);

    pc5->next = pc6;

    // l'incrémentation de l'indice

    pc7 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc7->first.codop = PUSH;
    pc7->first.param._const = (double) 1;

    inserer_code_en_queue(pc6, pc7);

    pc8 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc8->first.codop = LOAD;
    pc8->first.param.var = name(instattribute.node.fornode.rangvar);
    
    pc7->next = pc8;

    pc9 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc9->first.codop = ADD;
    
    pc8->next = pc9;

    pc10 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc10->first.codop = STORE;
    pc10->first.param.var = name(instattribute.node.fornode.rangvar);

    pc9->next = pc10;

    // l'itération (le jmp au début)
    pc11 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc11->first.codop = JMP;
    pc11->first.param.label_name = (char*) malloc(strlen( pc2->first.param.label_name) + 1);
    strcpy( pc11->first.param.label_name, pc2->first.param.label_name);

    pc10->next = pc11;

    // la fin du for
    pc12 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc12->first.codop = LABEL;
    pc12->first.param.label_name = (char*) malloc(strlen( pc5->first.param.label_name)+1);
    strcpy( pc12->first.param.label_name,  pc5->first.param.label_name);
    pc12->next = NULL;

    pc11->next = pc12;

    break;
   
  }
  
  return pc;
}

pseudocode generer_pseudo_code_list_inst(listinstvalueType * plistinstattribute){
  pseudocode  pc1=NULL, pc2=NULL;
 if (plistinstattribute != NULL){
   pc1 = generer_pseudo_code_inst(plistinstattribute->first);    
   pc2 = generer_pseudo_code_list_inst(plistinstattribute->next);
   inserer_code_en_queue(pc1, pc2);
   if (debug) afficher_pseudo_code(pc1);
  }
 return pc1;
}

pseudocode generer_pseudo_code(listinstvalueType * plistinstattribute){
  int i=0;
  pseudocode pcresult;
  pseudocode tempo = NULL;
  //générer le code pour les DATA
  if (debug) printf("Generer le code pour les %d DATA...\n", nombre_variables());
  for(i = nombre_variables() - 1; i >= 0; i--){
    if (debug) printf("%d ieme malloc.....\n",i);
    pcresult = (pseudocode) malloc(sizeof(struct pseudocodenode));
    if (debug) printf("DATA..\n");
    pcresult->first.codop = DATA;
    if (debug) {printf("malloc..pour....\n"); printf("%s\n", name(i+1));}
    pcresult->first.param.nv.name = (char*) malloc(strlen(name(i+1))+1);
    if (debug) printf("strcpy..\n");
    strcpy(pcresult->first.param.nv.name, name(i+1));
    if (debug) {printf("valinit == ..\n"); printf("%lf\n", valinit(i+1));}
    pcresult->first.param.nv.value = valinit(i+1);
    if (debug) printf("tempo..\n");
    pcresult->next = tempo;
    if (debug) printf("pcresult..\n");
    tempo = pcresult;
          

  }

  // générer le begin:
  if (debug) printf("Generer le code pour le label begin...\n");
  pseudocode pcbegin = (pseudocode) malloc(sizeof(struct pseudocodenode));
  pcbegin->first.codop = LABEL;
  pcbegin->first.param.label_name = (char*) malloc(6);
  strcpy(pcbegin->first.param.label_name, "begin");
  pcbegin->next = NULL;

  //générer le code pour les INSTRUCTUTIONS
  if (debug) printf("Generer le code pour les instructions...\n");
  pseudocode pcbody = generer_pseudo_code_list_inst(plistinstattribute);

  // lier le label begin en début du body
  if (debug) printf("lier le label begin en début du body...\n");
  inserer_code_en_queue(pcbegin, pcbody);

  // générer le label end:
  if (debug) printf("générer le label end:...\n");
  pseudocode pcend = (pseudocode) malloc(sizeof(struct pseudocodenode));
  pcend->first.codop = LABEL;
  pcend->first.param.label_name = (char*) malloc(4);
  strcpy(pcend->first.param.label_name, "end");
  pcend->next = NULL;

  // lier le label end en fin du body
  if (debug) printf("lier le label end en fin du body...\n");
  inserer_code_en_queue(pcbody,pcend);

  // lier le tout begin-insts-end en fin des data
  if (debug) printf("lier le tout begin-insts-end en fin des data...\n");
  inserer_code_en_queue(pcresult,pcbegin);

  return pcresult;
}





