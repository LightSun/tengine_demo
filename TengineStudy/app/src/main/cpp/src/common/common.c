//
// Created by Administrator on 2020/9/22 0022.
//
#include "common.h"

#ifdef _WIN32
double get_current_time()
{
    LARGE_INTEGER freq;
    LARGE_INTEGER pc;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&pc);

    return pc.QuadPart * 1000.0 / freq.QuadPart;
}
#else    // _WIN32

double get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}
#endif    // _WIN32

void split(float* array, char* str, const char* del)
{
    char* s = NULL;
    s = strtok(str, del);
    while (s != NULL)
    {
        *array++ = atof(s);
        s = strtok(NULL, del);
    }
}
