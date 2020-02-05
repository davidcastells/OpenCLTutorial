# LABs

## LAB 1 - Simple circuits

We start with an Image contrast circuit in HDL and then we show how to create the same circuit in OpenCL 
and easily use it from Software.
Then we create another simple circuit (to do Image Gamma correction) using floating point arithmetics.

LAB 1.1 - Pixel contrast circuit in HDL

LAB 1.2 - Pixel contrast circuit in OpenCL

LAB 1.3 - Pixel Gamma correction circuit in OpenCL

## LAB 2 - Basic OpenCL Kernels

The overhead of function invocation is considerable. So, in order to get some acceleration ,we must transfer 
a big amount of data so that the accelerator can work fast on it and return. Following this idea we work on 
the whole image rather than pixel by pixel.
We present the basic kernel types: pipelined kernel and NDRange.

LAB 2.1 - Image gamma correction Pipelined Kernel

LAB 2.2 - Image gamma correction NDRange Kernel


## LAB 3 - Optimization of OpenCL Kernels

OpenCL allows to do certain optimizations. We present a new problem, the filtering of impulsive noise in images with
the Median filter.

LAB 3.1 - Median filter with parallel insert sort.

LAB 3.2 - Median filter with parallel sort

LAB 3.3 - Median filter with line buffer

