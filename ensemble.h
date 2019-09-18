#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define MALLOC(t,n) (t*)malloc(sizeof(t)*n)
#define CALLOC(t,n) (t*)calloc(sizeof(t),n)

#define MIN(x,y) x<y ? x : y