#include <stdio.h>
#include <string.h>
#include "interpreteur.h"
#include "pseudocode.h"
#include "vm.h"

#include <stdlib.h>


boolean _pseudocode(pseudocode * ppc);

boolean _liste_inst_pc(pseudocode * ppc);
boolean _liste_inst_aux_pc(pseudocode * ppc);
boolean _inst_pc(struct pseudoinstruction ** ppi);

boolean _data(pseudocode * ppc);
boolean _data_aux(pseudocode * ppc);
boolean _data_item(struct pseudoinstruction ** ppi);
boolean _const();

typetokenOp _lire_token();

extern int yylex();

typetokenOp token; 
boolean follow_token = false;

varvalueType labelattribute;
varvalueType varattribute;
constvalueType constattribute;

tokenvalueType tokenattribute;

int rangvar;
boolean semanticerror = false;

int main(int argc, char ** argv){
	pseudocode * ppc_element_fictif = (pseudocode *) malloc (sizeof(pseudocode));
	pseudocode * ppc = (pseudocode *) malloc (sizeof(pseudocode));

	*ppc_element_fictif = (pseudocode) malloc (sizeof(struct pseudocodenode)); 

	if (debug) printf("Chargement du Pseudo Code :\n");

	token = _lire_token();
	if (_pseudocode(ppc_element_fictif) == true) {
		if (debug) printf("0 erreurs syntaxiques\n");

		//if (debug) afficher_var();
			
		*ppc = (*ppc_element_fictif)->next;

		if (debug) { printf("Affichage du Pseudo Code chargé :\n"); afficher_pseudo_code(*ppc);}
		// élimination de l'élément fictif (sentinelle)		

		if (debug) printf("Initialisation de la machine virtuelle :\n");
		inintialiser_machine_abstraite();

		if (debug) printf("Interprétation du peudocode :\n");
		interpreter_pseudo_code(*ppc);
	}else {
		printf("plusieurs erreurs syntaxiques\n");
		/* printf("%d erreurs syntaxiques\n", nombre_sx_erreurs());
		afficher_sx_erreurs(); */
	}
	
return 0;
}
// PSEUDOCODE : DATA BEGINOP LISTE_INST_PSEUDOCODE ENDOP
boolean _pseudocode(pseudocode * ppc){
	boolean result;
	if (debug) printf("prog()\n");

	if (_data(ppc)) {
		token = _lire_token();
		if (token == BEGINOP){

			pseudocode pcbegin = (pseudocode) malloc(sizeof(struct pseudocodenode));
			pcbegin->first.codop = LABEL;
			pcbegin->first.param.label_name = (char*) malloc(6);
			strcpy(pcbegin->first.param.label_name, "begin");
			pcbegin->next = NULL;

			inserer_code_en_queue(*ppc, pcbegin);

			token = _lire_token();
			if (_liste_inst_pc(ppc)) {
				token = _lire_token();
				if (token == ENDOP){
				
					pseudocode pcend = (pseudocode) malloc(sizeof(struct pseudocodenode));
					pcend->first.codop = LABEL;
					pcend->first.param.label_name = (char*) malloc(4);
					strcpy(pcend->first.param.label_name, "end");
					pcend->next = NULL;					

					inserer_code_en_queue(*ppc,pcend);

					result = true;
				} else {
					result = false;
				}
			}else result = false;
		}else{			
			// creer_sx_erreur(BeginExpected, tokenattribute.line);
			// déjà traité dans le nullable liste_decl !!
			
			result = false;
		}
	}else result = false;
	
	if (debug) printf("out of prog()\n");
	return result;
}

