//
// Created by Elijah on 18/12/2017.
//

#include "test_kernels.h"

int main() {
    if (!test_kernels(TRUE)) {
        fprintf(stderr, "FAILED AT test_kernels");
        return 1;
    }
    printf("All tests passed.");
}