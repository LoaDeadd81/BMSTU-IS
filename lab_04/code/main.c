#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "rsa.h"
#include "md5.h"

#define BUFF_SIZE 1024

int gen(char *open_file_name, char *close_file_name);

int sign(char *file_name, char *key_file_name, char *sign_file_name);

int check(char *file_name, char *key_file_name, char *sign_file_name);

int hash(char *file_name);

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        printf("not enough args");
        return -1;
    }

    if (strcmp(argv[1], "G") == 0) {
        if (argc < 4) {
            printf("not enough args");
            return -1;
        }
        return gen(argv[2], argv[3]);
    } else if (strcmp(argv[1], "S") == 0) {
        if (argc < 5) {
            printf("not enough args");
            return -1;
        }
        return sign(argv[2], argv[3], argv[4]);
    } else if (strcmp(argv[1], "C") == 0) {
        if (argc < 5) {
            printf("not enough args");
            return -1;
        }
        int rc = check(argv[2], argv[3], argv[4]);
        if (rc == 0) printf("SUCCESSFUL CHECK\n");
        else if (rc == 1) printf("CHECK FAILED\n");

        return rc;
    } else if (strcmp(argv[1], "H") == 0) {
        if (argc < 3) {
            printf("not enough args");
            return -1;
        }
        return hash(argv[2]);
    }
    printf("wrong mode");


//    lli p = 61031, q = 32633;
//    rsa_key_t open_key, private_key;
//    rsa_key_gen(&open_key, &private_key, p, q);
//
//    lli data = 4434442, enc, dec;
//    printf("data: %lld\n", data);
//    enc = rsa(data, private_key);
//    printf("enc: %lld\n", enc);
//    dec = rsa(enc, open_key);
//    printf("dec: %lld\n", dec);
    return 0;
}

int gen(char *open_file_name, char *close_file_name) {
    int open_out = open(open_file_name, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (open_out < 0) {
        perror("open open_file_name error");
        return -1;
    }
    int close_out = open(close_file_name, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (close_out < 0) {
        perror("open close_file_name error");
        close(open_out);
        return -1;
    }

    rsa_key_t open_key, private_key;
    rsa_key_gen(&open_key, &private_key);

    if (write(open_out, &open_key, sizeof(rsa_key_t)) < 0) {
        perror("write open key error");
        close(open_out);
        close(close_out);
        return -1;
    }

    if (write(close_out, &private_key, sizeof(rsa_key_t)) < 0) {
        perror("write private key error");
        close(open_out);
        close(close_out);
        return -1;
    }

    close(open_out);
    close(close_out);

    return 0;
}

int hash_file(hash_state_t *state, int fd) {
    char *buff = malloc(1024);
    int input_size = 0;

    md5_init(state);

    while ((input_size = read(fd, buff, BUFF_SIZE)) > 0)
        md5_update(state, (uint8_t *) buff, input_size);
    if (input_size < 0) {
        perror("read file error");
        free(buff);
        return -1;
    }

    md5_finish(state);
    free(buff);
}

int sign(char *file_name, char *key_file_name, char *sign_file_name) {
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd < 0) {
        perror("open inp file error");
        return -1;
    }

    int key_fd = open(key_file_name, O_RDONLY);
    if (key_fd < 0) {
        perror("open inp file error");
        close(file_fd);
        return -1;
    }

    rsa_key_t key;
    if (read(key_fd, &key, sizeof(rsa_key_t)) < 0) {
        perror("read key error");
        close(file_fd);
        return -1;
    }

    int sign_fd = open(sign_file_name, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (sign_fd < 0) {
        perror("open close_file_name error");
        close(file_fd);
        close(key_fd);
        return -1;
    }

    hash_state_t state;
    if (hash_file(&state, file_fd) < 0) {
        close(file_fd);
        close(sign_fd);
        close(key_fd);
        return -1;
    }

    lli enc_sign[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i) enc_sign[i] = rsa((lli) state.digest[i], key);

    if (write(sign_fd, &enc_sign, sizeof(lli) * HASH_SIZE) < 0) {
        perror("write sign error");
        close(file_fd);
        close(sign_fd);
        close(key_fd);
        return -1;
    }

    close(file_fd);
    close(sign_fd);
    close(key_fd);
    return 0;
}

int check(char *file_name, char *key_file_name, char *sign_file_name) {
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd < 0) {
        perror("open inp file error");
        return -1;
    }

    int key_fd = open(key_file_name, O_RDONLY);
    if (key_fd < 0) {
        perror("open inp file error");
        close(file_fd);
        return -1;
    }

    rsa_key_t key;
    if (read(key_fd, &key, sizeof(rsa_key_t)) < 0) {
        perror("read key error");
        close(file_fd);
        return -1;
    }

    int sign_fd = open(sign_file_name, O_RDONLY);
    if (sign_fd < 0) {
        perror("open close_file_name error");
        close(file_fd);
        close(key_fd);
        return -1;
    }

    lli enc_sign[HASH_SIZE], dec_sign[HASH_SIZE];
    if (read(sign_fd, &enc_sign, sizeof(lli) * HASH_SIZE) < 0) {
        perror("read sign error");
        close(file_fd);
        close(sign_fd);
        close(key_fd);
        return -1;
    }
    for (int i = 0; i < HASH_SIZE; ++i) dec_sign[i] = rsa(enc_sign[i], key);

    hash_state_t state;
    if (hash_file(&state, file_fd) < 0) {
        close(file_fd);
        close(sign_fd);
        close(key_fd);
        return -1;
    }

    close(file_fd);
    close(sign_fd);
    close(key_fd);

    for (int i = 0; i < HASH_SIZE; ++i)
        if ((lli) state.digest[i] != dec_sign[i])
            return 1;

    return 0;
}

void print_hash(uint8_t p[HASH_SIZE]) {
    for (int i = 0; i < HASH_SIZE; ++i)
        printf("%02x", p[i]);
    printf("\n");
}

int hash(char *file_name) {
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd < 0) {
        perror("open inp file error");
        return -1;
    }

    hash_state_t state;
    if (hash_file(&state, file_fd) < 0) {
        close(file_fd);
        return -1;
    }

    print_hash(state.digest);

    return 0;
}