//  LISTE_INST_PSEUDOCODE : INST_PSEUDOCODE LISTE_INST_AUX_PSEUDOCODE
boolean _liste_inst_pc(pseudocode * ppc){
	boolean result;
	if (debug) printf("list_inst()\n");

	struct pseudoinstruction ** ppi = (struct pseudoinstruction **) malloc (sizeof(struct pseudoinstruction *));
	pseudocode * ppc_aux = (pseudocode *) malloc (sizeof(pseudocode));

	*ppi = NULL;
	(*ppc_aux) = (pseudocode) malloc (sizeof(struct pseudocodenode));

	if (_inst_pc(ppi)) {
		(*ppc_aux)->first = **ppi;
		(*ppc_aux)->next = NULL;
		token = _lire_token();
		if (_liste_inst_aux_pc(ppc_aux) == true){
			inserer_code_en_queue(*ppc, *ppc_aux);
			result = true;
		}else result = false;
	}else result = false;

	if (debug) printf("out of list_inst()\n");
	return result;
}

// LISTE_INST_AUX_PSEUDOCODE : LISTE_INST_PSEUDOCODE | epsilon
// NULLABLE(LISTE_INST_AUX_PSEUDOCODE) = true
// follow(LISTE_INST_AUX_PSEUDOCODE) = {ENDOP}
// first(LISTE_INST_AUX_PSEUDOCODE) = first(LISTE_INST_PSEUDOCODE)
boolean _liste_inst_aux_pc(pseudocode * ppc){
        boolean result;
	if (debug) printf("list_inst_aux()\n");

	if (token == ENDOP){
		// _yyless("\tend\n");
		follow_token = true;
		result = true;
	} else if (_liste_inst_pc(ppc) == true){
		result = true;
	} else result = false;

	if (debug) printf("out of list_inst_aux()\n");	
	return result;
}

/*   INST_PSEUDOCODE :
	ADDOP 		|
	DIVOP		|
	DUPLOP		|
	LABELOP	        |
	MULTOP		|
	PRINTOP		|
	SUBOP		|
	SWAPOP          |
	STOREOP	IDFOP	|
	JMPOP IDFOP	|
	JNEOP IDFOP	|
	JGOP IDFOP	|
	LOADOP IDFOP	|
	PUSHOP CONST 	
*/
boolean _inst_pc(struct pseudoinstruction ** ppi){
	boolean result;
	if (debug) printf("inst()\n");

	// ppi est déjà allouée, mais pas *ppi
	(*ppi) = (struct pseudoinstruction *) malloc(sizeof (struct pseudoinstruction));

	if (token == ADDOP){		
		(*ppi)->codop = ADD;
		result = true;
	}else if (token == DIVOP){		
		(*ppi)->codop = _DIV;
		result = true;
	}else if (token == DUPLOP){		
		(*ppi)->codop = DUPL;
		result = true;
	}else if (token == LABELOP){		
		(*ppi)->codop = LABEL;
		(*ppi)->param.label_name = (char*) malloc(strlen(labelattribute.name_var)+1);
		strcpy((*ppi)->param.label_name, labelattribute.name_var);
		result = true;
	}else if (token == MULTOP){		
		(*ppi)->codop = _MULT;
		result = true;
	}else if (token == PRINTOP){		
		(*ppi)->codop = PRNT;
		result = true;
	}else if (token == SUBOP){		
		(*ppi)->codop = SUB;
		result = true;
	}else if (token == SWAPOP){		
		(*ppi)->codop = SWAP;
		result = true;
        }else if (token == STOREOP){
		(*ppi)->codop = STORE;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.var = (char*) malloc(sizeof(char) * strlen(varattribute.name_var)+1);
			strcpy((*ppi)->param.var, varattribute.name_var);
			result = true;
		}else result = false;
	}else if (token == LOADOP){
		(*ppi)->codop = LOAD;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.var = (char*) malloc(sizeof(char) * strlen(varattribute.name_var)+1);
			strcpy((*ppi)->param.var, varattribute.name_var);
			result = true;
		}else result = false;
	}else if (token == JMPOP){
		(*ppi)->codop = JMP;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.label_name = (char*) malloc(sizeof(char) * strlen(varattribute.name_var)+1);
			strcpy((*ppi)->param.label_name, varattribute.name_var);
			result = true;
		}else result = false;
	}else if (token == JNEOP){
		(*ppi)->codop = JNE;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.label_name = (char*) malloc(sizeof(char) * strlen(varattribute.name_var)+1);
			strcpy((*ppi)->param.label_name, varattribute.name_var);
			result = true;
		}else result = false;
	}else if (token == JGOP){
		(*ppi)->codop = JG;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.label_name = (char*) malloc(sizeof(char) * strlen(varattribute.name_var)+1);
			strcpy((*ppi)->param.label_name, varattribute.name_var);
			result = true;
		}else result = false;		
	}else if (token == PUSHOP){
		(*ppi)->codop = PUSH;
		token = _lire_token();
		if (_const()) {
			(*ppi)->param._const = constattribute.val_initial;
			result = true;
		}else result = false;
		//////////////////////result = true;
	}else result = false;

