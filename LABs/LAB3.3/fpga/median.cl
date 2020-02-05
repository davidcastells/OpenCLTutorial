

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

#define IMAGE_WIDTH 352
#define FIFO_SIZE (IMAGE_WIDTH*2+3)

#define GET_FIFO(fifo, x, y)    fifo[y*IMAGE_WIDTH+x]

unsigned char medianSort(unsigned char fifo[FIFO_SIZE])
{
    unsigned char max_0_0, max_0_1, max_0_2;
    unsigned char med_0_0, med_0_1, med_0_2;
    unsigned char min_0_0, min_0_1, min_0_2;
    
    sort3(GET_FIFO(fifo, 0, 0), GET_FIFO(fifo, 0, 1), GET_FIFO(fifo, 0, 2), &max_0_0, &med_0_0, &min_0_0);
    sort3(GET_FIFO(fifo, 1, 0), GET_FIFO(fifo, 1, 1), GET_FIFO(fifo, 1, 2), &max_0_1, &med_0_1, &min_0_1);
    sort3(GET_FIFO(fifo, 2, 0), GET_FIFO(fifo, 2, 1), GET_FIFO(fifo, 2, 2), &max_0_2, &med_0_2, &min_0_2);
    
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
    
    unsigned char fifo_r[FIFO_SIZE];
    unsigned char fifo_g[FIFO_SIZE];
    unsigned char fifo_b[FIFO_SIZE];
    
    for (int i=-FIFO_SIZE; i < w*h; i++)
    {
        #pragma unroll
        for (int k=0; k < FIFO_SIZE-1; k++)
        {
            fifo_r[k] = fifo_r[k+1];
            fifo_g[k] = fifo_g[k+1];
            fifo_b[k] = fifo_b[k+1];
        }
        
        int xr = (i+FIFO_SIZE) % IMAGE_WIDTH;
        int yr = (i+FIFO_SIZE) / IMAGE_WIDTH;
        
        unsigned char r, g, b;
        image_getRGB(inputImage, w, h, xr, yr, &r, &g, &b);
        
        fifo_r[FIFO_SIZE-1]= r;
        fifo_g[FIFO_SIZE-1]= g;
        fifo_b[FIFO_SIZE-1]= b;
        
        unsigned char nr, ng, nb;
        
        int x = i % IMAGE_WIDTH;
        int y = i / IMAGE_WIDTH;
        
        nr = medianSort(fifo_r);
        ng = medianSort(fifo_g);
        nb = medianSort(fifo_b);
        
        bitmap_setRGB(outputImage, w, h, x, y, nr, ng, nb);
    }
}