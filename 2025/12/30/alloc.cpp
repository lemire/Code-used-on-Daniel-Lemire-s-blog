#include <iostream>
#include <cstddef>
#include <memory>
#include <cstdlib>
#ifdef __APPLE__
#include <malloc/malloc.h>  // for malloc_size on macOS
#endif
#ifdef __linux__
#include <malloc.h> // for malloc_usable_size on Linux
#endif
size_t get_usable_size(void* ptr) {
#ifdef __linux__
    return malloc_usable_size(ptr);
#elif defined(__APPLE__)
    return malloc_size(ptr);
#else
    return 0;  // Unsupported platform
#endif
}

int main() {
    std::cout << "Demonstrating allocation overhead and rounding with operator new\n\n";

#ifdef __linux__
    std::cout << "Platform: Linux\n";
#elif defined(__APPLE__)
    std::cout << "Platform: macOS (using malloc_size)\n";
#else
    std::cout << "Platform: Other/unsupported (usable size will show 0)\n";
#endif

    std::cout << "Requested size | Actual usable size\n";
    std::cout << "---------------|-------------------\n";
    size_t total_requested = 0;
    size_t total_usable = 0;
    for (size_t requested = 1; requested <= 4096; requested++) {
        total_requested += requested;
        std::unique_ptr<char[]> ptr(new char[requested]);  // Allocate
        size_t usable = get_usable_size(ptr.get());  // Get usable size
        total_usable += usable;

        std::cout << requested << "\t       | " << usable << "\n";
    }
    std::cout << "---------------|-------------------\n";
    std::cout << "Total requested: " << total_requested << " bytes\n";
    std::cout << "Total usable:    " << total_usable << " bytes\n";
    std::cout << "Total overhead:  " << (total_usable - total_requested) << " bytes\n";
    std::cout << "Percentage overhead: "
              << ((total_usable - total_requested) * 100.0 / total_requested) << " %\n";

    return EXIT_SUCCESS;
}
