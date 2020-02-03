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
#ifndef EMBEDDEDIMAGE_H_INCLUDED_
#define EMBEDDEDIMAGE_H_INCLUDED_

#include "Image.h"

#define GIMP_IMAGE_WIDTH (352)
#define GIMP_IMAGE_HEIGHT (288)
#define GIMP_IMAGE_BYTES_PER_PIXEL (3) /* 3:RGB, 4:RGBA */
#define GIMP_IMAGE_PIXEL_DATA ((unsigned char*) GIMP_IMAGE_pixel_data)

extern const unsigned char GIMP_IMAGE_pixel_data[352 * 288 * 3 + 1];


class EmbeddedImage : public Image
{
private:
    int w;
    int h;
    
public:
    EmbeddedImage()
    {
        w = GIMP_IMAGE_WIDTH;
        h = GIMP_IMAGE_HEIGHT;
    }
    
    virtual int getWidth() 
    {
        return w;
    }
    
    virtual int getHeight() 
    {
        return h;
    }
    
    /**
     * 
     */
    virtual int getRGB(int x, int y)
    {
        unsigned char* pixel = &GIMP_IMAGE_PIXEL_DATA[(y*w+x)*GIMP_IMAGE_BYTES_PER_PIXEL];
        
        int ARGB = pixel[0] << 16 | pixel[1] << 8 | pixel[2];
        
        return ARGB;
    }
    
    /**
     */
     virtual void getRGB(int x, int y, int* r, int* g, int* b)
     {
         unsigned char* pixel = &GIMP_IMAGE_PIXEL_DATA[(y*w+x)*GIMP_IMAGE_BYTES_PER_PIXEL];
        
        *r = pixel[0];
        *g = pixel[1];
        *b = pixel[2];
     }
};

#endif

