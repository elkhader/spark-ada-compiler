#include "analyseur_spark_v0.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<string.h>
#include"tabsym.h"
#include"error.h"
#include"op_co_aff.h"
#include "pseudocode.h"
#include "vm.h"


extern int yylex();
extern int yylineno;

varvalueType varattribute;
constvalueType constattribute;
typevalueType typeattribute;
instvalueType instattribute;
listinstvalueType listinstattribute;
tokenvalueType tokenattribute;


typetoken token;
typetoken token_val;
typetoken token_type;
boolean follow_token=false;
boolean af_begin=false;
boolean compa=false;
boolean auxtrue=false;

typetoken _lire_token(){
	if (follow_token){
		follow_token = false; 
		return token; 
	}else{
		return (typetoken) yylex();
	}
}



int main(int argc , char** argv ){

	listinstvalueType ** pplistinstattribute = (listinstvalueType **) malloc (sizeof(listinstvalueType *));
	*pplistinstattribute = NULL;
	token = _lire_token();
	
	if(_PROG(pplistinstattribute)){

		if (debug) printf("0 erreurs syntaxiques\n");
		if(nbrError()==0){
			if (debug) printf("0 erreurs sémantiques\n");
			if (debug)// afficher_var();
			
			if (debug) { 
			printf("Affichage du Control Flow Graph produit :\n"); afficher_list_inst(*pplistinstattribute);
			}
			if (debug) printf("Generation du code ...\n");			
			pseudocode pc = generer_pseudo_code(*pplistinstattribute);
			if (debug) printf("Affichage du pseudocode généré :\n");
			afficher_pseudo_code(pc);
		}
		else{
						printf("%d erreurs sémantiques\n", nbrError());
						afficher_error();

		}
		
	}
	else{
		printf("plusieurs erreurs syntaxiques - voir ligne %d\n", yylineno);
		
		
		printf("%d erreurs sémantiques\n", nbrError());
		if (nbrError()> 0) afficher_error();
		
	}
	return 0;
}





boolean _AUX(AST *past)//AUX : idf | inumber | fnumber | ( ADDSUB )
{

	boolean result;
	if (debug) printf("in ________________________AUX();\n");
	auxtrue=true;
	if(token==IDF){
		
		second_aff(current_var.name_var,current_var.ligne_decl);
		*past = creer_feuille_idf(current_var.name_var, typevar(current_var.name_var));
		result=true;
	}
	else if (token==INUMBER){

			const_aff(token);
			
				*past = creer_feuille_nombre(last_const, Int);
			
			
			result=true;
		 
	}
	else if(token==FNUMBER)
	{
		const_aff(token);
		
		*past = creer_feuille_nombre(last_const, Double);
	
		result=true;
	}
	else if(token==PAR_OUV){
		token = _lire_token();
		
		if(_ADDSUB(past)){
			token = _lire_token();
			
			if(token==PAR_FER){
				result=true;
			}
			else{ result= false;}
		}
		else{ result= false;}

	}
	else{ result= false;}
	auxtrue=false;
	if (debug) printf("out ________________________AUX();\n");
	return result;

}



boolean _MULTDIVAUX(AST *past)//MULTDIVAUX : * AUX MULTDIVAUX | / AUX MULTDIVAUX |epsilon
{
	boolean result;
	if (debug) printf("in ________________________MULTDIVAUX();\n");
	if(token== MINUS || token==PLUS || token==PVIRG || token== PAR_FER){
		follow_token = true;
		result=true;
	}
	else if(token==MULT ){
		token = _lire_token();
		*past = creer_noeud_operation('*', *past, NULL, type(*past));

		AST *past2 = (AST *) malloc(sizeof(AST));
		(*past2) = (AST) malloc (sizeof(struct Exp));

		
		if(_AUX(past2)){
			token = _lire_token();
			(*past)->noeud.op.expression_droite = *past2; 
			
			if(_MULTDIVAUX(past)){
				
				result=true;
			}
			else{ result= false;}
		}
		else{ result= false;}
	}
	
	else if(token==DIV ){
		token = _lire_token();
		*past = creer_noeud_operation('/', *past, NULL, type(*past));

		AST *past2 = (AST *) malloc(sizeof(AST));
		(*past2) = (AST) malloc (sizeof(struct Exp));

		if(_AUX(past2)){
			token = _lire_token();

			(*past)->noeud.op.expression_droite = *past2; 
			if((*past2)->typeexp==_IDF){
				if( value((*past)->noeud.op.expression_droite->noeud.idf)==0){
					set_error(8, yylineno, (*past)->noeud.op.expression_droite->noeud.idf);
					
				}
			}
			if((*past2)->typeexp==NB){
				if( (*past)->noeud.op.expression_droite->noeud.nombre==0){
					set_error(8, yylineno,"kk");
					
				}
			}
			
			

			
			if(_MULTDIVAUX(past)){
				
				result=true;
			}
			else{ result= false;}
		}
		else{ result= false;}
	}
	else{ result= false;}
	if (debug) printf("out ________________________MULTDIVAUX();\n");

	return result;
}


