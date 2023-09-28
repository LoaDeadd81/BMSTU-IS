#include "enigma.h"

unsigned int backtrack(enigma_t *enigma, unsigned int symbol, unsigned int rotor_index, int *status);

enigma_t *new_enigma(unsigned int rotor_num, unsigned int rotor_size) {
    enigma_t *enigma = malloc(sizeof(enigma_t));
    if (enigma == NULL) return NULL;


    enigma->commutator = malloc(sizeof(unsigned int) * rotor_size);
    if (enigma->commutator == NULL) {
        free(enigma);
        return NULL;
    }
    enigma->reflector = malloc(sizeof(unsigned int) * rotor_size);
    if (enigma->reflector == NULL) {
        free(enigma->commutator);
        free(enigma);
        return NULL;
    }

    enigma->rotors = malloc(sizeof(unsigned int *) * rotor_num);
    for (int i = 0; i < rotor_num; ++i) enigma->rotors[i] = malloc(sizeof(unsigned int) * rotor_size);

    enigma->indexes = calloc(rotor_num, sizeof(unsigned int));

    enigma->rotor_size = rotor_size;
    enigma->rotor_num = rotor_num;

    return enigma;
}

void free_enigma(enigma_t *enigma) {
    for (int i = 0; i < enigma->rotor_num; ++i) free(enigma->rotors[i]);
    free(enigma->rotors);
    free(enigma->reflector);
    free(enigma->commutator);
    free(enigma->indexes);
    free(enigma);
}

void set_commutator(enigma_t *enigma, const unsigned int *commutator) {
    for (int i = 0; i < enigma->rotor_size; ++i) enigma->commutator[i] = commutator[i];
}

void set_rotors(enigma_t *enigma, unsigned int **rotors) {
    for (int i = 0; i < enigma->rotor_num; ++i)
        for (int j = 0; j < enigma->rotor_size; ++j)
            enigma->rotors[i][j] = rotors[i][j];

}

void set_reflector(enigma_t *enigma, const unsigned int *reflector) {
    for (int i = 0; i < enigma->rotor_size; ++i) enigma->reflector[i] = reflector[i];
}

unsigned int encrypt(enigma_t *enigma, unsigned int symbol, int *status) {
    *status = 1;
    if (enigma == NULL) {
        *status = 0;
        return 0;
    }
    if (symbol >= enigma->rotor_size) {
        *status = 0;
        return 0;
    }

    symbol = enigma->commutator[symbol];
    for (int i = 0; i < enigma->rotor_num; ++i)
        symbol = enigma->rotors[i][(symbol + enigma->indexes[i]) % enigma->rotor_size];

    symbol = enigma->reflector[symbol];

    for (int i = enigma->rotor_num - 1; i >= 0; --i) {
        symbol = backtrack(enigma, symbol, i, status);
        if (*status == 0) return 0;
    }
    symbol = enigma->commutator[symbol];

    int shift = 1;
    for (int i = 0; i < enigma->rotor_num && shift == 1; ++i) {
        shift = 0;
        if (enigma->indexes[i] >= enigma->rotor_size - 1) shift = 1;
        enigma->indexes[i] = (enigma->indexes[i] + 1) % enigma->rotor_size;
    }

    return symbol;
}

unsigned int backtrack(enigma_t *enigma, unsigned int symbol, unsigned int rotor_index, int *status) {
    *status = 1;

    for (int i = 0; i < enigma->rotor_size; ++i)
        if (enigma->rotors[rotor_index][i] == symbol)
            return i < enigma->indexes[rotor_index] ?
                   enigma->rotor_size - (enigma->indexes[rotor_index] - i)
                                                    : i - enigma->indexes[rotor_index];

    *status = 0;
    return 0;
}


