#include<stdio.h>

#include "libmath.h"


int main()
{
int x=12;
int y=3;

printf(" x+y = %d\n",add(x,y));

printf(" x-y = %d\n",sub(x,y));

printf(" x*y = %d\n",mult(x,y));

printf(" x/y = %d\n",divide(x,y));

    return 0;
}