boolean _MULTDIV(AST *past)//MULTDIV : AUX MULTDIVAUX
{
	boolean result;
	if (debug) printf("in ________________________MULTDIV();\n");
	AST *past1 = (AST *) malloc(sizeof(AST));
	AST *past2 = (AST *) malloc(sizeof(AST));

	(*past1) = (AST) malloc (sizeof(struct Exp)); 
	if(_AUX(past)){
		
		token = _lire_token();
		
								
		if(_MULTDIVAUX(past)){

			if ((arbre_droit(*past) != NULL) && (arbre_gauche(*past) != NULL)) {
				if (type(arbre_gauche(*past)) == type(arbre_droit(*past))){ 
					(*past)->typename = type(arbre_gauche(*past));
				}else (*past)->typename = Double;
			}else {(*past) = *past;}
			
						
			result=true;
			

		}
		else{ result= false;}
	}	
	else{ 
		result= false;}
	if (debug) printf("out ________________________MULTDIV();\n");

	return result;
}


boolean _ADDSUBVAUX(AST *past)//ADDSUBAUX : – MULTDIV ADDSUBVAUX | + MULTDIV ADDSUBVAUX |epsilon
{
	boolean result;
		if (debug) printf("in ________________________ADDSUBVAUX();\n");

	if((token==PVIRG )|| (token== PAR_FER)){
		follow_token = true;
		result=true;
	}
	else if (token==PLUS)
	{
		
		token = _lire_token();
		*past = creer_noeud_operation('+', *past, NULL, type(*past));


		AST *past2 = (AST *) malloc(sizeof(AST));
		(*past2) = (AST) malloc (sizeof(struct Exp));
		
		if(_MULTDIV(past2))
		{
						

			token = _lire_token();

				(*past)->noeud.op.expression_droite = *past2; 
			
			if(_ADDSUBVAUX(past))
			{
				result=true;
			}
			else{ result= false;}
		}
		else{ result= false;}
	}
	else if (token==MINUS )
	{
		
		token = _lire_token();
		*past = creer_noeud_operation('-', *past, NULL, type(*past));

		AST *past2 = (AST *) malloc(sizeof(AST));
		(*past2) = (AST) malloc (sizeof(struct Exp));
		
		if(_MULTDIV(past2))
		{
						

			token = _lire_token();
			(*past)->noeud.op.expression_droite = *past2; 
			if(_ADDSUBVAUX(past))
			{
				result=true;
			}
			else{ result= false;}
		}
		else{ result= false;}
	}
	else{ result= false;}
	
						
	if (debug) printf("out ________________________ADDSUBVAUX();\n");
	return result;
}



boolean _ADDSUB(AST *past)//ADDSUB : MULTDIV ADDSUBAUX
{
	boolean result;
	if (debug) printf("in ________________________ADDSUB();\n");
	AST *past1 = (AST *) malloc(sizeof(AST));
	(*past1) = (AST) malloc (sizeof(struct Exp));
	if(_MULTDIV(past)){
		
		token = _lire_token();
		
		/*printf("______________________________________________________________________\n");
						afficher_infixe_arbre(*past) ;
						printf("_____________________________________________________________\n");*/
		if(_ADDSUBVAUX(past)){

			if ((arbre_droit(*past) != NULL) && (arbre_gauche(*past) != NULL)) {
				if (type(arbre_gauche(*past)) == type(arbre_droit(*past))){ 
					(*past)->typename = type(arbre_gauche(*past));
				}else (*past)->typename = Double;
			}else {(*past) = *past;}

			result=true;
			afficher_op();
		}
		else{ result= false;}
	}	
	else{ result= false;}
	if (debug) printf("out ________________________ADDSUB();\n");

	return result;
}

