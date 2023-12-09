#include "rsa.h"

lli gcde(lli a, lli b, lli *x, lli *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    lli x1, y1;
    lli d = gcde(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return d;
}

lli mod_exp(lli num, lli exp, lli mod) {
    lli product = 1;

    while (exp > 0) {
        if (exp & 0x01) product = (product * num) % mod;
        num = (num * num) % mod;
        exp >>= 1;
    }

    return product;
}

void rsa_key_gen(rsa_key_t *public_key, rsa_key_t *private_key) {
    lli N = 0, phi = 0;
    lli e = E;

    N = P * Q;
    phi = (P - 1) * (Q - 1);

    lli x, y;
    gcde(e, phi, &x, &y);
    while (x < 0) x = x + phi;

    public_key->mod = N;
    public_key->exp = e;

    private_key->mod = N;
    private_key->exp = x;
}

lli rsa(const lli data, const struct rsa_key_t key) {
    return mod_exp(data, key.exp, key.mod);
}

//long long mod_mult(long long a, long long b, long long mod) {
//    long long res = 0;
//
//    while (b > 0) {
//        if (b & 1) res = (res + a) % mod;
//        a = (2 * a) % mod;
//        b >>= 1;
//    }
//
//    return res;
//}

//    num %= mod;
//    while (exp > 0) {
//        if (exp & 0x01) product = mod_mult(product, num, mod);
//        num = mod_mult(num, num, mod);
//        exp >>= 1;
//    }