#ifndef CFG_H
#define CFG_H

#include "ast.h"
#include "pseudocode.h"






typedef enum {PrintIdf=100,PrintLingeIdf=200, PrintString=300, AssignArith=400, AssignBool=500, IfThenArith=600, IfThenElseArith=700, For=800} Type_INST ;



typedef struct {
  char *name_var ;
  char* type_var;
  boolean intial;
  double val_initial;
  int ligne_decl;
  int rangvar;
}varvalueType;


typedef struct {
  Type typename;    
  double val_initial;      
} constvalueType;

typedef struct {
  int ligne_decl;  
} tokenvalueType;

typedef struct {
  Type typename;   
} typevalueType;

struct INST; 

struct LIST_INST;

typedef struct INST {
  Type_INST typeinst;
  union  {
    // PRINT idftoprint
    struct  {
      int rangvar; // indice de l'idf, à afficher, dans la table des symboles
    } printnode;
    // left := right
    struct  {
      int rangvar; // indice de l'idf, où il faut affecter, dans la table des symboles
      //int right0; // la valeur boolenne à affecter // a nettoyer
      AST right; // l'expression à affecter (arithmétique ou booléenne)
    } assignnode;
    // IF ... THEN 
    struct  {
      int rangvar; // indice de l'idf à comparer, dans la table des symbole
      //double right; // la valeur de comparaison
      AST right; // l'expression à comparer
      struct LIST_INST * thenlinst; // then list of instructions
      struct LIST_INST * elselinst; // else list of instructions
    } ifnode;
      // for (index:= exp_min..exp_max) loop list_inst end loop;
    struct {
      int rangvar; // indice de l'index de la boucle
      int borneinf; // l'expression borne inf
      int bornesup; // l'expression borne sup
      struct LIST_INST * forbodylinst; // for body list of instructions
    } fornode;
  } node;
} instvalueType;

typedef struct LIST_INST {
  struct INST first;
  struct LIST_INST * next;
} listinstvalueType;

typedef union {
  varvalueType varattribute;            
  constvalueType constattribute;         
  Type typename;         
  instvalueType instattribute;
  listinstvalueType listinstattribute; 
  //AST expattribute;         
} valueType;

#define YYSTYPE valueType

extern instvalueType* creer_instruction_print(int rangvar);
extern instvalueType* creer_instruction_affectation(int rangvar, AST * past);
//extern instvalueType* creer_instruction_if(int rangvar, constvalueType constattribute, listinstvalueType * plistthen, listinstvalueType * plistelse);
extern instvalueType* creer_instruction_if(int rangvar, AST * past, listinstvalueType * plistthen, listinstvalueType * plistelse);
extern instvalueType* creer_instruction_for(int rangvar, int borneinf, int bornesup, listinstvalueType *pplistfor);

extern void inserer_inst_en_queue(listinstvalueType * listinstattribute, instvalueType instattribute);

extern void inserer_inst_en_tete(listinstvalueType ** listinstattribute, instvalueType instattribute);

extern void afficher_inst(instvalueType instattribute);

extern void afficher_list_inst(listinstvalueType * plistinstattribute);



extern pseudocode generer_pseudo_code_inst(instvalueType instattribute);
     
extern pseudocode generer_pseudo_code_list_inst(listinstvalueType * plistinstattribute);

extern pseudocode generer_pseudo_code(listinstvalueType * plistinstattribute);

#define debug false
#endif
