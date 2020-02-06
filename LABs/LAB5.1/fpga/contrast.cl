
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

extern unsigned char doContrast(unsigned char x);

__kernel void contrastImage(__global unsigned char* restrict inputImage , int w, int h, __global unsigned char* restrict outputImage)
{
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            unsigned char r,g,b;
            image_getRGB(inputImage, w, h, x, y, &r, &g, &b);

            r = doContrast(r);
            g = doContrast(g);
            b = doContrast(b);

            bitmap_setRGB(outputImage, w, h, x, y, r, g, b);
        }
}

