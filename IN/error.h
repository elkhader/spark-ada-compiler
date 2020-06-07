#ifndef ERROR_H
#define ERROR_H





typedef enum{
	var_mal_initialisee=1, var_deja_declaree=2, non_declaree=3, incompatible_affectation =4, incompatible_operation=5,incompatible_comp=6, incompatible_for_index_type=7,division_zero=8
}semantic_error_type;

 
 

typedef struct 
{
	int num_ligne;
	char* nom_source_error;
	semantic_error_type type_error;
}sm_error;

static sm_error current_error ;
static  int nbr_error =0;

int nbrError();

void afficher_sm_erreur(semantic_error_type et, int line, char* name);
void ajouter_error(sm_error nouv_error);
void set_error(semantic_error_type et, int line, char* name);
void afficher_error();

void creer_sm_erreur( semantic_error_type et, int line_error, char* nom_source_error );


#endif