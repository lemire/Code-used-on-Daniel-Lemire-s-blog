#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <filesystem>
#include <mutex>
#include <atomic>
#include <algorithm>

namespace fs = std::filesystem;

// Mutex for thread-safe output
std::mutex cout_mutex;


bool create_tmp_directory() {
    namespace fs = std::filesystem;
    
    try {
        // Define the directory path
        fs::path dirPath = "tmp";
        
        // Check if directory exists, create it if it doesn't
        if (!fs::exists(dirPath)) {
            if (fs::create_directory(dirPath)) {
                std::cout << "Successfully created 'tmp' directory\n";
                return true;
            } else {
                std::cerr << "Failed to create 'tmp' directory\n";
                return false;
            }
        } else {
            std::cout << "'tmp' directory already exists\n";
            return true; // Success since directory is available
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return false;
    }
}
// Function to create files if they don't exist
void create_files() {
    create_tmp_directory();
    for (int i = 1; i <= 10000; ++i) {
        std::string filename = "tmp/"+std::to_string(i) + ".txt";
        if (!fs::exists(filename)) {
            std::ofstream file(filename);
            if (file.is_open()) {
                file << "This is file " << i << std::endl;
                file.close();
            } else {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cerr << "Failed to create " << filename << std::endl;
            }
        }
    }
    std::cout << "File creation/check completed" << std::endl;
}

// Function to open files in a range and count successes
void open_files_range(int start, int end, std::atomic<int>& success_count) {
    for (int i = start; i <= end; ++i) {
        std::string filename = "tmp/" + std::to_string(i) + ".txt";
        std::ifstream file(filename);
        if (file.is_open()) {
            success_count.fetch_add(1, std::memory_order_relaxed);
            file.close();
        } else {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cerr << "Failed to open " << filename << std::endl;
        }
    }
}

// Function to measure time with specified number of threads
double measure_time(int num_threads) {
    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};
    const int total_files = 10000;
    int files_per_thread = total_files / num_threads;

    // Start timing
    auto start_time = std::chrono::high_resolution_clock::now();

    // Launch threads
    for (int t = 0; t < num_threads; ++t) {
        int start = t * files_per_thread + 1;
        int end = (t == num_threads - 1) ? total_files : (start + files_per_thread - 1);
        threads.emplace_back(open_files_range, start, end, std::ref(success_count));
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    // End timing
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Verify all files were opened
    if (success_count != total_files) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cerr << "Warning: Only " << success_count << " out of " << total_files 
                  << " files were opened with " << num_threads << " threads" << std::endl;
    }

    return static_cast<double>(duration.count()) / 1000.0; // Convert to milliseconds
}

// Function to calculate median of a vector of doubles
double calculate_median(std::vector<double>& times) {
    if (times.empty()) return 0.0;
    
    std::sort(times.begin(), times.end());
    size_t size = times.size();
    
    if (size % 2 == 0) {
        // If even number of elements, average the two middle values
        return (times[size / 2 - 1] + times[size / 2]) / 2.0;
    } else {
        // If odd, take the middle value (10 runs -> index 4)
        return times[size / 2];
    }
}

int main() {
    // Create files if they don't exist
    create_files();

    // Array of thread counts to test
    std::vector<int> thread_counts = {1, 2, 4, 8, 16};
    const int num_measurements = 20;

    // Measure time for each thread count
    for (int threads : thread_counts) {
        std::vector<double> times;

        // Run measurement 10 times
        for (int i = 0; i < num_measurements; ++i) {
            double time_taken = measure_time(threads);
            times.push_back(time_taken);
        }

        // Calculate and report median time
        double median_time = calculate_median(times);
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Median time to open 1000 files with " << threads 
                  << " thread(s) over " << num_measurements << " runs: " 
                  << median_time << " ms" << std::endl;
    }

    return 0;
}