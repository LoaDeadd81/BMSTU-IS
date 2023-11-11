#ifndef LAB_03_AES_H
#define LAB_03_AES_H

#include <stdlib.h>
#include <stdint.h>


#define BLOCK_SIZE 16
#define KEY_SIZE 16
#define NB 4
#define ROUND_NUM 10


uint32_t *get_keys(uint8_t *key);

void encode_block(uint8_t *dst, uint8_t *src, uint32_t *keys);

void decode_block(uint8_t *dst, uint8_t *src, uint32_t *keys);

#endif //LAB_03_AES_H
