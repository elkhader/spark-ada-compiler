 #ifndef ANALYSEUR_spark_H
#define ANALYSEUR_spark_H

#include"tabsym.h"
#include"error.h"


typedef enum{
	   INTEGER, BOOLEAN, TRUE, FALSE, PVIRG, BEGIN1, AFFECT, PLUS, MINUS, MULT, DIV, PAR_OUV, PAR_FER, ELSE, END1, IF, THEN,FOR,LOOP, EGALITE,DIFF,UNTIL,NEW_LINE,PUT_LINE,PUT,IN,CHARACTER,FLOAT, TO,ELSIF,DEUX_P,CNUMBER,PROCEDURE,IS,IDF,INUMBER,FNUMBER}typetoken;



typetoken _lire_token();



boolean _CONST();
boolean _DECL_AUX();
boolean _TYPE();
boolean _DECL();
boolean _LISTE_DECL();
boolean _LISTE_DECLAUX();
boolean _PROG(listinstvalueType ** pplistinstattribute);
boolean _AUX(AST *past);
boolean _MULTDIVAUX(AST *past);
boolean _MULTDIV(AST *past);
boolean _ADDSUBAUX(AST *past);
boolean _ADDSUB(AST *past);
boolean _IF_INSTAUX(listinstvalueType ** pplistinstattribute);
boolean _INST(instvalueType ** ppinstattribute);
boolean _LISTE_INST(listinstvalueType ** pplistinstattribute);
boolean _LISTE_INSTAUX(listinstvalueType ** pplistinstattribute);
 



#endif