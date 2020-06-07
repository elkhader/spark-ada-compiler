
#include"analyseur_spark_v0.h"

#include "tabsym.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>




#define nbr_max_symb 100
static  int nbr_symb =0 ;
static varvalueType tab_symb[nbr_max_symb]; 

extern typetoken token_val;






void ajouter_nouvelle_variable_a_TS(varvalueType newvar){
//printf("T1\n");
 // tab_symb[nbr_symb].nbdecl = newvar.nbdecl;
//printf("T2\n");
  tab_symb[nbr_symb].name_var = (char *)malloc(strlen(newvar.name_var)+1);
//printf("T3\n");
  strcpy(tab_symb[nbr_symb].name_var,newvar.name_var);
//printf("T4\n");
  tab_symb[nbr_symb].ligne_decl = newvar.ligne_decl;
//printf("T5\n");
  tab_symb[nbr_symb].intial = newvar.intial;
//printf("T6\n");
  tab_symb[nbr_symb].type_var = newvar.type_var;
//printf("T7\n");
  tab_symb[nbr_symb].val_initial = newvar.val_initial;
//printf("T8\n");

  //  incrementer_nombre_variables incrementer_nombre_variables
  nbr_symb++;
}

void ajouter_var(varvalueType nouv_var,boolean mal)
{


  if (debug) printf("in _________________ajouter_var();\n");
  if(nouv_var.intial){
    if(mal==true){
      set_error(var_mal_initialisee, nouv_var.ligne_decl, nouv_var.name_var);
      
  }
  
    
  }
if (var_exist(nouv_var.name_var)){
   set_error(var_deja_declaree, nouv_var.ligne_decl, nouv_var.name_var);

    

    
}else{


    tab_symb[nbr_symb].name_var = (char *)malloc(strlen(nouv_var.name_var)+1);
    strcpy(tab_symb[nbr_symb].name_var,nouv_var.name_var);
    
    tab_symb[nbr_symb].ligne_decl = nouv_var.ligne_decl;
    tab_symb[nbr_symb].intial = nouv_var.intial;
    tab_symb[nbr_symb].type_var = nouv_var.type_var;
    tab_symb[nbr_symb].val_initial = nouv_var.val_initial;
    tab_symb[nbr_symb].rangvar=nbr_symb+1;
    //printf("%s\n",tab_symb[nbr_symb].name_var );
    
    
      nbr_symb++;
    }

    if (debug) printf("out____________ajouter_var();\n");

}


boolean var_exist(char * name_var)
{
  int i =0;
  while ((i < nbr_symb) && (strcmp(tab_symb[i].name_var,name_var) != 0)) {
    i++;
  }
    
  if (i == nbr_symb) {
    return false;
  }
    
  return true;
}


char *type_exist(char * name_var){
  int i =0;
  if(var_exist(name_var)){
    
    while ((i < nbr_symb) && (strcmp(tab_symb[i].name_var,name_var) != 0)) {
    i++;
  }
  
  }
  return tab_symb[i].type_var;
}



Type typevar(char * name_var){
  if(strcmp(type_exist( name_var),"boolean") == 0){
     
      return Bool;
  }
  else if(strcmp(type_exist( name_var),"integer")==0){
         return Int;
  }
  else 
        return Double;
  
}

double val_intial(char * name_var){
  int i =0;
  if(var_exist(name_var)){
    
    while ((i < nbr_symb) && (strcmp(tab_symb[i].name_var,name_var) != 0)) {
    i++;
  }
  
  }
  return tab_symb[i].val_initial;
}


void afficher_var()
{
  int i=0;
  for(i=0;i<nbr_symb;i++){
    printf("le nom de la %s\n la ligne %d\n intial %u \nvaleur %f\n type %s\n rang %d\n",tab_symb[i].name_var,tab_symb[i].ligne_decl,tab_symb[i].intial,
      tab_symb[i].val_initial,tab_symb[i].type_var, tab_symb[i].rangvar);
  }
}


char *name(int rangvar){
  return tab_symb[rangvar-1].name_var;

}

int rangvar_name(char *name_var){
  int i =0;
  if(var_exist(name_var)){
    
    while ((i < nbr_symb) && (strcmp(tab_symb[i].name_var,name_var) != 0)) {
    i++;
  }
  
  }
  return tab_symb[i].rangvar;
}









/* itoa: convert n to characters in s */
char * itoa(int n){
  int i = 0;
  char s[100];
  char * result;
  do { /* generate digits in reverse order */
    s[i++] = n % 10 + '0'; /* get next digit */
  } while ((n /= 10) > 0); /* delete it */
  s[i] = '\0';
  reverse(s);
  result = (char *) malloc(strlen(s) + 1);
  strcpy(result, s);
  return result;
}

/* reverse: reverse string s in place */
void reverse(char s[]){
  int c, i, j;
  for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}



int nombre_variables(){
  return nbr_symb;
}






double valinit(int rang){
  return tab_symb[rang-1].val_initial;
}







boolean inTS(char * varname, int * rangvar){
//printf("enter inTS(%s)\n", varname);
  

  int i =0;
  while ((i < nbr_symb) && (strcmp(tab_symb[i].name_var,varname) != 0)) {
    i++;
  }
    
  if (i == nbr_symb) {
    return false;
  }
    
  return true;
}



void set_valinit(int rang, double val){
  tab_symb[rang-1].val_initial = val;
}





