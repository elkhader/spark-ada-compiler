# spark-ada-compiler
A project that has been accomplished in a team of 4, during my 3rd semester of my Computer Science program. The compiler is built and developped in C, and dedicated for the real-time programming language Spark from Ada. 
 
 
 ### Grammar
 The following expressions were implemented in the code. 
 ``` 
PROG: procedure  idf  is  LISTE_DECL begin LISTE_INST end idf ;
LISTE_INST : INST LISTE_INSTAUX 
LISTE_INSTAUX : LISTE_INST | epsilon 
LISTE_DECL : DECL LISTE_DECLAUX 
LISTE_DECLAUX : LISTE_DECL | epsilon 
DECL : idf ‘ : ’ TYPE DECL_AUX 
DECL_AUX : ‘ :=’ CONST ';' | ';' 
TYPE : integer | boolean | float | character 
CONST : inumber | fnumber | cnumber |  true | false 
INST : idf := ADDSUB ';' | true ';' | false ';'  | cnumber ';' | New_line ; | put ‘(‘ idf ’)’ ‘ ;’ | put_line ‘(‘ idf ’)’ ‘ ;’|  for IDF in  inumber ‘ ..’ inumber loop LISTE_INST end loop ‘ ;’ | if ‘(‘ idf = ADDSUB ‘)’ then LISTE_INST IF_INSTAUX 
IF_INSTAUX : end if ‘ ;’ | else  LISTE_INST end if ;
ADDSUB : MULTDIV ADDSUBAUX 
ADDSUBAUX : – MULTDIV ADDSUBAUX | + MULTDIV ADDSUBAUX |  epsilon
MULTDIV : AUX MULTDIVAUX 
MULTDIVAUX : * AUX MULTDIVAUX | / AUX MULTDIVAUX | epsilon 
AUX : idf | inumber | fnumber | ( ADDSUB ) 
```

