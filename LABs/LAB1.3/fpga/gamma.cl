__kernel void gamma(int inv, double nGamma, __global int* outv)
{
    double dvin = inv;
    double dvout = 255.0*pow(dvin/255.0, 1.0/nGamma);
    
    if (dvout < 0)
        *outv = 0;
    else if (dvout > 255)
        *outv  = 255;
    else
        *outv = dvout;
}
