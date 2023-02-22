#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>

#include "MemoryLeakDetector.h"
#include "Macros.h"

int main()
{
    MemoryLeakDetector::start();
    //INCLUDE_FILE(false);
    //INCLUDE_LINE(true);
    INCLUDE_TOTAL_ALLOCS(true);

    WRITE_TO_FILE(true);

    int* ptr1 = new int;
    //delete ptr1;
    //int* ptr2 = new int[10];  

    //WRITE_TO_FILE(false);

    MemoryLeakDetector::end();
}

