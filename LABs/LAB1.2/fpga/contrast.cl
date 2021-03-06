#define FRAC_NUM 3
#define FRAC_DEN 2
#define N 64

__kernel void contrast(int inv, __global int* outv)
{
    int s1 = inv * FRAC_NUM;
    int s2 = s1 / FRAC_DEN;
    int s3 = s2 - N;
    
    *outv =  (s2 < N) ? 0 : (s3 > 255) ? 255 : s3;
}
