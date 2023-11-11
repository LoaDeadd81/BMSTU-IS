#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "AES.h"

#define ENCODE_MODE 0
#define DECODE_MODE 1
#define END "0101010101010101"

typedef long int sz_t;

void print_bytes(uint8_t *array, size_t length) {
    printf("[ ");
    for (size_t i = 0; i < length; ++i)
        printf("%x ", array[i]);
    printf("]\n");
}

int main(int argc, char **argv) {
    uint8_t key[KEY_SIZE] = "";

    if (argc < 5) {
        perror("not enought args");
        return -1;
    }

    int mode = 0;
    if (strcmp(argv[1], "E") == 0) mode = ENCODE_MODE;
    else if (strcmp(argv[1], "D") == 0) mode = DECODE_MODE;
    else {
        perror("wrong mode");
        return -1;
    }

    strcpy(key, argv[2]);

    int input = open(argv[3], O_RDONLY);
    if (input < 0) {
        perror("error while opening input file");
        return -1;
    }

    int output = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (output < 0) {
        close(input);
        perror("error while opening output file");
        return -1;
    }

    uint32_t *keys = get_keys(key);
    if (keys == NULL) {
        close(input);
        close(output);
        printf("%s\n", "mem allocation for keys failed");
        return -1;
    }

    uint8_t *buf = calloc(BLOCK_SIZE, sizeof(uint8_t));
    uint8_t *res = calloc(BLOCK_SIZE, sizeof(uint8_t));
    uint8_t *res_lst = calloc(BLOCK_SIZE, sizeof(uint8_t));
    if (buf == NULL || res == NULL || res_lst == NULL) {
        close(input);
        close(output);
        free(keys);
        free(buf);
        free(res);
        free(res_lst);
        return -1;
    }


    if (mode == ENCODE_MODE) {
        int len = 0, flag = 0;
        sz_t total = 0;
        while ((len = read(input, buf, BLOCK_SIZE)) == BLOCK_SIZE) {
//            print_bytes(buf, BLOCK_SIZE);
            encode_block(res, buf, keys);
//            print_bytes(res, BLOCK_SIZE);
            if (write(output, res, BLOCK_SIZE) < 0) {
                close(input);
                close(output);
                free(keys);
                free(buf);
                free(res);
                printf("%s\n", "write ciphertext failed");
                return -1;
            }
            total += len;
        }
        if (len < 0) {
            close(input);
            close(output);
            free(keys);
            free(buf);
            free(res);
            printf("%s\n", "read plain text failed");
            return -1;
        }
        if (len != 0) {
            memset(buf + len, 0, BLOCK_SIZE - len);
//            print_bytes(buf, BLOCK_SIZE);
            encode_block(res, buf, keys);
//            print_bytes(res, BLOCK_SIZE);
            if (write(output, res, BLOCK_SIZE) < 0) {
                close(input);
                close(output);
                free(keys);
                free(buf);
                free(res);
                printf("%s\n", "write ciphertext failed");
                return -1;
            }
            total += len;
        }

        char end[] = END;
        write(output, &end, sizeof(end) - 1);
        while (total > 0) {
            char tmp = total % 10;
            if (write(output, &tmp, sizeof(char)) < 0) {
                close(input);
                close(output);
                free(keys);
                free(buf);
                free(res);
                printf("%s\n", "write total size failed");
                return -1;
            }
            total /= 10;
        }
    } else if (mode == DECODE_MODE) {
        int len = 0;
        while ((len = read(input, buf, BLOCK_SIZE)) == BLOCK_SIZE) {
            if(strcmp(buf, END) == 0) break;
//            print_bytes(buf, BLOCK_SIZE);
            decode_block(res, buf, keys);
//            print_bytes(res, BLOCK_SIZE);
            if (write(output, res, BLOCK_SIZE) < 0) {
                close(input);
                close(output);
                free(keys);
                free(buf);
                free(res);
                printf("%s\n", "write plain text failed");
                return -1;
            }
        }
        if (len < 0) {
            close(input);
            close(output);
            free(keys);
            free(buf);
            free(res);
            printf("%s\n", "read ciphertext failed");
            return -1;
        }


        sz_t total = 0;
        char symbol = 0;
        int i = 0;
        while (read(input, &symbol, sizeof(char)) > 0) {
            total += symbol * pow(10, i++);
        }
        ftruncate(output, total);
    }

    close(input);
    close(output);
    free(keys);
    free(buf);
    free(res);
    return 0;
}