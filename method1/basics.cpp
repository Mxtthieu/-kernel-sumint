// ----------------------------------------------------------

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// ----------------------------------------------------------

// We provide a small library so we can easily setup OpenCL
#include "clutils.h"

// ----------------------------------------------------------

int main(int argc,char **argv)
{

    const char *clu_File = SRC_PATH "basics.cl"; 

    // Initialize OpenCL
    cluInit();
    srand (time(NULL));

    // After this call you have access to
    // clu_Context;      <= OpenCL context (pointer)
    // clu_Devices;      <= OpenCL device list (vector)
    // clu_Queue;        <= OpenCL queue (pointer)

    // Load Kernel
    cl::Program *program = cluLoadProgram(clu_File);
    cl::Kernel  *kernel = cluLoadKernel(program, "simple_function");

    // allocate memory and opencl buffer
    const int size = 32;
    cl::Buffer a_buffer(*clu_Context, CL_MEM_READ_ONLY, size * sizeof(int));
    cl::Buffer b_buffer(*clu_Context, CL_MEM_WRITE_ONLY, sizeof(int));

    // fill a_buffer and b_buffer with values
    int* a = new int[size];
    for (int i = 0; i < size; i++) {
        a[i] = rand()%10;
    }
    
    int b[1] = {0};
    clu_Queue->enqueueWriteBuffer(a_buffer, true, 0, size * sizeof(int), a);
    clu_Queue->enqueueWriteBuffer(b_buffer, true, 0, size * sizeof(int), b);
    delete[] a;

    // execute kernel
    kernel->setArg(0, a_buffer);
    kernel->setArg(1, b_buffer);

    cl_int clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange,
        cl::NDRange(size),    // global work size
        cl::NDRange(size));   // group work size
    cluCheckError(clerr, "Error running the kernel");

    // copy back the results
    int res = 0;
    

    clu_Queue->enqueueReadBuffer(b_buffer, true, 0, sizeof(int), &res);

    std::cout << res <<std::endl;
    
    return 0;
}

// ----------------------------------------------------------

