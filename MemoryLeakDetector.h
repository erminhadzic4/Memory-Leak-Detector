#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>

extern bool write_to_file;
extern bool include_file;
extern bool include_line;
extern bool include_total_allocs;
extern int total_allocated_bytes;

static bool write_to_file = false;
static bool include_file = false;
static bool include_line = false;
static bool include_total_allocs = false;

class MemoryAllocationInfo {
public:
    void* ptr;
    const char* file;
    int line;
    bool freed;
    MemoryAllocationInfo(const char* f, int l) : ptr(nullptr), file(f), line(l), freed(false) {}
    MemoryAllocationInfo(void* ptr, const char* f, int l, bool fr) : ptr(ptr), file(f), line(l), freed(fr) {}
};

class MemoryLeakDetector
{
private:
    static std::vector<MemoryAllocationInfo> allocations;
public:
    friend void operator delete(void* ptr) noexcept;
    friend void operator delete[](void* ptr) noexcept;
    friend void* operator new(std::size_t size, const char* file, int line);
    static void add_allocation(void* ptr, const char* file, int line);
    static int remove_allocation(void* ptr);
    static void clear();
    static void check();
    static void start();
    static void end();
};

void* operator new(std::size_t size, const char* file, int line);
void operator delete[](void* ptr) noexcept;
void operator delete(void* ptr) noexcept;
