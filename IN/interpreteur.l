%{
// auteur Karim Baïna, ENSIAS, Décembre 2010
#include "interpreteur.h"
#include <string.h>
%}
%option noyywrap
%option yylineno

%%
"add"		 	{if (debug) printf ("ADD"); return ADDOP;}
"div" 			{if (debug) printf ("DIV"); return DIVOP;}
"dupl" 			{if (debug) printf ("DUPL"); return DUPLOP;}
"jmp" 			{if (debug) printf ("JMP"); return JMPOP;}
"jne" 			{if (debug) printf ("JNE"); return JNEOP;}
"jg" 			{if (debug) printf ("JG"); return JGOP;}
"load" 			{if (debug) printf ("LOAD"); return LOADOP;}
"mult" 			{if (debug) printf ("MULT"); return MULTOP ;}
"print"			{if (debug) printf ("PRINT"); return PRINTOP;}
"push" 			{if (debug) printf ("PUSH"); return PUSHOP;}
"sub" 			{if (debug) printf ("SUB"); return SUBOP;}
"store"			{if (debug) printf ("STORE"); return STOREOP;}
"swap" 			{if (debug) printf ("SWAP"); return SWAPOP;}
"begin:"		{
			if (debug) printf ("BEGIN LABEL"); 
			set_label_attributes("begin", yylineno);
			return BEGINOP;
			}
"end:"			{
			if (debug) printf ("END LABEL"); 
			set_label_attributes("end", yylineno);
			return ENDOP;
			}
[a-zA-Z_][a-zA-Z0-9_]*":" {
			if (debug) printf ("LABEL"); 
			int i = strlen(yytext);
			char * label_name = (char*) malloc(i);
			i--;
			label_name[i]='\0';
			while(--i >= 0) label_name[i] = yytext[i]; // le yytext sans le ':'
			set_label_attributes(label_name, yylineno);
			return LABELOP;
			}
[a-zA-Z_][a-zA-Z0-9_]* 	{
			if (debug) printf ("IDF(%s)",yytext);
			set_idf_attributes(yytext, yylineno);
			return IDFOP;
			}
[0-9]+[.][0-9]+		{
			if (debug) printf ("NUMBER(%s)",yytext);
			set_number_attributes(atof(yytext));
			return DNUMBEROP;
			}
[0-9]+			{
			if (debug) printf ("NUMBER(%s)",yytext);
			set_number_attributes((double)atoi(yytext));
			return INUMBEROP;
			}
[R][E][M][^\n]*[\n]     /* commentaires */
[ \t\n]		        /* caractère vide */
.			{set_token_attributes(yylineno);}
%%