boolean _IF_INSTAUX(listinstvalueType ** pplistinstattribute){//IF_INSTAUX : end if ‘;’ | else  LISTE_INST end if ; 
boolean result;

if (debug) printf("in _________________________IF_INSTAUX();\n");

	*pplistinstattribute=NULL;
	if(token==END1){
		token = _lire_token();
		if(token==IF){
			token = _lire_token();
			if(token==PVIRG)
			{
				result=true;
			}
			else{ result= false;}
		}
		else{ result= false;}
	}
	else if(token==ELSE){
		token = _lire_token();
		
		if(_LISTE_INST(pplistinstattribute)){
			token = _lire_token();
			
			if(token==END1){
				token = _lire_token();
				if(token==IF){
					token = _lire_token();
					if(token==PVIRG)
					{
						result=true;
					}
					else{ result= false;}
				}
				else{ result= false;}
			}
			else{ result= false;}
	
		}
		else{ result= false;}
	}

	else{ result= false;}
	if (debug) printf("out _________________________IF_INSTAUX();\n");

	return result;

}



boolean _INST(instvalueType ** ppinstattribute)/*INST : idf = ADDSUB ';' | true ';' | false ';'  | cnumber ';' 
						| put ‘(‘ idf ’)’ ‘ ;’ 
						| for IDF in  inumber ‘ ..’ inumber loop  LISTE_INST end loop ‘ ;’ 
						| if ‘(‘ idf = ADDSUB ‘)’ then LISTE_INST IF_INSTAUX */
{
	boolean result;
	if (debug) printf("in ________________________INST();\n");
	AST *past = (AST *) malloc(sizeof(AST)); // NEW
	(*past) = (AST) malloc(sizeof(struct Exp));


	listinstvalueType ** pplistthen = (listinstvalueType **) malloc (sizeof(listinstvalueType *));
	listinstvalueType ** pplistelse = (listinstvalueType **) malloc (sizeof(listinstvalueType *));
	listinstvalueType ** pplistfor = (listinstvalueType **) malloc (sizeof(listinstvalueType *));
	*pplistthen = NULL;
	*pplistelse = NULL;
	*pplistfor = NULL;
	int localrangvar; constvalueType borneinfconstattribute, bornesupconstattribute, localconstattribute; varvalueType localvarattribute;
	int born_sup, born_inf;
	* ppinstattribute = NULL;
	////////////////////idf := ADDSUB ';' | true ';' | false ';'  | cnumber ';' 
	if(token==IDF){
		first_aff(current_var.name_var,current_var.ligne_decl);
		
		token = _lire_token();
		
		if(token==AFFECT){
			token = _lire_token();
				
				if(_ADDSUB(past)){
					token = _lire_token();
					boolean i =ver_aff(compa);
					ver_op();

					if(token==PVIRG){
						
						if(nbrError()==0){
				
				*ppinstattribute = creer_instruction_affectation(rangvar_name(current_var.name_var), past);
			}
			
					
						result=true;
						
					}
					else{ result= false;}
				}
				else if(token == TRUE){
					const_aff(token);
						boolean i =ver_aff(compa);
						*past = creer_feuille_booleen(true);
						*ppinstattribute = creer_instruction_affectation(rangvar_name(current_var.name_var), past);
					
					token = _lire_token();

					
					if(token==PVIRG){
						
						
						result=true;
						
					}
					else{ result= false;}
				}
				else if (token == FALSE){
					const_aff(token);
					boolean i =ver_aff(compa);
					*past = creer_feuille_booleen(false );
					*ppinstattribute = creer_instruction_affectation(rangvar_name(current_var.name_var), past);
				
					token = _lire_token();

					
					if(token==PVIRG){
					
						result=true;
						
					}
					else{ result= false;}
				}
				else if (token == CNUMBER){
					const_aff(token);
					token = _lire_token();

					
					if(token==PVIRG){
						
						result=true;

						
					}
					else{ result= false;}
				}

				else{ result= false;}


		}
		else{ result= false;}
	}
	else if(token==NEW_LINE){////////////| New_line ; 

		token = _lire_token();
		if(token==PVIRG){
			result=true;
		}
		else{ result= false;}

	}
	////////////////////////| put ‘(‘ idf ’)’ ‘ ;’ | put_line ‘(‘ idf ’)’ ‘ ;’
	else if (token==PUT|| token==PUT_LINE)
	{

		token = _lire_token();
		if(token==PAR_OUV){

			token =_lire_token();
			if(token==IDF){

				token =_lire_token();
				if(token==PAR_FER){

					token =_lire_token();
					if(token==PVIRG){
						if (nbrError()==0) {
							*ppinstattribute = creer_instruction_print(rangvar_name(current_var.name_var));
						}
						result=true;
					}
					else{ result= false;}
				}
				else{ result= false;}
				
			}
			else{ result= false;}
		}
		else{ result= false;}

	}
	////////////////for IDF in  inumber ‘ ..’ inumber loop  LISTE_INST end loop ‘ ;’
	else if(token==FOR){
		token = _lire_token();
		
		if(token==IDF){
			localrangvar=rangvar_name(current_var.name_var); 

			if(strcmp(type_exist(current_var.name_var),"boolean")==0) {

				set_error(incompatible_for_index_type, current_var.ligne_decl, current_var.name_var);
			}
			token = _lire_token();
			if(token==IN){

				token = _lire_token();
				if(token==INUMBER){
					born_inf =(int) last_const;

					token = _lire_token();
					if(token==UNTIL){

						token = _lire_token();
						if(token==INUMBER){
							born_sup = (int)last_const;
							token = _lire_token();
							if(token==LOOP){

								token = _lire_token();
								if(_LISTE_INST(pplistfor)){

									token = _lire_token();
									if(token==END1){

										token = _lire_token();
										if(token==LOOP){

											token = _lire_token();
											if(token==PVIRG){
												result=true;
												if(nbrError()==0){
													*ppinstattribute = creer_instruction_for(localrangvar,born_inf, born_sup,*pplistfor);

												}
											}
											else{ result= false;}
										}
										else{ result= false;}
									}
									else{ result= false;}
								}
								else{ result= false;}
							}
							else{ result= false;}
						}
						else{ result= false;}

					}
					else{ result= false;}
				}
				else{ result= false;}
			}
			else{ result= false;}
		}
		else{ result= false;}

	}
	////////////////////if ‘(‘ idf == ADDSUB |‘)’ then LISTE_INST IF_INSTAUX
	else if(token==IF){
		compa=true;
		token = _lire_token();
		if(token==PAR_OUV){

			token = _lire_token();
			if(token==IDF){
				first_aff(current_var.name_var,current_var.ligne_decl);
				localrangvar=rangvar_name(current_var.name_var);
				localvarattribute=current_var;
				token = _lire_token();
				if(token==EGALITE){

						token = _lire_token();
						if(_ADDSUB(past) || (token == TRUE )|| (token ==FALSE) ||  (token== CNUMBER)){
								if((token == TRUE )|| (token ==FALSE) ||  (token== CNUMBER)){
									const_aff(token);
								}
							token = _lire_token();
							if(token==PAR_FER){
								ver_aff(compa);
								ver_op();
								token = _lire_token();
								if(token ==THEN){
									compa=false;
									token = _lire_token();
									if(_LISTE_INST(pplistthen)){
							
										token = _lire_token();
										if(_IF_INSTAUX(pplistelse)){
											if(nbrError()==0){
												*ppinstattribute = creer_instruction_if(localrangvar, past, *pplistthen, *pplistelse);
											}
											
											result=true;
	
										}else{ result= false;}
	
									}else{ result= false;}
	
								}else{ result= false;}
	
							}else{ result= false;}
	
						}else{ result= false;}
				}else{ result= false;}

				

			}else{ result= false;}

		}else{ result= false;}

	}
	else{ result= false;}
	if (debug) printf("out________________________INST();\n");
	return result;
}




