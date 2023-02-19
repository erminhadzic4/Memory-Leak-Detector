#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

bool write_to_file = false;
bool include_file = true;
bool include_line = true;

#define WRITE_TO_FILE(enable) write_to_file = enable;
#define INCLUDE_THE_FILE(enable) include_file = enable;
#define INCLUDE_THE_LINE(enable) include_line = enable;
#define DEBUG_NEW new(__FILE__, include_line ? __LINE__ : 0)

class MemoryLeakDetector
{
private:
    static std::vector<void*> allocated_pointers;

public:
    static void add(void* ptr)
    {
        allocated_pointers.push_back(ptr);
    }

    static void remove(void* ptr)
    {
        auto it = std::find(allocated_pointers.begin(), allocated_pointers.end(), ptr);
        if (it != allocated_pointers.end())
        {
            allocated_pointers.erase(it);
        }
    }

    static void clear()
    {
        allocated_pointers.clear();
    }

    static void check()
    {
        if (!allocated_pointers.empty())
        {
            std::cout << "Memory leaks detected!" << std::endl;
            for (void* ptr : allocated_pointers)
            {
                std::cout << "Memory leak at " << ptr;

                if (include_line)
                {
                    std::cout << " at line " << __LINE__;
                }

                std::cout << std::endl;

                if (write_to_file)
                {
                    std::ofstream out("memory_leaks.txt", std::ios::app);
                    out << "Memory leak at " << ptr;

                    if (include_line)
                    {
                        out << " at line " << __LINE__;
                    }

                    out << std::endl;
                }
            }
        }
        else
        {
            std::cout << "No memory leaks detected." << std::endl;
        }
    }

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

        check();
        clear();
    }
};

std::vector<void*> MemoryLeakDetector::allocated_pointers;

void* operator new(std::size_t size, const char* file, int line)
{
    void* ptr = std::malloc(size);

    std::cout << "Allocating " << size << " bytes at " << ptr;
    if (include_file)
    {
        std::cout << " in " << file;
    }
    if (include_line)
    {
        std::cout << " line " << line;
    }
    std::cout << std::endl;

    if (write_to_file)
    {
        std::ofstream out("memory_leaks.txt", std::ios::app);
        out << "Allocating " << size << " bytes at " << ptr;
        if (include_file)
        {
            out << " in " << file;
        }
        if (include_line)
        {
            out << " line " << line;
        }
        out << std::endl;
    }

    MemoryLeakDetector::add(ptr);

    return ptr;
}

#define new DEBUG_NEW

void operator delete(void* ptr) noexcept
{
    MemoryLeakDetector::remove(ptr);
    std::free(ptr);
}



int main()
{
    MemoryLeakDetector::start();
    INCLUDE_THE_FILE(false);
    INCLUDE_THE_LINE(true);

    //WRITE_TO_FILE(true);

    int* ptr1 = new int;
    int* ptr2 = new int[10];

    //WRITE_TO_FILE(false);

    MemoryLeakDetector::end();
}
