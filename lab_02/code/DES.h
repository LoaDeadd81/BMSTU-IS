#ifndef LAB_02_DES_H
#define LAB_02_DES_H

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define BUFF_SIZE INT_MAX

size_t encode(uint8_t *to, uint8_t *keys8b, uint8_t *from, size_t length);

size_t decode(uint8_t *to, uint8_t *keys8b, uint8_t *from, size_t length);

#endif
