//
// Created by Elijah on 18/12/2017.
//

#include "test_kernels/test_kernels.h"

#define VERBOSE FALSE

int main() {
    if (!test_kernels(VERBOSE)) {
        fprintf(stderr, "FAILED AT test_kernels");
        return 1;
    }
    printf("All tests passed.");
}