#include <iostream>
#include <random>
#include <chrono>

double formula1(double x)
{
    return x * x - x * x + x * 4 - x * 5 + x + x;
}

double formula2(double x)
{
    return x + x;
}

double measure_time(double x, size_t num_loops)
{
    volatile double result = 0.0;

    auto time_start = std::chrono::high_resolution_clock::now();

    for (size_t j_loop = 0; j_loop < num_loops; ++j_loop)
    {
        // немного меняем x, чтобы компилятор не схлопнул цикл
        double current_x = x + j_loop * 0.000001;

        double val1 = formula1(current_x);
        double val2 = formula2(current_x);

        result += val1 + val2 - val1;
    }

    auto time_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = time_end - time_start;

    // чтобы result точно использовался
    std::cout << " result: " << result << "\n";

    return elapsed.count();
}

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double x = dis(gen);

    size_t num_loops1 = 10000;
    size_t num_loops2 = 100000;

    double time1 = measure_time(x, num_loops1);

    std::cout << time1 * 1000
              << " ms spent on "
              << num_loops1
              << " loops\n";

    double time2 = measure_time(x, num_loops2);

    std::cout << time2 * 1000
              << " ms spent on "
              << num_loops2
              << " loops\n";

    return 0;
}