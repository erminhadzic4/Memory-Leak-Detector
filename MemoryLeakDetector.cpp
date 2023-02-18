#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

bool write_to_file = false;
#define WRITE_TO_FILE(enable) write_to_file = enable;
#define DEBUG_NEW new(__FILE__, __LINE__)

void* operator new(std::size_t size, const char* file, int line)
{
    void* ptr = std::malloc(size);
    std::cout << "Allocating " << size << " bytes at " << ptr << " in " << file << " line " << line << std::endl;

    if (write_to_file)
    {
        std::ofstream out("memory_leaks.txt", std::ios::app);
        out << "Allocating " << size << " bytes at " << ptr << " in " << file << " line " << line << std::endl;
    }

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

        if (write_to_file)
        {
            std::ofstream out("memory_leaks.txt");
            out << "Memory Leak Detection Started" << std::endl;
            out << "-----------------------------" << std::endl;
        }
    }

    static void end()
    {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Memory Leak Detection Ended" << std::endl;

        if (write_to_file)
        {
            std::ofstream out("memory_leaks.txt", std::ios::app);
            out << "-----------------------------" << std::endl;
            out << "Memory Leak Detection Ended" << std::endl;
        }
    }
};

int main()
{
    MemoryLeakDetector::start();

    // Enable writing to file
    WRITE_TO_FILE(true);

    int* p = DEBUG_NEW int;
    double* q = DEBUG_NEW double[10];

    // Disable writing to file
    WRITE_TO_FILE(false);

    MemoryLeakDetector::end();
    
    return 0;
}
