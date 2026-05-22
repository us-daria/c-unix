#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END    0
#define WRITE_END	1

inline double formula1(const double x) { return x * x - x * x + x * 4 - x * 5 + x + x; }

inline double formula2(const double x) { return x + x; }

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double x = dis(gen);

    size_t tests[] = {10000, 100000};

    for (size_t num_loops : tests)
    {
        clock_t time_start = clock();

        int fd1[2];
        if (pipe(fd1) == -1) {
            std::cout << "[ERROR] pipe failed\n";
            return 1;
        }

        pid_t pid1 = fork();

        if (pid1 == -1) {
            std::cout << "[ERROR] fork failed\n";
            return 1;
        }

        else if (pid1 == 0)
        {
            close(fd1[0]);

            std::vector<double> val1(num_loops);

            for (int i = 0; i < num_loops; i++) {
                val1[i] = formula1(x);
            }

            write(fd1[1], val1.data(), val1.size() * sizeof(double));
            close(fd1[1]);
            exit(0);
        }
        else
        {
            close(fd1[1]);

            std::vector<double> val1(num_loops);
            std::vector<double> val2(num_loops);

            for (int i = 0; i < num_loops; i++) {
                val2[i] = formula2(x);
            }

            read(fd1[0], val1.data(), val1.size() * sizeof(double));
            close(fd1[0]);

            double val3;
            for (int i = 0; i < num_loops; i++) {
                val3 = val1[i] + val2[i] - val1[i];
            }

            waitpid(pid1, nullptr, 0);
        }

        clock_t time_end = clock();
        double time = (double(time_end - time_start) / CLOCKS_PER_SEC);

        std::cout << "Loops: " << num_loops
                  << " -> " << time * 1e6 << " us\n";
    }

    return 0;
}