boolean _CONST(){//CONST : inumber | fnumber | cnumber |  true | false 
	boolean result;
	if (debug) printf("in________________________CONST();\n");
	if(token==INUMBER){
		result = true;}
	
	else if (token==FNUMBER){
		result=true;
	}
	else if(token==CNUMBER){
		result=true;
	}
	else if(token==TRUE){
		result=true;
	}
	else if(token==FALSE){
		result=true;
	}
	else{ result= false;}
if (debug) printf("out________________________CONST();\n");
	return result;



}





boolean _TYPE(){//TYPE : integer | boolean | float | character ;
	boolean result;
	if (debug) printf("in ________________________TYPE();\n");
	
	if(token==INTEGER){
		result = true;
	}else if (token==FLOAT){
		result=true;
	}else if(token==BOOLEAN){
		result=true;
	}else if (token==CHARACTER){
		result=true;
	}else{ result= false;}
	
	if (debug) printf("out ________________________TYPE();\n");
	return result;
}




boolean _DECL_AUX(){//DECL_AUX : ‘ :=’ CONST ';' | ';' 
	boolean result;
	if (debug) printf("in ________________________DECL_AUX();\n");
	if(token==PVIRG){
		current_var.intial=false;
		
		
		result = true;}
	else if (token==AFFECT){
		
		token = _lire_token();
		if(_CONST()){
			token_val=token;


			token = _lire_token();
			if(token==PVIRG){

				current_var.intial=true;
			

				result=true;
			}
			else{ result= false;}
		}
		else{ result= false;}

	}else{ result= false;}
	if(result){
		set_val_initial();
		ajouter_var(current_var,mal_init_sc());
	}
	if (debug) printf("out ________________________DECL_AUX();\n");
	return result;
}




