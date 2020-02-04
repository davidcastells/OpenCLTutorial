unsigned char doGamma(unsigned char inv, double nGamma)
{
    double dvin = inv;
    double dvout = 255.0*pow(dvin/255.0, 1.0/nGamma);
    
    if (dvout < 0)
        return 0;
    else if (dvout > 255)
        return 255;
    else
        return dvout;
}

void image_getRGB(__global unsigned char* inputImage, int w, int h, int x, int y, unsigned char* r, unsigned char* g, unsigned char* b)
{
    __global unsigned char* pixel = &inputImage[(y*w+x)*3];

    *r = pixel[0];
    *g = pixel[1];
    *b = pixel[2];
}

void bitmap_setRGB(__global unsigned char* outputImage, int w, int h, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned offset = ((h - (y+1))*w + x)*3;

    
    outputImage[offset+0] = b;
    outputImage[offset+1] = g;
    outputImage[offset+2] = r;
}

__kernel void gammaImage(__global unsigned char* inputImage, int w, int h, double nGamma, __global unsigned char* outputImage)
{
    int y = get_global_id(1);
    int x = get_global_id(0);
    //for (int y = 0; y < h; y++)
        //for (int x = 0; x < w; x++)
        {
            unsigned char r,g,b;
            image_getRGB(inputImage, w, h, x, y, &r, &g, &b);

            r = doGamma(r, nGamma);
            g = doGamma(g, nGamma);
            b = doGamma(b, nGamma);

            bitmap_setRGB(outputImage, w, h, x, y, r, g, b);
        }
}

