

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

//#define CHAR_MAX 255

void resetSort(unsigned char a[9])
{
    for (int i=0; i < 9; i++)
        a[i] = CHAR_MAX;
}


unsigned char minc(unsigned char a, unsigned char b)
{
    if (a < b)
        return a;
    else
        return b;
}

unsigned char maxc(unsigned char a, unsigned char b)
{
    if (a < b)
        return b;
    else
        return a;
}

unsigned char medc(unsigned char a, unsigned char b, unsigned char c)
{
    if (a > b)
        if (b > c)
            // a > b > c
            return b;
        else
            // b < c
            if (a > c)
                // a > c > b
                return c;
            else
                // c > a > b
                return a;
    else
        // a < b
        if (b > c)
            // b > c
            if (a > c)
                // b > a > c 
                return a;
            else
                // b > c > a 
                return c;
        else
            // c > b > a
            return b;
                
}

unsigned char medianSort(unsigned char a[9])
{
    unsigned char m0 = medc(a[0], a[1], a[2]);
    unsigned char m1 = medc(a[3], a[4], a[5]);
    unsigned char m2 = medc(a[6], a[7], a[8]);
    
    return medc(m0, m1, m2);
}


__kernel void medianFilter(__global unsigned char* inputImage, int w, int h, __global unsigned char* outputImage)
{
    unsigned char ar[9];
    unsigned char ag[9];
    unsigned char ab[9];
    
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
/*
            resetSort(ar);
            resetSort(ag);
            resetSort(ab);
*/
            
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
                    
                    image_getRGB(inputImage, w, h, xr, yr, &ar[index], &ag[index], &ab[index]);
                }
            
            unsigned char r, g, b;
            r = medianSort(ar);
            g = medianSort(ag);
            b = medianSort(ab);
                    
            //image_getRGB(inputImage, w, h, x, y, &ar[4], &ag[4], &ab[4]);
            bitmap_setRGB(outputImage, w, h, x, y, r, g, b);
        }
}