boolean _DECL(){//DECL : idf ‘ : ’ TYPE DECL_AUX 
	boolean result;
	if (debug) printf("in ________________________DECL();\n");
	if(token==IDF){

		token = _lire_token();
		if(token==DEUX_P){

			token = _lire_token();
			if(_TYPE())
			{
				token_type =token;
				token = _lire_token();
				
				if (_DECL_AUX())
				{
					result=true;
				} 
				else{ result= false;}
			}
			else{ result= false;}

		}
		else{ result= false;}

	}else{
		result= false;
	}
	
	if (debug) printf("out ________________________DECL();\n");
return result;

}

boolean _LISTE_DECLAUX(){//LISTE_DECLAUX : LISTE_DECL | epsilon 
	boolean result;
	if (debug) printf("in________________________LISTE_DECLAUX();\n");
	if(token==BEGIN1){
		follow_token = true;
		result=true;
	}
	else if (_LISTE_DECL()){
		
			result=true;
		
	}
	
	else{ result= false;}
	if (debug) printf("out________________________LISTE_DECLAUX();\n");
return result;

}


boolean _LISTE_DECL()//LISTE_DECL : DECL LISTE_DECLAUX
{
	boolean result;
	if (debug) printf("in________________________LISTE_DECL();\n");
	
		if (_DECL()){
			token = _lire_token();
			
	
			if( _LISTE_DECLAUX()){
				result=true;
			}
	
			else{ result= false;}
		}
		else{ result= false;}
	
	if (debug) printf("out________________________LISTE_DECL();\n");
return result;
}


boolean _LISTE_INSTAUX(listinstvalueType ** pplistinstattribute)//LISTE_INSTAUX : LISTE_INST | epsilon
{
	boolean result;

	if (debug) printf("in ________________________LISTE_INSTAUX();\n");
	if( (token == END1) || (token == ELSE)){
		
		follow_token = true;
		* pplistinstattribute = NULL;
		result=true;
	}
	else if (_LISTE_INST(pplistinstattribute)){
		
			result=true;
		
	}
	
	else{ result= false;}

	if (debug) printf("out________________________LISTE_INSTAUX();\n");
	return result;


}



boolean _LISTE_INST(listinstvalueType ** pplistinstattribute)//LISTE_INST : INST LISTE_INSTAUX
{

	boolean result;
	if (debug) printf("in _________________________LISTE_INST()();\n");
		instvalueType ** ppinstattribute = (instvalueType **) malloc(sizeof(instvalueType *));
		*ppinstattribute = NULL;
		if (_INST(ppinstattribute)){
			
			token = _lire_token();

	
			if( _LISTE_INSTAUX(pplistinstattribute)){
				if(nbrError()==0){
					inserer_inst_en_tete(pplistinstattribute, **ppinstattribute);
				}
				result=true;
			}
	
			else{ result= false;}
		}
		else{ result= false;}
	
	if (debug) printf("out_________________________LISTE_INST()();\n");
return result;
}



