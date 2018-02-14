#include "run_tests/run_tests.h"

#define VERBOSE TRUE

int main() {
    cl_device_id device;
    cl_context context;

    setContext(&device, &context, TRUE);

    run_all_tests(device, context, VERBOSE);
}
