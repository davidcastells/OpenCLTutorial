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

bool gContrastSW = false;
bool gContrastHW = false;
bool gReportTime = false;


void usage()
{
    printf("Usage: test_contrast [OPTIONS]...\n");
    printf("\nCreates a bitmap with an optional gamma enhancement\n");
    printf("\nOptions:\n");
    printf("  -sw \tCreate the contrast image in software test_sw.bmp File\n");
    printf("  -hw \tCreate the contrast image in hardware test_hw.bmp File\n");
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
            gContrastSW = true;
        }         
        else if (strcmp(args[i], "-hw") == 0)
        {
            gContrastHW = true;
        }
        else if (strcmp(args[i], "-t") == 0)
        {
            gReportTime = true;
        }
    }

    if (!(gContrastSW || gContrastHW))
        return 1;
    
    return 0;
}

#define FRAC_NUM 3
#define FRAC_DEN 2
#define N 64

int doContrast(int inv)
{
    int s1 = inv * FRAC_NUM;
    int s2 = s1 / FRAC_DEN;
    int s3 = s2 - N;
    
    return  (s2 < N) ? 0 : (s3 > 255) ? 255 : s3;
}

void doContrastImage(EmbeddedImage* image, CBitmap* bitmap)
{
    for (int y = 0; y < image->getHeight(); y++)
        for (int x = 0; x < image->getWidth(); x++)
        {
            int r,g,b;
            image->getRGB(x, y, &r, &g, &b);

            r = doContrast(r);
            g = doContrast(g);
            b = doContrast(b);

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
    
    
    if (gContrastSW)
    {
        lap.start();
        doContrastImage(&image, &bitmap);
        lap.stop();
        
        if (gReportTime)
            printf("Total  time= %f seconds\n", lap.lap());
        
        bitmap.Save("test_sw.bmp");
    }
    else if (gContrastHW)
    {
        FPGAInterface fpga;
        fpga.initOpenCL();
        
        lap.start();
        fpga.contrastImage(&image, &bitmap);
        lap.stop();
        
        if (gReportTime)
            printf("Total  time= %f seconds\n", lap.lap());
        
        bitmap.Save("test_hw.bmp");
        
        fpga.finalizeOpenCL();
    }
    
    
}
