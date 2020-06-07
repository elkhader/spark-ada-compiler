#ifndef AST_H
#define AST_H
#include "pseudocode.h"



typedef enum {NB=0, _IDF = 1, BOOLEAN1 = 2, OP=3} Type_Exp ; 

typedef enum {Int, Bool, Double} Type;

typedef enum {plus, moins, mult, _div} Type_Op; 

typedef enum {false, true} boolean;

struct Exp ; 

typedef struct Exp * AST;

typedef union {
  double nombre ;
  char *idf;
  boolean bool; 
  struct {
    Type_Op top;
    AST expression_gauche ;
    AST expression_droite ;
  } op;
} ExpValueTypeNode;

typedef struct Exp {
  Type_Exp typeexp ; 
  Type     typename; 
  ExpValueTypeNode noeud ;
}expvalueType;

double value(char *idf);

// précondition : a<> NULL and est_feuille(a) == false
AST arbre_gauche(AST a);

// précondition : a<> NULL and est_feuille(a) == false
AST arbre_droit(AST a);

// précondition : a<> NULL and est_feuille(a) == false
Type_Op top(AST a);

// précondition : a<> NULL
Type     type(AST a);

// précondition : a<> NULL 
boolean est_feuille(AST a);


AST creer_feuille_booleen(boolean b);

AST creer_feuille_nombre(double n, Type type);

AST creer_feuille_idf(char *idf, Type type);

AST creer_noeud_operation(char op, AST arbre_g, AST arbre_d, Type type);


// précondition : ast<> NULL 
void afficher_infixe_arbre (AST ast);


// précondition : ast<> NULL 
void afficher_postfixe_arbre (AST ast);


// précondition : ast<> NULL 
double evaluer (AST ast);


// précondition ast <> NULL

pseudocode generer_pseudo_code_ast(AST ast);
#endif