return result;
}

// DATA : DATA_ITEM DATA_AUX 
boolean _data(pseudocode * ppc){
	boolean result;
	struct pseudoinstruction ** ppi = (struct pseudoinstruction **) malloc(sizeof(struct pseudoinstruction *));
	pseudocode * ppc_aux = (pseudocode *) malloc (sizeof(pseudocode));

	*ppi = NULL;

	(*ppc_aux) = (pseudocode) malloc (sizeof(struct pseudocodenode));

	if (debug) printf("liste_decl()\n");

	if (_data_item(ppi)){
		(*ppc_aux)->first = **ppi;
		(*ppc_aux)->next = NULL;
		token = _lire_token();
		if (_data_aux(ppc_aux)){
			inserer_code_en_queue(*ppc, *ppc_aux);
			result = true;
		}else result = false;
	}else result = false;

	if (debug) printf("out of liste_decl()\n");
	return result;
}

// DATA_AUX : DATA | epsilon
// NULLABLE(DATA_AUX) = true
// follow(DATA_AUX) = {beginop}
// first(DATA_AUX) = first(DATA_AUX) = {idf}
boolean _data_aux(pseudocode * ppc){
	boolean result;
	if (debug) printf("liste_decl_aux()\n");

	if ( (token == BEGINOP) ){
		follow_token = true;
		result = true;
	}else if (_data(ppc)){
		result = true;
	}else{
		/* printf("TOKE = %d\n",token);
		creer_sx_erreur(BeginExpected, tokenattribute.line); */
		result = false;
	}

	if (debug) printf("out of liste_decl_aux()\n");
	return result;
}

// DATA_ITEM : idfop CONST
boolean _data_item(struct pseudoinstruction ** ppi) {
	boolean result;

	// ppi  est alloué mais pas *ppi
	*ppi = (struct pseudoinstruction *) malloc(sizeof(struct pseudoinstruction));

	if (debug) printf("decl()\n");

	if ( (token == IDFOP) ) {
		(*ppi)->codop = DATA;

		(*ppi)->param.nv.name = (char*) malloc(sizeof(char) * strlen(varattribute.name_var)+1);
		strcpy((*ppi)->param.nv.name, varattribute.name_var);

		token = _lire_token();
		if (_const() == true){
			(*ppi)->param.nv.value = constattribute.val_initial;
			result = true;
		}else result = false;
	}else result = false;

	if (debug) printf("out of decl()\n");
	return result;
}

// CONST : inumberop | dnumberop
boolean _const() {
	boolean result;
	if (debug) printf("const()\n");

	if (token == INUMBEROP) {
		result = true;	
	}else if (token == DNUMBEROP) {
		result = true;
	} else result = false;

	if (debug) printf("out of const()\n");
	return result;
}


typetokenOp _lire_token(){
	if (follow_token){
		follow_token = false;
		return token;
	}else{ 
		return (typetokenOp) yylex();
	}
}

void set_label_attributes(char *name, int line){
	labelattribute.name_var = (char *)malloc(sizeof(char) * strlen(name)+1);
	strcpy(labelattribute.name_var, name);
	labelattribute.ligne_decl = line;
}

void set_idf_attributes(char *name, int line){
	varattribute.name_var = (char *)malloc(sizeof(char) * strlen(name)+1);
	strcpy(varattribute.name_var, name);
	varattribute.ligne_decl = line;
}

void set_number_attributes(double value){
	constattribute.val_initial = value;
}

void set_token_attributes(int line){
	tokenattribute.ligne_decl = line;
}
