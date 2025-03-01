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
#include <queue>
#include <future>
#include <functional>
#include <condition_variable>

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

void create_files() {
    create_tmp_directory();
    for (int i = 1; i <= 10000; ++i) {
        std::string filename = "tmp/" + std::to_string(i) + ".txt";
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

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
        using return_type = typename std::invoke_result<F, Args...>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

double measure_time(int num_threads) {
    ThreadPool pool(num_threads);
    std::atomic<int> success_count{0};
    const int total_files = 10000;
    std::vector<std::future<void>> results;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= total_files; ++i) {
        results.emplace_back(pool.enqueue([i, &success_count]() {
            std::string filename = "tmp/" + std::to_string(i) + ".txt";
            std::ifstream file(filename);
            if (file.is_open()) {
                success_count.fetch_add(1, std::memory_order_relaxed);
                file.close();
            } else {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cerr << "Failed to open " << filename << std::endl;
            }
        }));
    }

    for (auto& result : results) {
        result.get();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    if (success_count != total_files) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cerr << "Warning: Only " << success_count << " out of " << total_files
                  << " files were opened with " << num_threads << " threads" << std::endl;
    }

    return static_cast<double>(duration.count()) / 1000.0;
}

double calculate_median(std::vector<double>& times) {
    if (times.empty()) return 0.0;
    
    std::sort(times.begin(), times.end());
    size_t size = times.size();
    
    if (size % 2 == 0) {
        return (times[size / 2 - 1] + times[size / 2]) / 2.0;
    } else {
        return times[size / 2];
    }
}

int main() {
    create_files();

    std::vector<int> thread_counts = {1, 2, 4, 8, 16};
    const int num_measurements = 20;

    for (int threads : thread_counts) {
        std::vector<double> times;

        for (int i = 0; i < num_measurements; ++i) {
            double time_taken = measure_time(threads);
            times.push_back(time_taken);
        }

        double median_time = calculate_median(times);
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Median time to open 1000 files with " << threads
                  << " thread(s) over " << num_measurements << " runs: "
                  << median_time << " ms" << std::endl;
    }

    return 0;
}
