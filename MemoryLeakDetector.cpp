//#include "MemoryLeakDetector.h"
//
//std::vector<MemoryAllocationInfo> MemoryLeakDetector::allocations;
//
//extern bool include_file;
//extern bool include_line;
//extern bool include_total_allocs;
//extern int total_allocated_bytes;
//extern bool write_to_file;
//
//#define WRITE_TO_FILE(enable) write_to_file = enable;
//#define INCLUDE_FILE(enable) include_file = enable;
//#define INCLUDE_LINE(enable) include_line = enable;
//#define INCLUDE_TOTAL_ALLOCS(enable) include_total_allocs = enable;
//#define DEBUG_NEW new(__FILE__, include_line ? __LINE__ : 0)
//
//void MemoryLeakDetector::add_allocation(void* ptr, const char* file, int line)
//{
//    allocations.emplace_back(ptr, file, line, false);
//}
//
//int MemoryLeakDetector::remove_allocation(void* ptr)
//{
//    auto it = std::find_if(allocations.begin(), allocations.end(), [ptr](const MemoryAllocationInfo& info) {
//        return info.ptr == ptr;
//        });
//
//    if (it != allocations.end()) {
//        auto& info = *it;
//        if (info.freed) {
//            std::cout << "Double free detected at " << ptr << " in " << info.file << " line " << info.line << std::endl;
//            return -1;
//        }
//        else {
//            info.freed = true;
//            operator delete[](ptr);
//        }
//    }
//
//    return 1;
//}
//
//void MemoryLeakDetector::clear()
//{
//    allocations.clear();
//}
//
//void MemoryLeakDetector::check()
//{
//    bool leak = false;
//    for (auto allocation : allocations)
//        if (allocation.freed == false)
//            leak = true;
//    if (leak) {
//        std::cout << "Memory leaks detected!" << std::endl;
//        for (const auto& info : allocations) {
//            if (!info.freed) {
//                std::cout << "Memory leak at " << info.ptr;
//                if (include_file) {
//                    std::cout << " in " << info.file;
//                }
//                if (include_line) {
//                    std::cout << " at line " << info.line;
//                }
//                std::cout << std::endl;
//
//                if (write_to_file) {
//                    std::ofstream out("memory_leaks.txt", std::ios::app);
//                    out << "Memory leak at " << info.ptr;
//                    if (include_file) {
//                        out << " in " << info.file;
//                    }
//                    if (include_line) {
//                        out << " at line " << info.line;
//                    }
//                    out << std::endl;
//                }
//            }
//        }
//        clear();
//    }
//    else {
//        std::cout << "No memory leaks detected!" << std::endl;
//    }
//}
//
//void MemoryLeakDetector::start()
//{
//    std::cout << "Memory Leak Detection Started" << std::endl;
//    std::cout << "-----------------------------" << std::endl;
//
//    if (write_to_file)
//    {
//        std::ofstream out("memory_leaks.txt");
//        out << "Memory Leak Detection Started" << std::endl;
//        out << "-----------------------------" << std::endl;
//    }
//}
//
//void MemoryLeakDetector::end()
//{
//    std::cout << "-----------------------------" << std::endl;
//    std::cout << "Memory Leak Detection Ended" << std::endl;
//    if (include_total_allocs) {
//        std::cout << "Total allocated bytes: " << total_allocated_bytes << std::endl;
//    }
//
//    if (write_to_file)
//    {
//        std::ofstream out("memory_leaks.txt", std::ios::app);
//        out << "-----------------------------" << std::endl;
//        out << "Memory Leak Detection Ended" << std::endl;
//        if (include_total_allocs) {
//            out << "Total allocated bytes: " << total_allocated_bytes << std::endl;
//        }
//    }
//
//    check();
//    clear();
//}
//
//void operator delete[](void* ptr) noexcept
//{
//    const char* file = nullptr;
//    int line = 0;
//
//    auto it = std::find_if(MemoryLeakDetector::allocations.begin(), MemoryLeakDetector::allocations.end(),
//        [ptr](const MemoryAllocationInfo& info) { return info.ptr == ptr; });
//
//    if (it != MemoryLeakDetector::allocations.end()) {
//        auto& info = *it;
//        file = info.file;
//        line = info.line;
//        info.freed = true;
//        total_allocated_bytes -= _msize(ptr);
//        std::free(ptr);
//    }
//    else {
//        std::cout << "Error: invalid pointer passed to operator delete[]" << std::endl;
//    }
//
//    if (include_file && include_line)
//    {
//        std::cout << "Deallocating memory at " << ptr << " in " << file << " line " << line << std::endl;
//    }
//    else if (include_file)
//    {
//        std::cout << "Deallocating memory at " << ptr << " in " << file << std::endl;
//    }
//    else if (include_line)
//    {
//        std::cout << "Deallocating memory at " << ptr << " line " << line << std::endl;
//    }
//    else
//    {
//        std::cout << "Deallocating memory at " << ptr << std::endl;
//    }
//}
//
//
//void operator delete(void* ptr) noexcept
//{
//    int freed = MemoryLeakDetector::remove_allocation(ptr);
//    if (!freed)
//        std::free(ptr);
//
//}
//void* operator new(std::size_t size, const char* file, int line)
//{
//    void* ptr = std::malloc(size);
//    total_allocated_bytes += size;
//
//    if (include_file && include_line)
//    {
//        std::cout << "Allocating " << size << " bytes at " << ptr << " in " << file << " line " << line << std::endl;
//    }
//    else if (include_file)
//    {
//        std::cout << "Allocating " << size << " bytes at " << ptr << " in " << file << std::endl;
//    }
//    else if (include_line)
//    {
//        std::cout << "Allocating " << size << " bytes at " << ptr << " line " << line << std::endl;
//    }
//    else
//    {
//        std::cout << "Allocating " << size << " bytes at " << ptr << std::endl;
//    }
//
//    if (write_to_file)
//    {
//        std::ofstream out("memory_leaks.txt", std::ios::app);
//        if (include_file && include_line)
//        {
//            out << "Allocating " << size << " bytes at " << ptr << " in " << file << " line " << line << std::endl;
//        }
//        else if (include_file)
//        {
//            out << "Allocating " << size << " bytes at " << ptr << " in " << file << std::endl;
//        }
//        else if (include_line)
//        {
//            out << "Allocating " << size << " bytes at " << ptr << " line " << line << std::endl;
//        }
//        else
//        {
//            out << "Allocating " << size << " bytes at " << ptr << std::endl;
//        }
//    }
//
//    MemoryLeakDetector::add_allocation(ptr, file, line);
//    return ptr;
//}
//
