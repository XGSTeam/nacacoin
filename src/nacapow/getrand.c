#define MAX(a,b) ((a) >= (b) ? (a) : (b))
#define MIN(a,b) ((a) <= (b) ? (a) : (b))

double getrand(long int *idum)
{
    static long int idum2 = 123456789;
    static long int iv[32] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0 };
    static long int iy = 0;

    long int i__1;
    double ret_val, d__1;
    static long int j, k;

    if (*idum <= 0)
    {
        // Computing MAX
        i__1 = -(*idum);
        *idum = MAX(i__1, 1);
        // be sure to prevent idum=0
        idum2 = *idum;
        for (j = 40; j >= 1; --j)
        {
            // load the shuffle table (after 8 warm-up
            k = *idum / 53668;
            *idum = (*idum - k * 53668) * 40014 - k * 12211;
            if (*idum < 0)
            {
                *idum += 2147483563;
            }
            if (j <= 32)
            {
                iv[j - 1] = *idum;
            }
            /* L11: */
        }
        iy = iv[0];
}

    k = *idum / 53668;
    // start here when no initializing
    *idum = (*idum - k * 53668) * 40014 - k * 12211;

    // compute idum=mod(IA1*idum,IM1) without overflows by Schrage's
    // method

    if (*idum < 0)
    {
        *idum += 2147483563;
    }
    k = idum2 / 52774;
    idum2 = (idum2 - k * 52774) * 40692 - k * 3791;

    // compute idum2=mod(IA2*idum2,IM2) likewise

    if (idum2 < 0)
    {
        idum2 += 2147483399;
    }
    j = iy / 67108862 + 1;
    // will be in the range 1:NTAB
    iy = iv[j - 1] - idum2;

    // here idum is shuffled, idum and idum2 are combined to grnerate
    /*        output */

    iv[j - 1] = *idum;
    if (iy < 1)
    {
        iy += 2147483562;
    }
    /* Computing MIN */
    d__1 = iy * 4.6566130573917691e-10;
    ret_val = MIN(d__1, .99999987999999995);

    /* because users don't expect endpoints */
    return ret_val;
}


