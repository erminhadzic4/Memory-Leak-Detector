#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>

#include "MemoryLeakDetector.h"

int main()
{
    MemoryLeakDetector::start();
    INCLUDE_FILE(false);
    INCLUDE_LINE(false);
    INCLUDE_TOTAL_ALLOCS(false);
    WRITE_TO_FILE(false);

    int* ptr1 = new int;
    //delete ptr1;
    //int* ptr2 = new int[10];  

    //WRITE_TO_FILE(false);

    MemoryLeakDetector::end();
}

