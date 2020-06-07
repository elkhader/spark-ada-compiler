#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"error.h"
#include"tabsym.h"
#include"analyseur_spark_v0.h"
#define debug false

#define nbr_max_error 100 

static sm_error tab_error[nbr_max_error];  









void ajouter_error(sm_error nouv_error)
{
if (debug) printf("in ______ajouter_error();\n");


    tab_error[nbr_error].nom_source_error = (char *)malloc(strlen(nouv_error.nom_source_error)+1);
    strcpy(tab_error[nbr_error].nom_source_error,nouv_error.nom_source_error);
    
    tab_error[nbr_error].num_ligne = nouv_error.num_ligne;
    
    tab_error[nbr_error].type_error = nouv_error.type_error;
    
    nbr_error++;
    if (debug) printf("out ______ajouter_error();\n");
}




void set_error(semantic_error_type et, int line, char* name){

if (debug) printf("in ______set_error();\n");
  
  current_error.nom_source_error = (char *)malloc(sizeof(char)*50);
    strcpy(current_error.nom_source_error,name);

    
    current_error.num_ligne = line;
    
    current_error.type_error = et;
    

      ajouter_error(current_error);
if (debug) printf("out ______set_error();\n");
    
}










void afficher_sm_erreur(semantic_error_type et, int line, char* name){ //shows one error 
 
  if(et==var_mal_initialisee){
       
      printf("ligne  %d : %s cette variable est mal initialisee",line, name);

  }else if(et==var_deja_declaree){
    printf("ligne  %d : %s cette variable est deje declaree ",line, name);
  }else if(et==non_declaree){
    printf("ligne  %d : %s  variable non declaree ",line, name);
  }else if(et==incompatible_for_index_type){
    printf("ligne  %d : %s   incompatible type d\'index pour FOR",line, name);
  }else if(et==incompatible_affectation){
    printf("ligne  %d : %s  incompatible avec la valeur d\'affectation",line, name);
  }else if(et==incompatible_operation){
    printf("ligne  %d : %s  incompatible avec la valeur l\'operation",line, name);
  }else if(et==incompatible_comp){
    printf("ligne  %d : %s  incompatible avec la valeur de comparaison",line, name);
  }else if(et==division_zero){
    printf("ligne  %d : %s   division par 0 !!",line, name);
  }

  
}


void afficher_error()// shows all errors
{
  int i=0;
 // printf("%d\n",nbr_error);
  for(i=0;i<nbr_error;i++){

    afficher_sm_erreur(tab_error[i].type_error,tab_error[i].num_ligne, tab_error[i].nom_source_error);
    printf("\n");
  }
}

int nbrError(){
  return nbr_error;
}
