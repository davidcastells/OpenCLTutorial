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
#include <stdlib.h>

bool gNoise = false;
bool gMedianSW = false;
bool gMedianHW = false;
bool gReportTime = false;



void usage()
{
    printf("Usage: test_contrast [OPTIONS]...\n");
    printf("\nCreates a bitmap with an optional gamma enhancement\n");
    printf("\nOptions:\n");
    printf("  -no \tCreate the noisy image test_noisy.bmp File\n");
    printf("  -sw \tCreate the filtered image in software test_sw.bmp File\n");
    printf("  -hw \tCreate the filtered image in hardware test_hw.bmp File\n");
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
        if (strcmp(args[i], "-no") == 0)
        {
            gNoise = true;
        }
        else if (strcmp(args[i], "-sw") == 0)
        {
            gMedianSW = true;
        }
        else if (strcmp(args[i], "-hw") == 0)
        {
            gMedianHW = true;
        }
        else if (strcmp(args[i], "-t") == 0)
        {
            gReportTime = true;
        }
    }

    if (!(gNoise || gMedianSW || gMedianHW))
        return 1;
    
    return 0;
}



int compare(const void * a, const void * b) 
{ 
    return ( *(int*)a - *(int*)b ); 
} 

void doMedianFilter(EmbeddedImage* image, CBitmap* bitmap)
{
    int w = image->getWidth();
    int h = image->getHeight();
    
    int ar[9];
    int ag[9];
    int ab[9];
    
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            
            for (int yd = -1; yd <= 1; yd++)
                for (int xd = -1; xd <= 1; xd++)
                {
                    int xr = x + xd;
                    int yr = y + yd;
                    
                    if (xr > w) xr = w-1;
                    if (xr < 0) xr = 0;
                    if (yr > h) yr = h-1;
                    if (yr < 0) yr = 0;
                    
                    int index = (yd+1)*3 + (xd+1);
                    image->getRGB(xr, yr, &ar[index], &ag[index], &ab[index]);

                }
            
            qsort(ar, 9, sizeof(int), compare);
            qsort(ag, 9, sizeof(int), compare);
            qsort(ab, 9, sizeof(int), compare);
            
            bitmap->setRGB(x, y, ar[4], ag[4], ab[4]);
            //bitmap->setRGB(x, y, ar[4], ag[4], ab[4]);
        }
}

void doCopyImage(EmbeddedImage* image, CBitmap* bitmap)
{
    for (int y = 0; y < image->getHeight(); y++)
        for (int x = 0; x < image->getWidth(); x++)
        {
            int r, g, b;
            image->getRGB(x, y, &r, &g, &b);
            bitmap->setRGB(x, y, r, g, b);
        }
}

void doGenerateNoise(EmbeddedImage* image, double percent)
{
    int nNoisePoints = image->getWidth() * image->getHeight() * percent;
    
    srand(0);
    
    for (int i=0; i < nNoisePoints; i++)
    {
        int x = rand() % image->getWidth();
        int y = rand() % image->getHeight();
        int color = rand() % 2;
        
        if (color)
            image->setRGB(x, y, 255, 255, 255);
        else
            image->setRGB(x, y, 0, 0, 0);
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
    
    double noisePercent = 0.2;
    
    doGenerateNoise(&image, noisePercent);
    
    PerformanceLap lap;
    
    if (gNoise)
    {
        lap.start();
        doCopyImage(&image, &bitmap);
        lap.stop();
        
        if (gReportTime)
            printf("Total  time= %f seconds\n", lap.lap());
        
        bitmap.Save("test_noisy.bmp");
    }
    else if (gMedianSW)
    {
        lap.start();
        doMedianFilter(&image, &bitmap);
        lap.stop();
        
        if (gReportTime)
            printf("Total  time= %f seconds\n", lap.lap());
        
        bitmap.Save("test_sw.bmp");
    }
    else if (gMedianHW)
    {
        FPGAInterface fpga;
        fpga.initOpenCL();
        
        lap.start();
        fpga.medianFilter(&image, &bitmap);
        lap.stop();
        
        if (gReportTime)
            printf("Total  time= %f seconds\n", lap.lap());
        
        bitmap.Save("test_hw.bmp");
        
        fpga.finalizeOpenCL();
    }
    
    
}