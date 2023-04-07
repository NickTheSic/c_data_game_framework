
#ifndef NL_RANDOM_H_
#define NL_RANDOM_H_

static unsigned int g_random_seed = 1312;
static unsigned int 
XORRandom(unsigned int seed = g_random_seed)
{
    g_random_seed = seed;

    g_random_seed ^= g_random_seed << 13;
    g_random_seed ^= g_random_seed >> 17;
    g_random_seed ^= g_random_seed << 5;

    return g_random_seed;
}

#endif //NL_RANDOM_H_