#ifndef BMSTU_IS_ENIGMA_H
#define BMSTU_IS_ENIGMA_H

#include <stdlib.h>

typedef struct enigma_t {
    unsigned int rotor_size;
    unsigned int rotor_num;
    unsigned int *commutator;
    unsigned int *reflector;
    unsigned int **rotors;
    unsigned int *indexes;
} enigma_t;

enigma_t *new_enigma(unsigned int rotor_num, unsigned int rotor_size);

void free_enigma(enigma_t *enigma);

void set_commutator(enigma_t *enigma, const unsigned int *commutator);
void set_rotors(enigma_t *enigma, unsigned int **rotors);
void set_reflector(enigma_t *enigma, const unsigned int *reflector);

unsigned int encrypt(enigma_t *enigma, unsigned int symbol, int *status);

#endif
