
extern "C" { 
#include "nadeau.h"
}
#include <iostream>
#include <fstream>
#include <unistd.h>

struct memory_tracker {
    int64_t current;
    memory_tracker() : current(getCurrentRSS()) {}
    int64_t extra() {
        int64_t memory_gain = getCurrentRSS() - current;
        current += memory_gain;
        return memory_gain;
    }
};

int main() {
    constexpr size_t big_number = 1000;
    char** bigpointer = new char*[big_number];
    memory_tracker mt;
    for(size_t i =0; i <big_number;i++) {
        bigpointer[i]  = new char[1000*1000];
      std::cout << "extra mem " << mt.extra() << std::endl;
    }
}