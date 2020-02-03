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

#include <iostream>

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


        initKernels(ct, pi, di, cq, openCLFilesPath);

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


void FPGAInterface::initKernels(cl_context context, 
        cl_platform_id platform, 
        cl_device_id device, 
        cl_command_queue cq,  
        string openCLFilesPath)
{
       
        if (m_verbose)
        {
            printf("Reading Kernel files\n");
            printf("Initial numLimbs = %d\n", BigIntegerArrayFPGA::m_numLimbs);
        }

        string platformName = getPlatformName(platform);
        
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
            
            std::string fullPath  = openCLFilesPath.c_str() + std::string("/big_integer.aocx");
            
            //std::string fullPath = openCLFilesPath.c_str() + std::string("/multMod.aocx");

            printf("Opening %s\n", fullPath.c_str());
            
            //string binaryPath = openCLFilesPath + string("/") + string("/multMod");

            setCwd(openCLFilesPath.c_str());
            
            system("pwd");
            
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
            
            program = createProgramFromBinary(context, fullPath.c_str(), &device, 1); 

            printf("[OK]\n");
            
            printf("Compiling...\n");
            fflush(stdout);
            
            //ret = clBuildProgram(program, 1, &device, "-g -s", NULL, NULL);   // in Windows
            ret = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

            printf("[OK]\n");
            fflush(stdout);
            
            perfCounter(&tf);
            
            printf("[INFO] Kernel Compilation Time: %f\n", (float) secondsBetweenLaps(t0, tf));
            
            //if (ret == CL_BUILD_PROGRAM_FAILURE) 
            {
                // Determine the size of the log
                size_t log_size;
                clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

                // Allocate memory for the log
                char *log = (char *) malloc(log_size);

                // Get the log
                clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

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

        
//	m_addKernel = clCreateKernel(program, "addNumbers", &ret);
//	SAMPLE_CHECK_ERRORS(ret);
//        
//        if (verbose)
//            cout << "addNumbers Kernel [OK]" << endl;
//
//	m_mulKernel = clCreateKernel(program, "mulNumbers", &ret);
//	SAMPLE_CHECK_ERRORS(ret);
//    
//        if (verbose)
//            cout << "mulNumbers Kernel [OK]" << endl;
//
//        m_divKernel = clCreateKernel(program, "divNumbers", &ret);
//	SAMPLE_CHECK_ERRORS(ret);
//        
//        if (verbose)
//            cout << "divNumbers Kernel [OK]" << endl;
//        

#ifdef USE_KERNEL_MULT_MOD
        m_mulModKernel = clCreateKernel(program, "multModNumbers", &ret);
	SAMPLE_CHECK_ERRORS(ret);

        if (verbose)
            cout << "multModNumbers Kernel [OK]" << endl;
#endif
        
#ifdef USE_KERNEL_POW_MOD        
        m_powModKernel = clCreateKernel(program, "powModNumbers", &ret);
	SAMPLE_CHECK_ERRORS(ret);

        if (verbose)
            printf("powModNumbers Kernel [OK]\n");
        
//        m_powModMontKernel = clCreateKernel(program, "powModMontNumbers", &ret);
//	SAMPLE_CHECK_ERRORS(ret);
//
//        if (verbose)
//            cout << "powModMontNumbers Kernel [OK}" << endl;
#endif
        
        fflush(stderr);
        fflush(stdout);
        
	m_cq = cq;
        
        if (verbose)
        {
            cout << " Arg numLimbs = " << to_string(numLimbs) << endl;
            cout << " Final numLimbs = " << to_string(BigIntegerArrayFPGA::m_numLimbs) << endl;
            cout << " Final Count = " << to_string(BigIntegerArrayFPGA::m_numCount) << endl;
        }
}

void BigIntegerArrayFPGA::finalizeKernels()
{
	
	cl_int ret;
        ret = clReleaseKernel(m_powModKernel);
	SAMPLE_CHECK_ERRORS(ret);
        ret = clReleaseKernel(m_mulModKernel);
	SAMPLE_CHECK_ERRORS(ret);
        ret = clReleaseKernel(m_divKernel);
	SAMPLE_CHECK_ERRORS(ret);
        ret = clReleaseKernel(m_mulKernel);
	SAMPLE_CHECK_ERRORS(ret);
	ret = clReleaseKernel(m_addKernel);
	SAMPLE_CHECK_ERRORS(ret);
	ret = clReleaseMemObject(m_memA);
	SAMPLE_CHECK_ERRORS(ret);
	ret = clReleaseMemObject(m_memB);
	SAMPLE_CHECK_ERRORS(ret);
        ret = clReleaseMemObject(m_memC);
	SAMPLE_CHECK_ERRORS(ret);
	ret = clReleaseMemObject(m_memQ);
	SAMPLE_CHECK_ERRORS(ret);
        ret = clReleaseMemObject(m_memR);
	SAMPLE_CHECK_ERRORS(ret);
}
