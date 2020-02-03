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
 * File:   FPGAInterface.h
 * Author: dcr
 *
 * Created on January 18, 2019, 6:39 PM
 */

#ifndef FPGAINTERFACE_H
#define FPGAINTERFACE_H

#include <string>

#include "OpenCLUtils.h"


using namespace std;
    
void OpenCLBuildNotifierBridge(cl_program prog, void *user_data);

class FPGAInterface 
{
public:
    FPGAInterface();
    virtual ~FPGAInterface();

public:
    void initOpenCL();
    void notifyBuild(cl_program program);
    
    void gamma(int* v, double gamma);
    
    void initKernels();
    void finalizeKernels();
    
private:
    int m_verbose;
public:    
    cl_context      m_context;
    cl_device_id    m_deviceId;
    cl_platform_id  m_platform;
    string m_openCLFilesPath;
    cl_command_queue m_queue;
    cl_command_queue m_queue2;
    cl_kernel m_gammaKernel;
    cl_mem      m_memOutV;
};

#endif /* FPGAINTERFACE_H */


