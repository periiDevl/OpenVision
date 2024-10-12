#ifndef TIMER_H_CLASS
#define TIMER_H_CLASS


class Timer {
public:
    void start()
    {
        startTime = std::chrono::high_resolution_clock::now();
        endTime = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void stop()
    {
        endTime = std::chrono::high_resolution_clock::now();
        running = false;
    }

    double getElapsedSeconds()
    {
        if (running)
        {
            endTime = std::chrono::high_resolution_clock::now();
        }

        return std::chrono::duration<double>(endTime - startTime).count();
    }

    double getElapsedMilliSeconds()
    {
        if (running)
        {
            endTime = std::chrono::high_resolution_clock::now();
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    }

    double getElapsedMicroSeconds()
    {
        if (running)
        {
            endTime = std::chrono::high_resolution_clock::now();
        }

        return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    }

    long long getElapsedNanoSeconds()
    {
        if (running)
        {
            endTime = std::chrono::high_resolution_clock::now();
        }

        return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool running = false;
};


#endif