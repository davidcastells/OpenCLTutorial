

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

#define CHAR_MAX 255

void resetSort(unsigned char a[9])
{
    for (int i=0; i < 9; i++)
        a[i] = CHAR_MAX;
}

void insertSort(unsigned char a[9], unsigned char v)
{
    char cur[9];
    char prev[9];
        
    #pragma unroll
    for (int i=0; i < 9; i++)
    {
        cur[i] = (v < a[i]);
        prev[i] = (i==0) ? 0 : (v < a[i-1]);
    }
    
    char loadUp[9];
    char loadV[9];
    
    #pragma unroll
    for (int i=0; i < 9; i++)
    {
        loadUp[i] = cur[i] & prev[i];
        loadV[i] = cur[i] & (~prev[i]);
    }
    
    #pragma unroll
    for (int i=9-1; i >= 0; i--)
    {
        a[i] = (loadV[i]) ? v : (loadUp[i])? a[i-1] : a[i];
    }
    
}

__kernel void medianFilter(__global unsigned char* inputImage, int w, int h, __global unsigned char* outputImage)
{
    unsigned char ar[9];
    unsigned char ag[9];
    unsigned char ab[9];
    
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            resetSort(ar);
            resetSort(ag);
            resetSort(ab);
            
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
                    unsigned char r, g, b;
                    image_getRGB(inputImage, w, h, xr, yr, &r, &g, &b);
                    
                    insertSort(ar, r);
                    insertSort(ag, g);
                    insertSort(ab, b);
                }
            
            //image_getRGB(inputImage, w, h, x, y, &ar[4], &ag[4], &ab[4]);
            bitmap_setRGB(outputImage, w, h, x, y, ar[4], ag[4], ab[4]);
        }
}