#include <cassert>
#include <execution>
#include <iostream>
#include <thread>
#include <mutex>
#include <numeric>
#include <optional>
#include <stack>
#include <unordered_map>
#include <vector>

#include "../BenchMarker.h"

std::stack<uint32_t> Stack;
std::mutex Mutex;

struct IDRecord {
    uint32_t PushCount = 0;
    uint32_t PopCount = 0;
};

enum class WaitMode {
    BusyWaiting,
    Yield
};

std::unordered_map<uint32_t, IDRecord> IDCheckList;

std::atomic<bool> IsProductionDone = false;

void push(uint32_t Value) {
    std::lock_guard Lock(Mutex);
    Stack.push(Value);
    IDCheckList[Value].PushCount++;
}

std::optional<uint32_t> try_pop() {
    std::lock_guard Lock(Mutex);

    if (Stack.empty()) return std::nullopt;
        
    uint32_t Output = Stack.top();
    Stack.pop();
    
    IDCheckList[Output].PopCount++;
    return Output;
}

void ThreadWork_Push(uint32_t Begin, uint32_t End) {
    for (uint32_t i = Begin; i < End; ++i) {
        push(i);
    }
}
void ThreadWork_PushwithList(const std::vector<uint32_t>& IDList, uint32_t Begin, uint32_t End) {
    for (uint32_t i = Begin; i < End; ++i) {
        push(IDList[i]);
    }
}
void ThreadWork_TryPop(WaitMode Mode, uint64_t& EmptyRetryCount) {
    uint64_t RetryCount = 0;

    while (true) {
        const bool IsDone = IsProductionDone.load();

        auto Output = try_pop();
        if (Output.has_value()) continue;

        if (IsDone) break;

        ++RetryCount;

        if (Mode == WaitMode::Yield) std::this_thread::yield();
    }

    EmptyRetryCount = RetryCount;
}

void Work(
    uint32_t EventCount,
    uint32_t ProducerCount,
    uint32_t ConsumerCount,
    WaitMode Mode)
{
    // 공유 자원 초기화
    Stack = std::stack<uint32_t>{};
    IDCheckList.clear();
    IDCheckList.reserve(EventCount);
    IsProductionDone.store(false);

    // 실험 조건 출력
    std::cout << "\n========================================\n";

    std::cout << "EventCount    : " << EventCount << '\n';
    std::cout << "ProducerCount : " << ProducerCount << '\n';
    std::cout << "ConsumerCount : " << ConsumerCount << '\n';
    std::cout << "WaitMode      : "
              << (Mode == WaitMode::Yield
                  ? "Yield" : "Busy Waiting") << '\n';

    // 고유 ID 생성
    std::vector<uint32_t> IDList(EventCount);
    std::iota(IDList.begin(), IDList.end(), 100);

    for (const auto& Elem : IDList)
        IDCheckList.emplace(Elem, IDRecord{});

    // 스레드 및 재시도 횟수 관리
    std::vector<std::thread> ProducerList;
    std::vector<std::thread> ConsumerList;

    ProducerList.reserve(ProducerCount);
    ConsumerList.reserve(ConsumerCount);

    std::vector<uint64_t> EmptyRetryCounts(ConsumerCount, 0);

    const uint32_t WorkCount = EventCount / ProducerCount;

    // 벤치마크 시작
    const auto StartTime = std::chrono::steady_clock::now();

    // 생산자 스레드 생성
    for (uint32_t i = 0; i < ProducerCount; ++i) {
        const uint32_t Begin = WorkCount * i;

        const uint32_t End =
            (i == ProducerCount - 1)
            ? EventCount
            : Begin + WorkCount;

        ProducerList.emplace_back(
            ThreadWork_PushwithList,
            std::cref(IDList),
            Begin,
            End
        );
    }

    // 소비자 스레드 생성
    for (uint32_t i = 0; i < ConsumerCount; ++i) {
        ConsumerList.emplace_back(
            ThreadWork_TryPop,
            Mode,
            std::ref(EmptyRetryCounts[i])
        );
    }

    // 생산자 종료 대기
    for (auto& Thread : ProducerList)
        Thread.join();

    IsProductionDone.store(true);

    // 소비자 종료 대기
    for (auto& Thread : ConsumerList)
        Thread.join();

    // 벤치마크 종료
    const auto EndTime = std::chrono::steady_clock::now();

    const double ElapsedTime =
        std::chrono::duration<double>(
            EndTime - StartTime
        ).count();

    // ID 검증 및 연산 횟수 집계
    bool IsValid = true;

    uint64_t TotalPushCount = 0;
    uint64_t TotalPopCount = 0;

    for (const auto& [Key, Value] : IDCheckList) {

        TotalPushCount += Value.PushCount;
        TotalPopCount += Value.PopCount;

        // 중복 검사
        if (Value.PushCount > 1)
            std::cout << "Duplicate Push ID : " << Key << '\n';

        if (Value.PopCount > 1)
            std::cout << "Duplicate Pop ID : " << Key << '\n';

        // 누락 검사
        if (Value.PushCount == 0)
            std::cout << "Missing Push ID : " << Key << '\n';

        if (Value.PopCount == 0)
            std::cout << "Missing Pop ID : " << Key << '\n';

        if (Value.PushCount != 1 || Value.PopCount != 1)
            IsValid = false;
    }

    if (IDCheckList.size() != IDList.size())
        IsValid = false;

    if (!Stack.empty())
        IsValid = false;

    if (TotalPushCount != EventCount ||
        TotalPopCount != EventCount)
        IsValid = false;

    // 빈 스택 재시도 횟수 집계
    const uint64_t TotalEmptyRetryCount =
        std::accumulate(
            EmptyRetryCounts.begin(),
            EmptyRetryCounts.end(),
            uint64_t{0}
        );

    // 처리량 계산
    const double Throughput =
        ElapsedTime > 0.0
        ? (TotalPushCount + TotalPopCount) / ElapsedTime
        : 0.0;

    // 결과 출력
    std::cout << std::fixed << std::setprecision(3);

    std::cout << "\n--------------- Result ----------------\n";
    std::cout << "Execution Time    : "
              << ElapsedTime * 1000.0 << " ms\n";

    std::cout << "Push Count        : "
              << TotalPushCount << '\n';

    std::cout << "Pop Count         : "
              << TotalPopCount << '\n';

    std::cout << "Empty Retry Count : "
              << TotalEmptyRetryCount << '\n';

    std::cout << "Throughput        : "
              << Throughput << " ops/s\n";

    std::cout << "Remaining Count   : "
              << Stack.size() << '\n';

    std::cout << "Validation        : "
              << (IsValid ? "Success" : "Failure") << '\n';

    std::cout << "========================================\n";
}

