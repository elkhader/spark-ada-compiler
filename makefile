
BASE_DIR := ${shell pwd}
OUT_DIR := ${BASE_DIR}/OUT
IN_DIR := ${BASE_DIR}/IN





all : compiler interpreter clean



lex : 
	cd ${IN_DIR}; \
	flex -i scanner.l
	cd ${IN_DIR}; \
	mv lex.yy.c zz.yy.c
	cd ${IN_DIR}; \
	gcc -c zz.yy.c

cfg : 
	cd ${IN_DIR}; \
	gcc -c cfg.c

pile : 
	cd ${IN_DIR}; \
	gcc -c pile.c

vm : 
	cd ${IN_DIR}; \
	gcc -c vm.c

error :
	cd ${IN_DIR}; \
	gcc -c error.c

op_co_aff: 
	cd ${IN_DIR}; \
	gcc -c op_co_aff.c

tabsym :  
	cd ${IN_DIR}; \
	gcc -c tabsym.c

ast : 
	cd ${IN_DIR}; \
	gcc -c ast.c

pseudocode : 
	cd ${IN_DIR}; \
	gcc -c pseudocode.c

parser_spark_v0 : 
	cd ${IN_DIR}; \
	gcc -c parser_spark_v0.c





compiler :  lex cfg error op_co_aff tabsym parser_spark_v0 ast pseudocode pile vm
	cd ${IN_DIR}; \
	gcc -o spark-compiler cfg.o ast.o error.o op_co_aff.o tabsym.o pseudocode.o pile.o vm.o  zz.yy.o parser_spark_v0.o
	mv ${IN_DIR}/spark-compiler ${OUT_DIR}/spark-compiler

interpreteurlex : 
	cd ${IN_DIR}; \
	flex -i interpreteur.l
	cd ${IN_DIR}; \
	mv lex.yy.c interpreteur.yy.c
	cd ${IN_DIR}; \
	gcc -c interpreteur.yy.c

interpreteur : 
	cd ${IN_DIR}; \
	gcc -c interpreteur_spark.c

interpreter : interpreteurlex interpreteur error tabsym pseudocode pile vm
	cd ${IN_DIR}; \
	gcc -o spark-interpreter error.o tabsym.o pseudocode.o pile.o vm.o interpreteur.yy.o interpreteur_spark.o
	mv ${IN_DIR}/spark-interpreter ${OUT_DIR}/spark-interpreter




clean :
	rm ${IN_DIR}/*.o
	rm ${IN_DIR}/*.yy.c
	





