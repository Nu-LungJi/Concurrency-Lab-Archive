#pragma once
#include <chrono>
#include <iostream>

class BenchMarker {
public:
    void Start_BenchMark()  { Start = std::chrono::steady_clock::now(); }
    void End_BenchMark()    { End   = std::chrono::steady_clock::now(); }
    
    void Print_BenchMarkResult() { std::cout << std::chrono::duration<double, std::milli>(End - Start).count() << "\n";  }
    void Print_BenchMarkThroughput(uint64_t EventCount) { std::cout << Get_Throughput(EventCount) << "\n";}
    
    double Get_BenchMarkResult() const { return std::chrono::duration<double, std::milli>(End - Start).count(); }
    
    double Get_Throughput(uint64_t EventCount) const {
        double ElapsedMs = Get_BenchMarkResult();

        if (ElapsedMs <= 0.0) return 0.0;

        return static_cast<double>(EventCount) * 1000.0 / ElapsedMs;
    }
public:
    std::chrono::time_point<std::chrono::steady_clock> Start;
    std::chrono::time_point<std::chrono::steady_clock> End;
};