void Simulator() {
    
    // Day 1
    {
        //for (int i = 0; i < 30; ++i) push(i);
        //
        //for (int i = 0; i < 30; ++i) {
        //    try_pop();
        //}
    }
    
    // Day 2
    {
        //std::thread TRD1{ThreadWork_Push, 0, 10};
        //std::thread TRD2{ThreadWork_Push, 10, 20};
        //std::thread TRD3{ThreadWork_Push, 20, 30};
        //
        //TRD1.join();
        //TRD2.join();
        //TRD3.join();
        //
        //std::thread TRD4{ThreadWork_TryPop, 10};
        //std::thread TRD5{ThreadWork_TryPop, 10};
        //std::thread TRD6{ThreadWork_TryPop, 10};
        //
        //TRD4.join();
        //TRD5.join();
        //TRD6.join();
    }
    
    // Day 3
    {
        Stack = std::stack<uint32_t>{};
        IDCheckList.clear();
        IsProductionDone.store(false);
        
        std::vector<uint32_t> IDList(30);
        std::iota(IDList.begin(), IDList.end(), 100);
        
        for (const auto& Elem : IDList) {
            IDCheckList.emplace(Elem, IDRecord{});
        }
        
        std::vector<uint64_t> EmptyRetryCounts(3, 0);

        constexpr WaitMode Mode = WaitMode::Yield;
        
        std::thread TRD1{ThreadWork_PushwithList, std::cref(IDList), 0, 10};
        std::thread TRD4{ThreadWork_TryPop, Mode, std::ref(EmptyRetryCounts[0])};
        std::thread TRD2{ThreadWork_PushwithList, std::cref(IDList), 10, 20};
        std::thread TRD5{ThreadWork_TryPop, Mode, std::ref(EmptyRetryCounts[1])};
        std::thread TRD6{ThreadWork_TryPop, Mode, std::ref(EmptyRetryCounts[2])};
        std::thread TRD3{ThreadWork_PushwithList, std::cref(IDList), 20, 30};
        
        TRD1.join();
        TRD2.join();
        TRD3.join();
        
        IsProductionDone.store(true);
        
        TRD6.join();
        TRD5.join();
        TRD4.join();
        
        bool IsValid = true;
        
        for (const auto& [Key, Value] : IDCheckList) {
            // 중복 검사
            if (Value.PushCount > 1) std::cout << "Duplicate Push ID : " << Key << '\n';
            if (Value.PopCount > 1)  std::cout << "Duplicate Pop ID : " << Key << '\n';

            // 누락 검사
            if (Value.PushCount == 0) std::cout << "Missing Push ID : " << Key << '\n';
            if (Value.PopCount == 0)  std::cout << "Missing Pop ID : " << Key << '\n';
            
            if (Value.PushCount != 1 || Value.PopCount != 1) {
                std::cout << "ID Validation Failure { PushCount : " << Value.PushCount
                    << ", PopCount : " << Value.PopCount << " }" << '\n';
                
                IsValid = false;
            }
        }
        
        if (IDCheckList.size() != IDList.size()) IsValid = false;

        if (!Stack.empty()) IsValid = false;

        std::cout << (IsValid ? "Validation Success" : "Validation Failure") << '\n';
    }
    
    // Day 4 
    {
        constexpr uint32_t EventCounts[] = {
            10'000,
            100'000,
            1'000'000
        };

        constexpr uint32_t ProducerCounts[] = {
            1, 2, 4, 8
        };

        constexpr uint32_t ConsumerCounts[] = {
            1, 2, 4, 8
        };

        constexpr WaitMode WaitModes[] = {
            WaitMode::BusyWaiting,
            WaitMode::Yield
        };

        for (auto EventCount : EventCounts) {
            for (auto ProducerCount : ProducerCounts) {
                for (auto ConsumerCount : ConsumerCounts) {
                    for (auto Mode : WaitModes) {
                        Work(
                            EventCount,
                            ProducerCount,
                            ConsumerCount,
                            Mode
                        );
                    }
                }
            }
        }
    }
}

int main() {
    Simulator();
    return 0;
}