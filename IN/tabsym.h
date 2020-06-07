#ifndef TABSY_H
#define TABSY_H


#include"error.h"
#include"ast.h"
#include"cfg.h"
















static varvalueType current_var;
static double last_const;
static boolean decl_start=false;




boolean var_exist(char * nom_var);
char *type_exist(char * name_var);
void ajouter_var(varvalueType nouv_var,boolean mal);
void afficher_var();

char *name(int rangvar);
void set_name_line(char* name, int line );
void set_type(char* type);
void set_const(double cons );
void set_val_initial();
boolean mal_init_sc();
double val_intial(char * name_var);

Type typevar(char * name_var);

int rangvar_name(char *nam);

char * itoa(int n);
void reverse(char s[]);

double valinit(int rang);
extern int nombre_variables();
boolean inTS(char * varname, int * rangvar);
void set_valinit(int rang, double val);


#endif