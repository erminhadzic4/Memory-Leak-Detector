#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

bool write_to_file = false;
bool include_file = false;
bool include_line = false;
bool include_total_allocs = false;

int total_allocations = 0;
int total_deallocations = 0;
int total_allocated_bytes = 0;

#define WRITE_TO_FILE(enable) write_to_file = enable;
#define INCLUDE_FILE(enable) include_file = enable;
#define INCLUDE_LINE(enable) include_line = enable;
#define INCLUDE_TOTAL_ALLOCS(enable) include_total_allocs = enable;
#define DEBUG_NEW new(__FILE__, include_line ? __LINE__ : 0)

// Class to store memory allocation information
class MemoryAllocationInfo {
public:
    void* ptr;
    const char* file;
    int line;

    MemoryAllocationInfo(void* p, const char* f, int l) : ptr(p), file(f), line(l) {}
};

class MemoryLeakDetector
{
private:
    static std::vector<MemoryAllocationInfo> allocations;

public:
    static void add_allocation(void* ptr, const char* file, int line)
    {
        allocations.emplace_back(ptr, file, line);
    }

    static void remove_allocation(void* ptr)
    {
        auto it = std::find_if(allocations.begin(), allocations.end(), [ptr](const MemoryAllocationInfo& info) {
            return info.ptr == ptr;
            });

        if (it != allocations.end()) {
            allocations.erase(it);
        }
    }

    static void clear()
    {
        allocations.clear();
    }


    static void check()
    {
        if (!allocations.empty())
        {
            std::cout << "Memory leaks detected!" << std::endl;
            for (const auto& info : allocations)
            {
                std::cout << "Memory leak at " << info.ptr;
                if (include_file)
                {
                    std::cout << " in " << info.file;
                }
                if (include_line)
                {
                    std::cout << " at line " << info.line;
                }
                std::cout << std::endl;

                if (write_to_file)
                {
                    std::ofstream out("memory_leaks.txt", std::ios::app);
                    out << "Memory leak at " << info.ptr;
                    if (include_file)
                    {
                        out << " in " << info.file;
                    }
                    if (include_line)
                    {
                        out << " at line " << info.line;
                    }
                    out << std::endl;
                }
            }
            clear();
        }
        else {
            std::cout << "No memory leaks detected!" << std::endl;
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
        std::cout << "Total allocations: " << total_allocations << std::endl;
        std::cout << "Total deallocations: " << total_deallocations << std::endl;
        std::cout << "Total allocated bytes: " << total_allocated_bytes << std::endl;

        if (write_to_file)
        {
            std::ofstream out("memory_leaks.txt", std::ios::app);
            out << "-----------------------------" << std::endl;
            out << "Memory Leak Detection Ended" << std::endl;
            out << "Total allocations: " << total_allocations << std::endl;
            out << "Total deallocations: " << total_deallocations << std::endl;
            out << "Total allocated bytes: " << total_allocated_bytes << std::endl;
        }

        check();
        clear();
    }
};

std::vector<MemoryAllocationInfo> MemoryLeakDetector::allocations;

void* operator new(std::size_t size, const char* file, int line)
{
    void* ptr = std::malloc(size);
    total_allocations++;
    total_allocated_bytes += size;

    if (include_file && include_line)
    {
        std::cout << "Allocating " << size << " bytes at " << ptr << " in " << file << " line " << line << std::endl;
    }
    else if (include_file)
    {
        std::cout << "Allocating " << size << " bytes at " << ptr << " in " << file << std::endl;
    }
    else if (include_line)
    {
        std::cout << "Allocating " << size << " bytes at " << ptr << " line " << line << std::endl;
    }
    else
    {
        std::cout << "Allocating " << size << " bytes at " << ptr << std::endl;
    }

    if (write_to_file)
    {
        std::ofstream out("memory_leaks.txt", std::ios::app);
        if (include_file && include_line)
        {
            out << "Allocating " << size << " bytes at " << ptr << " in " << file << " line " << line << std::endl;
        }
        else if (include_file)
        {
            out << "Allocating " << size << " bytes at " << ptr << " in " << file << std::endl;
        }
        else if (include_line)
        {
            out << "Allocating " << size << " bytes at " << ptr << " line " << line << std::endl;
        }
        else
        {
            out << "Allocating " << size << " bytes at " << ptr << std::endl;
        }
    }

    MemoryLeakDetector::add_allocation(ptr, file, line);

    return ptr;
}

// Operator overloading to track memory deallocation
void operator delete(void* ptr) noexcept
{
    // Remove allocation info from global data structure
    MemoryLeakDetector::remove_allocation(ptr);
    total_deallocations++;
    std::free(ptr);
}

#define new DEBUG_NEW

int main()
{
    MemoryLeakDetector::start();
    INCLUDE_FILE(false);
    INCLUDE_LINE(true);

    //WRITE_TO_FILE(true);

    int* ptr1 = new int;
    int* ptr2 = new int[10];

    //WRITE_TO_FILE(false);

    MemoryLeakDetector::end();
}
