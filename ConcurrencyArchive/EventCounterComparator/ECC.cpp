#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#include "../BenchMarker.h"

void MutexCounter(uint32_t From, uint32_t To, uint32_t& Ref, std::mutex& Mutex) {
    for (uint32_t i = From; i < To; ++i) {
        std::lock_guard<std::mutex> Lock(Mutex);
        Ref++;
    }
}

void AtomicCounter(uint32_t From, uint32_t To, std::atomic<uint32_t> & Atomic) {
    for (uint32_t i = From; i < To; ++i)
        Atomic.fetch_add(1);
}

void LocalCounter(uint32_t From, uint32_t To, uint32_t& Ref) {
    uint32_t Local = 0;
    for (uint32_t i = From; i < To; ++i) Local++;
    Ref = Local;
}

int Routine(uint32_t WorkingThreadCount, uint32_t TestCount)
{
    std::cout << "ThreadCount : " << WorkingThreadCount << " & TestCount : " << TestCount << "\n";
    uint32_t EventCount = WorkingThreadCount * TestCount;
    std::unique_ptr<BenchMarker> BM = std::make_unique<BenchMarker>();
    
    if (WorkingThreadCount == 0 || TestCount == 0 || TestCount > UINT32_MAX / WorkingThreadCount) return 0;
    
    for (int i = 0; i < 10; ++i)
    {
    BM->Start_BenchMark();
    
        std::vector<std::thread> ThreadList;
        std::mutex Mut;
        uint32_t MutexResult = 0;
    
        for (uint32_t i = 0; i < WorkingThreadCount; ++i) {
            ThreadList.emplace_back(std::thread{MutexCounter, (EventCount / WorkingThreadCount) * i, (EventCount / WorkingThreadCount) * (i + 1)
                , std::ref(MutexResult), std::ref(Mut)});
        }
        
        for (auto& th : ThreadList) th.join();
    
    BM->End_BenchMark();
    std::cout << "Sum Result : " << MutexResult << "\n";
    std::cout << "Thread Counter Result : " << BM->Get_BenchMarkResult() << "\n";
    std::cout << "Mutex Throughput : " << BM->Get_Throughput(EventCount) << " events/sec\n";
    BM->Start_BenchMark();
    
    std::vector<std::thread> AtomicThreadList;
     std::atomic<uint32_t> AtomicResult = 0;
    
    for (uint32_t i = 0; i < WorkingThreadCount; ++i) {
        AtomicThreadList.emplace_back(std::thread{
            AtomicCounter, (EventCount / WorkingThreadCount) * i, (EventCount / WorkingThreadCount) * (i + 1)
                , std::ref(AtomicResult)});
    }
        
    for (auto& th : AtomicThreadList) th.join();
    
    BM->End_BenchMark();
    std::cout << "Sum Result : " << AtomicResult << "\n";
    std::cout << "Atomic Counter Result : " << BM->Get_BenchMarkResult() << "\n";
    std::cout << "Atomic Throughput : " << BM->Get_Throughput(EventCount) << " events/sec\n";
    BM->Start_BenchMark();
    
    std::vector<std::thread> LocalThreadList;
    std::vector<uint32_t> LocalResults(WorkingThreadCount, 0);

    
    for (uint32_t i = 0; i < WorkingThreadCount; ++i) {
        LocalThreadList.emplace_back(std::thread{
            LocalCounter, (EventCount / WorkingThreadCount) * i, (EventCount / WorkingThreadCount) * (i + 1)
, std::ref(LocalResults[i])
        });
    }
    uint32_t LocalResult = 0;
    for (auto& th : LocalThreadList) th.join();
    
    for (auto& Res : LocalResults) LocalResult += Res;
    
    BM->End_BenchMark();
    std::cout << "Sum Result : " << LocalResult << "\n";
    std::cout << "Normal Counter Result : " << BM->Get_BenchMarkResult() << "\n";
    std::cout << "Local Throughput : " << BM->Get_Throughput(EventCount) << " events/sec\n";
    if (LocalResult == EventCount && AtomicResult == EventCount && MutexResult == EventCount) 
        std::cout << "All Result are Same." << "\n";
    
    }
}

int main() {
    Routine(1, 10000);
    Routine(1, 100000);
    Routine(1, 1000000);
    Routine(1, 10000000);
    Routine(1, 100000000);
    
    Routine(2, 10000);
    Routine(2, 100000);
    Routine(2, 1000000);
    Routine(2, 10000000);
    Routine(2, 100000000);
    
    Routine(4, 10000);
    Routine(4, 100000);
    Routine(4, 1000000);
    Routine(4, 10000000);
    Routine(4, 100000000);
    
    Routine(8, 10000);
    Routine(8, 100000);
    Routine(8, 1000000);
    Routine(8, 10000000);
    Routine(8, 100000000);
    
    std::cout << "Process End";
    
    return 0;
}