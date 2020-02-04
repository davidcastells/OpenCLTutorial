/*
 * Copyright (C) 2020 Universitat Autonoma de Barcelona - David Castells-Rufas <david.castells@uab.cat>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * Copyright (C) 2018 Universitat Autonoma de Barcelona 
 * David Castells-Rufas <david.castells@uab.cat>
 * 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   FPGAInterface.cpp
 * Author: dcr
 * 
 * Created on January 18, 2019, 6:39 PM
 */

#include "FPGAInterface.h"
#include "PerformanceLap.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

FPGAInterface::FPGAInterface() 
{
    m_verbose = 1;
}


FPGAInterface::~FPGAInterface() 
{
}

void OpenCLBuildNotifierBridge(cl_program prog, void *user_data)
{
    FPGAInterface* pObj = (FPGAInterface*) user_data;
    pObj->notifyBuild(prog);
}

void FPGAInterface::notifyBuild(cl_program program)
{
    cl_int ret;
    size_t logSize;
    char* sLog;

    ret = clGetProgramBuildInfo( program , m_deviceId, CL_PROGRAM_BUILD_LOG, 0 , NULL, &logSize);
    SAMPLE_CHECK_ERRORS(ret);
    
    sLog = new char[logSize];

    if (!sLog) 
    {
        cout << "Unable to allocate string for build info (" << logSize << " bytes needed)" << endl;
        return;
    }

    ret = clGetProgramBuildInfo(program , m_deviceId, CL_PROGRAM_BUILD_LOG, logSize , sLog , &logSize) ;
    SAMPLE_CHECK_ERRORS(ret);
    
    cout << "Compilation Output:" << endl;
    if (logSize)
        cout << sLog << endl;
    else
        cout << "...empty!" << endl;
    
    delete sLog;
}

void FPGAInterface::finalizeOpenCL()
{
    finalizeKernels();
}

void FPGAInterface::initOpenCL()
{
    cl_uint spi = 0;
    cl_uint sdi = 0;

    if (m_verbose)
        cout << "[OCLFPGA] initialization (version compiled "  << __DATE__  << " "  << __TIME__ << ")" << endl;
    
    try
    {
        if(!setCwdToExeDir()) 
        {
            if (m_verbose)
                cout << "[OCLFPGA] ### ERROR ### Failed to change dir" << endl;
            return;
        }

        m_platform = selectPlatform(spi);

        if (m_verbose)
            cout << "[*] Platform Selected [OK] " << endl;
        
        m_deviceId = selectDevice(m_platform, sdi);

        if (m_verbose)
            cout << "[*] Device Selected [OK] " << endl;


        m_context = createContext(m_platform, m_deviceId);

        if (m_verbose)
            cout << "[*] Context Created [OK] " << endl;

        m_queue = createQueue(m_deviceId, m_context, 0);

        if (m_verbose)
            cout << "[*] Queue Created [OK] " << endl;

        m_queue2 = createQueue(m_deviceId, m_context, 0);

        if (m_verbose)
            cout << "[*] Queue 2 Created [OK] " << endl;

        m_openCLFilesPath = ".";

        if (m_verbose)
            cout << "Default Path=  " << m_openCLFilesPath << endl;


        initKernels();

    }
    catch (Error& err)
    {
        printf("%s\n", err.what());
        exit(0);
    }
}