boolean _PROG(listinstvalueType ** pplistinstattribute)//PROG : procedure  idf  is  LISTE_DECL begin LISTE_INST end idf ;
{

	boolean result;
	if (debug) printf("in ________________________PROG();\n");
	if(token==PROCEDURE){
		token=_lire_token();          
		if(token==IDF){
			token=_lire_token();
			if(token==IS){
				decl_start=true;
				token=_lire_token();
				if(_LISTE_DECL()){
					token = _lire_token();
					
					if(token==BEGIN1)
					{
						decl_start=false;
						af_begin=true;
						token = _lire_token();
						
						if(_LISTE_INST(pplistinstattribute)){
							token = _lire_token();
							
							if(token==END1){
								af_begin=false;
								token=_lire_token();
								if(token==IDF){
									token=_lire_token();
									if (token==PVIRG)
									{
										result=true;
									}
									else{ result=false;}
								}
								else{ result= false;}
							}
							else{ result= false;}
						}
						else{ result= false;}
							
						
					}
					else{ result= false;}
						
			
				}else{
					result= false;
				}
			}else{
				result=false;
			}
		}else{	
			result=false;
	    }
	}else{
		result= false;
	}

if (debug) printf("out ________________________PROG();\n");
return result;
}













void set_name_line(char* name, int line ){

	if (debug) printf("in ________________________set_name_line( );\n");
	if(decl_start ){

  			current_var.name_var=(char*)malloc(sizeof(char)*strlen(name)+1);
  			strcpy(current_var.name_var, name);
  			current_var.ligne_decl=line;
 	 }
 	else{
 		if(af_begin){
  			if(!var_exist(name)){
  				set_error(non_declaree, line,  name);
  			}else{
  				current_var.name_var=(char*)malloc(sizeof(char)*strlen(name)+1);
  				strcpy(current_var.name_var, name);
  				current_var.ligne_decl=line;
  			}
  			
 		}
 	}



  if (debug) printf("out ________________________set_name_line( );\n");
}

void set_type(char* type){
	if (debug) printf("in ________________________set_type();\n");
  current_var.type_var=(char*)malloc(sizeof(char)*strlen(type)+1);
  strcpy(current_var.type_var, type);
  
  if (debug) printf("out ________________________set_type();\n");


  
}


void set_const(double cons ){
if (debug) printf("in ________________________set_const();\n");
  last_const =cons;
 if (debug) printf("out ________________________set_const( );\n");
}


void set_val_initial(){
if (debug) printf("in ________________________set_val_initial();\n");
  if(current_var.intial){
    current_var.val_initial=last_const;

  }
  else
    current_var.val_initial=0; 

/*  to show our  current var
printf("\n\n\n\ncurrent var ____________________________________________________________________________________\n");
printf("name:---------%s\ntype-----------%s,inti------------%d,\n val_initial----------%f\n line--------%d/n_______________________________________________________________________________\n\n\n\n",
	current_var.name_var,current_var.type_var,current_var.intial,current_var.val_initial,current_var.ligne_decl);
*/


if (debug) printf("out ________________________set_val_initial();\n");
}



boolean mal_init_sc(){
	boolean mal_int;
	if (debug) printf("in ________________________mal_init_sc();\n");
	if(token_type==FLOAT &&(token_val==FNUMBER || token_val==INUMBER  ) )
	{
		mal_int = false;
	}
	else if(token_val==INUMBER && token_type == INTEGER)
	{
		mal_int = false;
	}
	else if(token_type==BOOLEAN &&(token_val==TRUE || token_val ==FALSE))
	{
		mal_int = false;
	}
	else
		
		mal_int = true;
	if (debug) printf("out ________________________mal_init_sc();\n");
	return mal_int;

}




/*flex -i scanner_v3.l

gcc -c lex.yy.c
gcc -c analyseur_zz_v3.c
gcc -o zzc analyseur_zz_v3.o lex.yy.o -ll*/



