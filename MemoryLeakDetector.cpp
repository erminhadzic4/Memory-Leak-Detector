#include <iostream>
#include <cstdlib>
#include <cstring>

#define DEBUG_NEW new(__FILE__, __LINE__)

void* operator new(std::size_t size, const char* file, int line)
{
    void* ptr = std::malloc(size);
    std::cout << "Allocating " << size << " bytes at " << ptr << " in " << file << " line " << line << std::endl;
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}

class MemoryLeakDetector
{
public:
    static void start()
    {
        std::cout << "Memory Leak Detection Started" << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }

    static void end()
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Memory Leak Detection Ended" << std::endl;
    }
};

int main()
{
    MemoryLeakDetector::start();


    MemoryLeakDetector::end();
}
