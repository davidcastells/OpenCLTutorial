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

void FPGAInterface::initOpenCL()
{
    cl_uint spi = 0;
    cl_uint sdi = 0;

    if (m_verbose)
        cout << "[OCLFPGA] initialization (version compiled "  << __DATE__  << " "  << __TIME__ << ")" << endl;
    
    try
    {
        //printAllEnv();

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
        
//        printf("env %p obj %p jstr %p\n", env, obj, jstr);
        
        
//        if (jstr != NULL)
//        {
//            const char *nativeString = env->GetStringUTFChars(jstr, 0);
//            // use your string
//            string openCLFilesPath = string(nativeString);
//            env->ReleaseStringUTFChars(jstr, nativeString);    
//        }


    initKernels();

        if (m_verbose)
        {
//            printf(" expected numBits = %d\n", NUM_XMP_BITS);
//            printf(" numBits = %d\n", BigIntegerArrayFPGA::m_numLimbs * 32) ;
//            printf(" numLimbs = %d\n", BigIntegerArrayFPGA::m_numLimbs) ;
//            printf(" numCount = %d\n" , BigIntegerArrayFPGA::m_numCount) ;
        }   
        
        

    }
    catch (Error& err)
    {
        
            printf("%s\n", err.what());
            return;
            // Avoid exiting in a JNI
            // exit(0);
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
                
        //if (platformName.find("NVIDIA") == string::npos)
//        if (1 < 2)
//        {
//            // If not an NVIDIA device, compile the source code
//            const char* pKernels[10]; 
//            size_t pKernelSizes[10];
//
//            memset(pKernels, 0, sizeof(pKernels));
//            memset(pKernelSizes, 0, sizeof(pKernelSizes));
//
//            int kernelCount = 0;
//
//            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "add.cl");
//            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "mul.cl");
//            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "div.cl");
//            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "multMod.cl");
//            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "powMod.cl");
////            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "powModMont.cl");
//
//            for (int i=0; i < kernelCount; i++)
//            {
//                pKernelSizes[i] = strlen(pKernels[i]);
//            }
//
//            if (verbose)
//                cout << "Compiling Kernel files (count " << kernelCount << ")" << endl << flush;
//
//            program = clCreateProgramWithSource(context, kernelCount, pKernels, pKernelSizes, &ret);
//            SAMPLE_CHECK_ERRORS(ret);
//
//            string flags = " -cl-nv-verbose -I " + openCLFilesPath;
//            //ret = clBuildProgram(program, 1, &device, "-g -s", NULL, NULL);   // in Windows
//            ret = clBuildProgram(program, 1, &device, flags.c_str(), NULL, NULL);
//
//            //if (ret == CL_BUILD_PROGRAM_FAILURE) 
//            {
//                // Determine the size of the log
//                size_t log_size;
//                clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
//
//                // Allocate memory for the log
//                char *log = (char *) malloc(log_size);
//
//                // Get the log
//                clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
//
//                // Print the log
//                printf("%s\n", log);
//
//
//                writeToFile(openCLFilesPath, "all.cl.err", log);
//                writeToFile(openCLFilesPath, "all.cl.in", pKernels[0]);
//            }
//
//            SAMPLE_CHECK_ERRORS(ret);
//
//            for (int i=0; i < kernelCount; i++)
//            {
//                free((void*)pKernels[i]);
//            }
//        }
//        else
        {
            uint64 t0, tf;
            
            perfCounter(&t0);
            
            // NVIDIA devices should work with binaries (PTX)

            const unsigned char* pKernels[10]; 
            size_t pKernelSizes[10];
            cl_int pStatus[10];

            memset(pKernels, 0, sizeof(pKernels));
            memset(pKernelSizes, 0, sizeof(pKernelSizes));

            int kernelCount = 0;
            
            std::string fullPath  = m_openCLFilesPath.c_str() + std::string("/../fpga/contrast.aocx");
            
            //std::string fullPath = openCLFilesPath.c_str() + std::string("/multMod.aocx");

            printf("Opening %s\n", fullPath.c_str());
            
            //string binaryPath = openCLFilesPath + string("/") + string("/multMod");

            setCwd(m_openCLFilesPath.c_str());
            
            int sysret = system("pwd");
            
            //std::string binaryPath = getBoardBinaryFile("big_integer", device);
            
//            pKernels[kernelCount] = readBinaryFile(openCLFilesPath, "multMod.aocx", &pKernelSizes[kernelCount]); kernelCount++;
            /*pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "mul.cl");
            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "div.cl");
            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "multMod.cl");
            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "powMod.cl");
            pKernels[kernelCount++] = readCodeFromFile(openCLFilesPath, "powModMont.cl");*/

//            if (verbose)
//                cout << "Compiling Kernel files (count " << kernelCount << ")" << endl << flush;
//
//            program = clCreateProgramWithBinary(context, 1, &device, pKernelSizes, pKernels, pStatus, &ret);
//            SAMPLE_CHECK_ERRORS(ret);
            
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


//                writeToFile(openCLFilesPath, "all.cl.err", log);
//                writeToFile(openCLFilesPath, "all.cl.in", pKernels[0]);
            }

            SAMPLE_CHECK_ERRORS(ret);

            for (int i=0; i < kernelCount; i++)
            {
                free((void*)pKernels[i]);
            }
        }
        
        
        fflush(stderr);
        fflush(stdout);
        
	//string allCode = m_addSourceCode + "\n" + m_mulSourceCode;
        
	m_contrastKernel = clCreateKernel(program, "contrast", &ret);
	SAMPLE_CHECK_ERRORS(ret);

    m_memOutV = clCreateBuffer(m_context, CL_MEM_READ_WRITE, 1*sizeof(int), NULL, &ret);
    SAMPLE_CHECK_ERRORS(ret);
        
    fflush(stderr);
    fflush(stdout);


}

void FPGAInterface::contrast(int* v)
{
    cl_int ret;
    
    ret = clSetKernelArg(m_contrastKernel, 0, sizeof(cl_int), (void *)v);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clSetKernelArg(m_contrastKernel, 1, sizeof(cl_mem), (void *)&m_memOutV);
    SAMPLE_CHECK_ERRORS(ret);
    
    // send the events to the FPGA
    size_t wgSize[3] = {1, 1, 1};
    size_t gSize[3] = {1, 1, 1};

    ret = clEnqueueNDRangeKernel(m_queue, m_contrastKernel, 1, NULL, gSize, wgSize, 0, NULL, NULL);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clEnqueueReadBuffer(m_queue, m_memOutV, CL_TRUE, 0, 1*sizeof(int), v, 0, NULL, NULL);
    SAMPLE_CHECK_ERRORS(ret);
}

void FPGAInterface::finalizeKernels()
{
    cl_int ret;
    
    ret = clReleaseMemObject(m_memOutV);
    SAMPLE_CHECK_ERRORS(ret);
    
    ret = clReleaseKernel(m_contrastKernel);
    SAMPLE_CHECK_ERRORS(ret);
    
    
	
}
