%{
	#include <math.h>
	#include"analyseur_spark_v0.h"
	

%}
%option noyywrap
%option yylineno

IDF 	[A-Za-z][A-Za-z0-9_]*

%%
[ \t\n]+		{;}
"--"[^\n]*  	{;}
"procedure"		{  return PROCEDURE;}
"is"			{  return IS;}
"begin"			{  return BEGIN1;}
"end"			{ return END1;}
\;				{  return PVIRG;}	
"integer"		{  set_type(yytext) ;return INTEGER;}
"boolean"		{  set_type(yytext) ;return BOOLEAN;}
"true"			{ set_const(1); return TRUE;}	
"false"			{set_const(0);; return FALSE;}	
"float"         {set_type(yytext) ; return FLOAT;}
"character"     { set_type(yytext) ; return CHARACTER;}
"if"			{ return IF;}
"then"			{ return THEN;}
"for"			{ return FOR;}
"in"        	{ return IN;}
"loop"			{return LOOP;}
"put"			{ return PUT;}
"put_line"		{ return TO;}
"new_line"		{ return NEW_LINE;}
"else"			{ return ELSE;}
\'[a-z0-9A-Z]\' { return CNUMBER;}
{IDF}			{set_name_line(yytext,yylineno); return IDF;}
".."			{ return UNTIL;}	
":="			{ return AFFECT;}	
[*]				{ return MULT;}
[+]				{ return PLUS;}	
"/="			{ return DIFF;}
[=]				{ return EGALITE;}
[:]			{ return DEUX_P;}	
[/]				{ return DIV;}	
[(]				{ return PAR_OUV;}
[)]				{ return PAR_FER;}
[-]				{ return MINUS;}
[0-9]*+\.[0-9]*				{ set_const(atof(yytext)); return FNUMBER;}
[0-9]+				{ set_const(atof(yytext)); return INUMBER;}
.         {; }
%%
