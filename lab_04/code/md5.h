#ifndef __MD5_H__
#define __MD5_H__

#include <stdio.h>
#include <stdint.h>

#define HASH_SIZE 16

typedef struct hash_state_t{
    uint64_t size;
    uint32_t buffer[4];
    uint8_t input[64];
    uint8_t digest[HASH_SIZE];
}hash_state_t;

void md5_init(hash_state_t *state);
void md5_update(hash_state_t *state, uint8_t *input, size_t input_len);
void md5_finish(hash_state_t *state);

#endif