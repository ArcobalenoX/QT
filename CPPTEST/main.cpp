#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>
#include<sstream>
#include "cast_demo.h"
#include "complex.h"
#include "templatearray.h"

using namespace std;








int search(int* number, int len ,int target)
{
    if(number == NULL)
    {
        return -1;
    }

    int left=0;
    int right=len;
    while(left<right)
    {
        int mid = (left+right)/2;
        int midv = number[mid];
        if(target < midv)
        {
            left = mid+1;
        }
        else if(target > midv)
        {
           right = mid-1;
        }
        else
        {
            return mid;
        }

    }
    return -1;
}










int main()
{


return 0;
}










