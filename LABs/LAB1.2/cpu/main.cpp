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

bool gOriginal = false;
bool gContrast = false;
bool gReportTime = false;

void usage()
{
    printf("Usage: test_contrast [OPTIONS]...\n");
    printf("\nCreates a bitmap with an optional contrast enhancement\n");
    printf("\nOptions:\n");
    printf("  -o\tCreate the original image as test_original.bmp File\n");
    printf("  -c\tCreate the contrast image as test_contrast.bmp File\n");
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
        if (strcmp(args[i], "-o") == 0)
        {
            gOriginal = true;
        }         
        else if (strcmp(args[i], "-c") == 0)
        {
            gContrast = true;
        }
        else if (strcmp(args[i], "-t") == 0)
        {
            gReportTime = true;
        }
    }

    if (!(gOriginal || gContrast))
        return 1;
    
    return 0;
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
    
    if (gOriginal)
    {
        for (int y = 0; y < image.getHeight(); y++)
            for (int x = 0; x < image.getWidth(); x++)
            {
                int r,g,b;
                image.getRGB(x, y, &r, &g, &b);
                bitmap.setRGB(x, y, r, g, b);
            }
        
        bitmap.Save("test_original.bmp");
    }
    else if (gContrast)
    {
        FPGAInterface fpga;
        fpga.initOpenCL();
        
        for (int y = 0; y < image.getHeight(); y++)
            for (int x = 0; x < image.getWidth(); x++)
            {
                int r,g,b;
                image.getRGB(x, y, &r, &g, &b);
                fpga.contrast(&r);
                fpga.contrast(&g);
                fpga.contrast(&b);
                bitmap.setRGB(x, y, r, g, b);
            }
        
        bitmap.Save("test_contrast.bmp");
    }
    
}