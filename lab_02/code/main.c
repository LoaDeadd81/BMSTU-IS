#include <stdio.h>
#include <string.h>
#include <math.h>

#include "DES.h"

#define ENCODE_MODE 0
#define DECODE_MODE 1


int main(int argc, char **argv) {
    uint8_t key[9] = "";
    uint8_t *buf = calloc(BUFF_SIZE, sizeof(uint8_t));
    uint8_t *res = calloc(BUFF_SIZE, sizeof(uint8_t));

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
    if (key[8] != '\0') {
        perror("too long key");
        return -1;
    }
    FILE *input = fopen(argv[3], "r");
    if (input == NULL) {
        perror("error while opening input file");
        return -1;
    }
    FILE *output = fopen(argv[4], "w");
    if (output == NULL) {
        perror("error while opening output file");
        return -1;
    }


    if (mode == ENCODE_MODE) {
        size_t len = 0;
        int cnt = 0;
        int symbol = 0;
        while (cnt < BUFF_SIZE && (symbol = fgetc(input)) != EOF)
            buf[cnt++] = symbol;
        if (symbol != EOF) {
            perror("file too big");
            return -1;
        }

        len = encode(res, key, buf, cnt);

        fputc(cnt % 255, output);
        while (cnt > 255) {
            fputc(cnt / 255, output);
            cnt /= 255;
        }
        fputc('\0', output);

        for (int i = 0; i < len; ++i)
            fputc(res[i], output);

    } else if (mode == DECODE_MODE) {
        size_t len = 0;
        int cnt = 0;
        int symbol = 0;

        len = fgetc(input);
        int tmp = 1;
        while ((symbol = fgetc(input)) != '\0') {
            len += symbol * pow(255.0, tmp++);
        }

        while (cnt < BUFF_SIZE && (symbol = fgetc(input)) != EOF)
            buf[cnt++] = symbol;
        if (symbol != EOF) {
            perror("file too big");
            return -1;
        }

        decode(res, key, buf, cnt);

        for (int i = 0; i < len; ++i)
            fputc(res[i], output);

    }

    fclose(input);
    fclose(output);
    return 0;
}
