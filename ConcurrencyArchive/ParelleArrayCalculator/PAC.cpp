#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdint>
#include <limits>

constexpr size_t MAX_ARRAY_SIZE = 1'000'000;
constexpr int64_t MAX_ELEMENT_VALUE = 1'000'000;

struct Result {
    int64_t SumResult = 0;
    int64_t MaxResult = std::numeric_limits<int64_t>::min();
    int64_t MinResult = std::numeric_limits<int64_t>::max();
    bool HasValue = false;
};

void ValidateInput(const std::vector<int64_t>& Input) {
    if (MAX_ARRAY_SIZE < Input.size()) 
        throw std::invalid_argument("Array size exceeded.");
    
    for (const auto& Value : Input) {
        if (Value > MAX_ELEMENT_VALUE || Value < -MAX_ELEMENT_VALUE) 
            throw std::invalid_argument("Array Value exceeded.");
    }
}

void ThreadCalculate(const std::vector<int64_t>& Array, size_t From, size_t To, Result& Result) {
    if (From == To) return;
    
    Result.HasValue = true;
    
    for (size_t i = From; i < To; ++i) {
        Result.SumResult += Array[i];
        Result.MaxResult = std::max(Result.MaxResult, Array[i]);
        Result.MinResult = std::min(Result.MinResult, Array[i]);
    }
}

Result Spatial_Calculator(int ThreadCount, const std::vector<int64_t>& Array) {
    if (ThreadCount <= 0) throw std::invalid_argument("ThreadCount Cannot be Zero.");
    
    int Size = static_cast<int>(Array.size());
    if (Size == 0) return Result{};
    
    ThreadCount = std::min(ThreadCount, Size);
    
    std::vector<Result> ResultList(ThreadCount);
    std::vector<std::thread> ThreadList;
    int Sep = Size / ThreadCount;
    
    for (size_t i = 0; i < ThreadCount; ++i) {
        //std::thread CalThread(ThreadCalculate, std::cref(Array), i * Sep, (i == ThreadCount - 1) ? Size : i * Sep + Sep, std::ref(ResultList[i]));
        //ThreadList.push_back(std::move(CalThread));
        int From = i * Sep;
        int To = (i == ThreadCount - 1) ? Size : i * Sep + Sep;
        ThreadList.emplace_back([&Array, &ResultList, i, From, To]() {
            ThreadCalculate(Array, From, To, ResultList[i]);
        });
    }
    
    for (auto& Thr : ThreadList) Thr.join();
    
    Result TotalResult;
    for (const auto& Res : ResultList) {
        if (!Res.HasValue) continue;
        TotalResult.SumResult += Res.SumResult;
        TotalResult.MaxResult = std::max(TotalResult.MaxResult, Res.MaxResult);
        TotalResult.MinResult = std::min(TotalResult.MinResult, Res.MinResult);
        
        TotalResult.HasValue = true;
    }
    
    return TotalResult;
}

Result Normal_Calculator(const std::vector<int64_t>& Array) {
    Result TotalResult;
    int Size = static_cast<int>(Array.size());
    
    TotalResult.HasValue = !Array.empty();
    
    for (const auto& Element :Array) {
        TotalResult.SumResult += Element;
        TotalResult.MaxResult = std::max(TotalResult.MaxResult, Element);
        TotalResult.MinResult = std::min(TotalResult.MinResult, Element);
    }
    
    return TotalResult;
}

int main() {
    std::vector<int64_t> Array;
    for (int64_t i = 0; i < 500000; ++i) Array.push_back(i);
    
    ValidateInput(Array);
    auto Start = std::chrono::steady_clock::now();
    auto SpatialResult = Spatial_Calculator(4, Array);
    auto End = std::chrono::steady_clock::now();
    
    std::cout << "Spatial Calculator Benchmark : " << std::chrono::duration<double, std::milli>(End - Start).count() << "\n";
    
    Start = std::chrono::steady_clock::now();
    auto NormalResult = Normal_Calculator(Array);
    End = std::chrono::steady_clock::now();
    
    std::cout << "Normal Calculator Benchmark : " << std::chrono::duration<double, std::milli>(End - Start).count() << "\n";
    
    std::cout << "Sum: " << SpatialResult.SumResult << "\n";

    if (SpatialResult.HasValue) {
        std::cout << "Max: " << SpatialResult.MaxResult << "\n";
        std::cout << "Min: " << SpatialResult.MinResult << "\n";
    }
    else {
        std::cout << "Max: None\n";
        std::cout << "Min: None\n";
    }
    bool IsEqual = SpatialResult.SumResult == NormalResult.SumResult && SpatialResult.HasValue == NormalResult.HasValue &&
    (!SpatialResult.HasValue || (SpatialResult.MaxResult == NormalResult.MaxResult && SpatialResult.MinResult == NormalResult.MinResult));

    std::cout << (IsEqual ? "PASS" : "FAIL");
    
    
    
    return 0;
}