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
#include "OpenCLUtils.h"
#include "EmbeddedImage.h"
#include "Bitmap.h"
#include "FPGAInterface.h"
#include "PerformanceLap.h"

#include <math.h>

bool gGammaSW = false;
bool gGammaHW = false;
bool gReportTime = false;

double nGamma = 1;

void usage()
{
    printf("Usage: test_contrast [OPTIONS]...\n");
    printf("\nCreates a bitmap with an optional gamma enhancement\n");
    printf("\nOptions:\n");
    printf("  -sw <value>\tCreate the gamma image in software test_sw.bmp File\n");
    printf("  -hw <value>\tCreate the gamma image in hardware test_hw.bmp File\n");
    printf("  -t\tMeasure time");
    printf("\n");
}

/**
 * Parse parameters
 * @param argc
 * @param args
 * @return 1 if any error occurred
 */
int parseArguments(int argc, char* args[])
{
    for (int i=0; i < argc; i++)
    {
        if (strcmp(args[i], "-sw") == 0)
        {
            nGamma = atof(args[++i]);
            gGammaSW = true;
        }         
        else if (strcmp(args[i], "-hw") == 0)
        {
            nGamma = atof(args[++i]);
            gGammaHW = true;
        }
        else if (strcmp(args[i], "-t") == 0)
        {
            gReportTime = true;
        }
    }

    if (!(gGammaSW || gGammaHW))
        return 1;
    
    return 0;
}


int doGamma(int v)
{
    double dvin = v;
    double dvout = 255.0*pow(dvin/255.0, 1.0/nGamma);
    
    if (dvout < 0) return 0;
    if (dvout > 255) return 255;
    return dvout;
}

void doGammaImage(EmbeddedImage* image, CBitmap* bitmap)
{
    for (int y = 0; y < image->getHeight(); y++)
        for (int x = 0; x < image->getWidth(); x++)
        {
            int r,g,b;
            image->getRGB(x, y, &r, &g, &b);

            r = doGamma(r);
            g = doGamma(g);
            b = doGamma(b);

            bitmap->setRGB(x, y, r, g, b);
        }
}

int main(int argc, char* args[])
{
    if (parseArguments(argc, args))
    {
        usage();
        exit(-1);
    }

    EmbeddedImage image;
    CBitmap bitmap(image.getWidth(), image.getHeight());
    
    PerformanceLap lap;
    
    
    if (gGammaSW)
    {
        lap.start();
        doGammaImage(&image, &bitmap);
        lap.stop();
        
        if (gReportTime)
            printf("Total  time= %f seconds\n", lap.lap());
        
        bitmap.Save("test_sw.bmp");
    }
    else if (gGammaHW)
    {
        FPGAInterface fpga;
        fpga.initOpenCL();
        
        lap.start();
        fpga.gammaImage(&image, &bitmap, nGamma);
        lap.stop();
        
        if (gReportTime)
            printf("Total  time= %f seconds\n", lap.lap());
        
        bitmap.Save("test_hw.bmp");
        
        fpga.finalizeOpenCL();
    }
    
    
}