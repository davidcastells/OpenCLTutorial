

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

void sort3(unsigned char a, unsigned char b, unsigned char c, unsigned char* max, unsigned char* med, unsigned char* min)
{
    if (a > b)
    {
        if (b > c)
        {
            // a > b > c
            *max = a;
            *med = b;
            *min = c;
        }
        else
        {
            // b < c
            if (a > c)
            {
                // a > c > b
                *max = a;
                *med = c;
                *min = b;
            }
            else
            {
                // c > a > b
                *max = c;
                *med = a;
                *min = b;
            }
        }
    }
    else
    {
        // a < b
        if (b > c)
        {
            // b > c
            if (a > c)
            {
                // b > a > c 
                *max = b;
                *med = a;
                *min = c;
            }
            else
            {
                // b > c > a 
                *max = b;
                *med = c;
                *min = a;
            }
        }
        else
        {
            // c > b > a
            *max = c;
            *med = b;
            *min = a;
        }
    }
        
}

unsigned char medianSort(unsigned char a[9])
{
    unsigned char max_0_0, max_0_1, max_0_2;
    unsigned char med_0_0, med_0_1, med_0_2;
    unsigned char min_0_0, min_0_1, min_0_2;
    
    sort3(a[0], a[1], a[2], &max_0_0, &med_0_0, &min_0_0);
    sort3(a[3], a[4], a[5], &max_0_1, &med_0_1, &min_0_1);
    sort3(a[6], a[7], a[8], &max_0_2, &med_0_2, &min_0_2);
    
    unsigned char max_1_0, max_1_1, max_1_2;
    unsigned char med_1_0, med_1_1, med_1_2;
    unsigned char min_1_0, min_1_1, min_1_2;
    
    sort3(max_0_0, max_0_1, max_0_2, &max_1_0, &med_1_0, &min_1_0);
    sort3(med_0_0, med_0_1, med_0_2, &max_1_1, &med_1_1, &min_1_1);
    sort3(min_0_0, min_0_1, min_0_2, &max_1_2, &med_1_2, &min_1_2);
    
    unsigned char max_2, med_2, min_2;
    
    sort3(min_1_0, med_1_1, max_1_2, &max_2, &med_2, &min_2);
    
    return med_2;
}


__kernel void medianFilter(__global unsigned char* inputImage, int w, int h, __global unsigned char* outputImage)
{
    unsigned char ar[9];
    unsigned char ag[9];
    unsigned char ab[9];
    
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