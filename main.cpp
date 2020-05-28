#include <chrono>
#include "omp.h"
#include "CImg.h"
#include "geometry.hpp"
#include "data_processing.hpp"
#include "fun.hpp"
#include "autotest.hpp"
using namespace std;

int main() {

    auto start = std::chrono::system_clock::now();

    autotest();
    autotest2();
    autotest3();

    auto end = std::chrono::system_clock::now();
    int elapsed_ms = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    std::cout << "Program runtime is " << elapsed_ms << " ms\n";

    return 0;
}
