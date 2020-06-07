#include "tabsym.h"
#include"analyseur_spark_v0.h"
#include"op_co_aff.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

/*char *name_var ;
  	char *name_var ;
    char* type_var;
    int ligne_decl;
    int range;
    boolean idden;
    struct aff_type *suivant;


  	*/

void first_aff(char* name, int line){
if (debug) printf("in ________firstaff();\n");
	notre_list=malloc(sizeof(struct aff_type));
	notre_list->name_var=(char*)malloc(sizeof(char)*strlen(name)+1);
	  	

  	strcpy(notre_list->name_var, name);
  	notre_list->ligne_decl=line;
  	notre_list->type_var=(char*)malloc(50*sizeof(char));
  	strcpy(notre_list->type_var,type_exist(name));
  	notre_list->range=1;
  	notre_list->idden=true;
  	notre_list->suivant=malloc(sizeof(struct aff_type));
    notre_list->suivant=NULL;
  	if (debug) printf("out ________firstaff();\n");


}



void second_aff(char*name, int line){
	if (debug) printf("in ________seconaff();\n");


  aff_type *nouveau=malloc(sizeof(struct aff_type));
  nouveau->name_var=(char*)malloc(sizeof(char)*strlen(name)+1);
  strcpy(nouveau->name_var, name);
  nouveau->ligne_decl=line;
  nouveau->type_var=(char*)malloc(50*sizeof(char));
  strcpy(nouveau->type_var,type_exist(name));
  nouveau->idden=true;
  	aff_type* p= malloc(sizeof(struct aff_type));
    aff_type* k= malloc(sizeof(struct aff_type));
    p=notre_list->suivant;
    k=notre_list;
  	int prange;
    int o=0;
  	while(p!=NULL ){
      o=1;
  		prange=p->range;
      k=p;
  		p=p->suivant;
  	}
    if(o==0){
      nouveau->range=2;
      notre_list->suivant=nouveau;
    }else{
      nouveau->range=prange+1;
      k->suivant=nouveau;
      
      
    }
  	if (debug) printf("out ________seconaff();\n");
    
}



void const_aff(typetoken token){
	if (debug) printf("in ________constaff();\n");


  aff_type *nouveau=malloc(sizeof(struct aff_type));
  nouveau->name_var=(char*)malloc(sizeof(char)*50);
  strcpy(nouveau->name_var, "none");
  nouveau->ligne_decl=-1;
  nouveau->type_var=(char*)malloc(50*sizeof(char));
  nouveau->idden=false;
  if(token==FALSE){
        strcpy(nouveau->type_var,"boolean");
      }else if(token==TRUE){
        strcpy(nouveau->type_var,"boolean");
      }else if(token==INUMBER){
        strcpy(nouveau->type_var,"integer");
      }else if(token==FNUMBER){
        strcpy(nouveau->type_var,"float");
      }else if(token==CNUMBER){
        strcpy(nouveau->type_var,"character");
      }
        
    aff_type* p= malloc(sizeof(struct aff_type));
    aff_type* k= malloc(sizeof(struct aff_type));
    p=notre_list->suivant;
    k=notre_list;
    int prange;
    int o=0;
    while(p!=NULL ){
      o=1;
      prange=p->range;
      k=p;
      p=p->suivant;
    }
    if(o==0){
      nouveau->range=2;
      notre_list->suivant=nouveau;
    }else{
      nouveau->range=prange+1;
      k->suivant=nouveau;
      
      
    }
	

if (debug) printf("out ________constaff();\n");

}









void afficher_op(){
if (debug) printf("in ________afficher_op();\n");
	aff_type *p=malloc(sizeof(struct aff_type));
  p= notre_list;
  if(p!=NULL){
    
  while(p!=NULL){
    //printf("%s      %s       %d \n",p->name_var,p->type_var,p->ligne_decl);
    p=p->suivant;
  }
  /*printf("§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§\n");
  ver_aff();
  printf("§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§\n");
  ver_op();
  printf("§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§\n");
*/
  }

  //notre_list=NULL;
	if (debug) printf("out ________afficher_op();\n");
  
}







boolean ver_aff(boolean compa){
    if (debug) printf("in ________ver_aff();\n");
  if(notre_list!=NULL){
       aff_type *p=malloc(sizeof(struct aff_type));
  p= notre_list;
  int a=0;
  char *type_principal=malloc(sizeof(char)*50);
  while(p!=NULL & a==0){
    if(p->range==1){
      strcpy(type_principal,p->type_var);
      p=p->suivant;
    }
    else{
      if( (strcmp(type_principal,p->type_var)==0) || ( (strcmp(type_principal,"float")==0) & (strcmp(p->type_var,"integer")==0) )){
          p=p->suivant;

      }
      else{
        a=1;
      }
    }
  }
  
  if(p!=NULL){
    if(compa){
        set_error(incompatible_comp, notre_list->ligne_decl, notre_list->name_var);
    }else{
      set_error(incompatible_affectation, notre_list->ligne_decl, notre_list->name_var);
    }

    return false;
  }

  }

 return true;
  

  if (debug) printf("out ________ver_aff();\n");

 
}



void ver_op(){
  if (debug) printf("in ________ver_op();\n");
if(notre_list!=NULL){

  aff_type *p=malloc(sizeof(struct aff_type));
  p= notre_list->suivant;
  if(p->suivant==NULL){
   
  }else{
    while(p!=NULL){
      if(p->idden){
        if(strcmp(p->type_var,"boolean")||strcmp(p->type_var,"charachter")){
          set_error(incompatible_operation, p->ligne_decl, p->name_var);
        }
      }
      p=p->suivant;
      
    }
  }








notre_list=NULL;
}
  






  if (debug) printf("out ________ver_op();\n");
}






boolean ver_op_san(){
  if (debug) printf("in ________ver_op();\n");
 
if(notre_list!=NULL){

  aff_type *p=malloc(sizeof(struct aff_type));
  p= notre_list->suivant;
  if(p->suivant==NULL){
    printf("acuuuuuuuuuuuuuuuuuuuuun operation aslan\n");

  }else{
    while(p!=NULL){
      if(p->idden){
        if(strcmp(p->type_var,"boolean")||strcmp(p->type_var,"charachter")){
          return false;
        }
      }
      p=p->suivant;
      
    }
  }








notre_list=NULL;
}
  


return true;



  if (debug) printf("out ________ver_op();\n");
}








