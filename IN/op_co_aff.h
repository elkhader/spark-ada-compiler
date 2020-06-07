#ifndef OPCOAFF_H
#define OPCOAFF_H


#include"error.h"
#include"tabsym.h"




typedef struct aff_type
{
	char *name_var ;
  	char* type_var;
  	int ligne_decl;
  	int range;
  	boolean idden;
  	struct aff_type *suivant;

}aff_type;


static aff_type *notre_list;
void first_aff(char* name, int line);
void second_aff(char*name, int line);
void const_aff(typetoken token);
void afficher_op();
boolean ver_aff(boolean compa);
boolean ver_op_san();
void ver_op();













#endif