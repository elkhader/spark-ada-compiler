#ifndef VM_H
#define VM_H

#include "pseudocode.h"


void inintialiser_machine_abstraite();


void interpreter_pseudo_instruction(struct pseudoinstruction pi, char ** next_label_name);


void interpreter_pseudo_code(pseudocode pc);

#endif

