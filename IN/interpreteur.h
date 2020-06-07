#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include "cfg.h"



typedef enum {
ADDOP = 10000,
DIVOP = 10001,
DUPLOP = 10002,
JMPOP = 10003,
JNEOP = 10004,
JGOP = 10005,
LABELOP = 10006,
LOADOP = 10007,
MULTOP = 10008,
PRINTOP = 10009,
PUSHOP = 10010,
SUBOP = 10011,
STOREOP = 10012,
SWAPOP = 10013,
IDFOP = 10014,
DNUMBEROP = 10015,
INUMBEROP = 10016,
BEGINOP = 10017,
ENDOP = 10018
} typetokenOp;


void set_label_attributes(char *name, int line);

void set_idf_attributes(char *name, int line);

void set_number_attributes(double value);

void set_token_attributes(int line);
#endif