void FPGAInterface::initKernels()
{
    cl_int ret;
       
    if (m_verbose)
    {
        printf("Reading Kernel files\n");
    }

    string platformName = getPlatformName(m_platform);
        
    printf("[OCLFPGA] Platform Name = %s\n", platformName.c_str());

    cl_program program;
                

    uint64 t0, tf;

    perfCounter(&t0);

    // NVIDIA devices should work with binaries (PTX)

    const unsigned char* pKernels[10]; 
    size_t pKernelSizes[10];
    cl_int pStatus[10];

    memset(pKernels, 0, sizeof(pKernels));
    memset(pKernelSizes, 0, sizeof(pKernelSizes));

    int kernelCount = 0;

    std::string fullPath  = m_openCLFilesPath.c_str() + std::string("/../fpga/gamma.aocx");


    printf("Opening %s\n", fullPath.c_str());

    setCwd(m_openCLFilesPath.c_str());

    int sysret = system("pwd");

    printf("Create Program From Binary...\n");

    program = createProgramFromBinary(m_context, fullPath.c_str(), &m_deviceId, 1); 

    printf("[OK]\n");

    printf("Compiling...\n");
    fflush(stdout);

    //ret = clBuildProgram(program, 1, &device, "-g -s", NULL, NULL);   // in Windows
    ret = clBuildProgram(program, 1, &m_deviceId, NULL, NULL, NULL);

    printf("[OK]\n");
    fflush(stdout);

    perfCounter(&tf);

    printf("[INFO] Kernel Compilation Time: %f\n", (float) secondsBetweenLaps(t0, tf));

    //if (ret == CL_BUILD_PROGRAM_FAILURE) 
    {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, m_deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, m_deviceId, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
    }

    SAMPLE_CHECK_ERRORS(ret);

    for (int i=0; i < kernelCount; i++)
    {
        free((void*)pKernels[i]);
    }

    fflush(stderr);
    fflush(stdout);

    //string allCode = m_addSourceCode + "\n" + m_mulSourceCode;

    m_gammaKernel = clCreateKernel(program, "gammaImage", &ret);
    SAMPLE_CHECK_ERRORS(ret);

    
        
    fflush(stderr);
    fflush(stdout);


}


void FPGAInterface::gammaImage(EmbeddedImage* image, CBitmap* bitmap, double gamma)
{
    cl_int ret;
    
    int w = image->getWidth();
    int h = image->getHeight();
    
    PerformanceLap lap;
    lap.start();
    
    m_memIn = clCreateBuffer(m_context, CL_MEM_READ_WRITE, sizeof(char)*3*w*h, NULL, &ret);
    SAMPLE_CHECK_ERRORS(ret);
    
    m_memOut = clCreateBuffer(m_context, CL_MEM_READ_WRITE, sizeof(char)*3*w*h, NULL, &ret);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clEnqueueWriteBuffer(m_queue, m_memIn, CL_TRUE, 0, sizeof(char)*3*w*h, image->getBytes(), 0, NULL, NULL);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clSetKernelArg(m_gammaKernel, 0, sizeof(cl_mem), (void *)&m_memIn);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clSetKernelArg(m_gammaKernel, 1, sizeof(cl_int), (void *)&w);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clSetKernelArg(m_gammaKernel, 2, sizeof(cl_int), (void *)&h);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clSetKernelArg(m_gammaKernel, 3, sizeof(cl_double), (void *)&gamma);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clSetKernelArg(m_gammaKernel, 4, sizeof(cl_mem), (void *)&m_memOut);
    SAMPLE_CHECK_ERRORS(ret);
    
    lap.stop();
    //printf("Argument Setting= %f seconds\n", lap.lap());
    
    lap.start();
    
    // send the events to the FPGA
    size_t wgSize[3] = {1, 1, 1};   // workgroup size
    size_t gSize[3] = {w, h, 1};    // global size

    ret = clEnqueueNDRangeKernel(m_queue, m_gammaKernel, 1, NULL, gSize, wgSize, 0, NULL, NULL);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clFinish(m_queue);
    SAMPLE_CHECK_ERRORS(ret);
    
    lap.stop();
    printf("Kernel time= %f seconds\n", lap.lap());
    
    lap.start();
    
    ret = clEnqueueReadBuffer(m_queue, m_memOut, CL_TRUE, 0, sizeof(char)*3*w*h, bitmap->getBytes(), 0, NULL, NULL);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clReleaseMemObject(m_memIn);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clReleaseMemObject(m_memOut);
    SAMPLE_CHECK_ERRORS(ret);
    
    lap.stop();
    //printf("Restult fetch= %f seconds\n", lap.lap());
}

void FPGAInterface::finalizeKernels()
{
    cl_int ret;
    
    ret = clReleaseKernel(m_gammaKernel);
    SAMPLE_CHECK_ERRORS(ret);

}
