#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
    const int rotor_num = 3;
    const int size = 256;

    int arr[size] = {};
    srand(time(NULL));

    for (int i = 0; i < size; ++i) arr[i] = i;

    for (int i = 0; i < rotor_num + 1; ++i) {
        for (int j = 0; j < size; ++j) {
            int k = rand() % size;
            int tmp = arr[j];
            arr[j] = arr[k];
            arr[k] = tmp;
        }

        for (int j = 0; j < size; ++j) printf(j < size - 1 ? "%d, " : "%d", arr[j]);
        printf("\n");
        srand(time(NULL));
    }


}