#ifndef __RSA_H__
#define __RSA_H__

#define P 61031
#define Q 32633
#define E ((2 << 16) + 1)

typedef long long lli;

typedef struct rsa_key_t {
    lli mod;
    lli exp;
} rsa_key_t;


void rsa_key_gen(rsa_key_t *public_key, rsa_key_t *private_key);

lli rsa(lli data, rsa_key_t key);

#endif  //__RSA_H__
