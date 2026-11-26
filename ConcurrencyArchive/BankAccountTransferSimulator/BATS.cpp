#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

#include "../BenchMarker.h"

struct Account final {
    Account(int Value) : Money(Value) {}
    
    int Money = 0;
    std::mutex AccountMutex;
};

struct TransferRequest {
    uint32_t SrcIndex;
    uint32_t DestIndex;
    int Value;
};

#include <random>
#include <vector>

#include <algorithm>
#include <random>
#include <vector>

std::vector<TransferRequest> GenerateRequests(uint64_t EventCount, uint32_t AccountCount, uint32_t HotAccountRatio) {
    std::vector<TransferRequest> RequestList;
    RequestList.reserve(EventCount);

    std::mt19937 Generator(42);

    // 0번 계좌를 제외한 일반 계좌
    std::uniform_int_distribution<uint32_t> NormalDist(1, AccountCount - 1);

    std::uniform_int_distribution<int> DirectionDist(0, 1);

    const uint64_t HotCount = EventCount * HotAccountRatio / 100;

    for (uint64_t i = 0; i < EventCount; ++i) {
        uint32_t Src;
        uint32_t Dest;

        if (i < HotCount) {
            // 핫계좌를 포함한 이체
            Src = NormalDist(Generator);
            Dest = 0;

            // 출금과 입금 방향을 무작위로 결정
            if (DirectionDist(Generator) == 1)
                std::swap(Src, Dest);
        }
        else {
            // 일반 계좌끼리 이체
            Src = NormalDist(Generator);
            Dest = NormalDist(Generator);

            while (Src == Dest) Dest = NormalDist(Generator);
        }

        RequestList.push_back({Src, Dest, 1});
    }

    // 핫계좌 요청이 앞부분에만 집중되지 않도록 섞기
    std::shuffle(
        RequestList.begin(),
        RequestList.end(),
        Generator
    );

    return RequestList;
}

std::mutex GlobalMutex;
std::atomic<int> Success = 0, Failure = 0;

void TransferBalance(Account* Dest, Account* Src, int Value) {
    if (Value <= 0) {
        std::cout << "Value must be positive.\n";
        Failure.fetch_add(1);
        return;
    }

    if (Dest->Money > std::numeric_limits<int>::max() - Value) {
        std::cout << "Balance overflow.\n";
        Failure.fetch_add(1);
        return;
    }

    if (Src->Money < Value) {
        std::cout << "Insufficient balance.\n";
        Failure.fetch_add(1);
        return;
    }

    Dest->Money += Value;
    Src->Money -= Value;

    Success.fetch_add(1);
}

void BankTransferGlobal(Account* Dest, Account* Src, int Value) {
    if (Src == Dest) {
        Failure.fetch_add(1);
        return;
    }

    std::scoped_lock lock(GlobalMutex);
    TransferBalance(Dest, Src, Value);
}
void BankTransferPerAccount(Account* Dest, Account* Src, int Value) {
    if (Src == Dest) {
        Failure.fetch_add(1);
        return;
    }

    std::scoped_lock lock(Src->AccountMutex, Dest->AccountMutex);
    TransferBalance(Dest, Src, Value);
}
void SelfTransfer(Account* Acc) {
    BankTransferGlobal(Acc, Acc, 10000);
}

void OutOfMoney(Account* Dest, Account* Src) {
    BankTransferGlobal(Dest, Src, 1000000);
}

int64_t Get_SumAllAccount(const std::vector<std::unique_ptr<Account>>& AccountList) {
    int64_t Sum = 0;
    for (const auto& Acc : AccountList) {
        if (Acc.get()->Money < 0) std::abort();
        Sum += Acc->Money;
    }
    
    return Sum;
}

template <auto T>
void ThreadWork(const std::vector<std::unique_ptr<Account>>& AccountList, const std::vector<TransferRequest>& RequestList, uint64_t Begin, uint64_t End) {
    for (uint64_t i = Begin; i < End; ++i) {
        const auto& Req = RequestList[i];
        T(AccountList[Req.DestIndex].get(), AccountList[Req.SrcIndex].get(), Req.Value);
    }
}

template <auto T>
void Work(const char* Name, uint64_t EventCount, uint32_t AccountCount, uint32_t ThreadCount, uint32_t HotAccountRatio) {
    Success.store(0);
    Failure.store(0);
    
    constexpr int  DefaultBalance  = 100'000;
    
    std::cout << "\n" << "======================== " << Name << " ========================" << "\n";
    std::cout << "EventCount : " << EventCount << ", AccountCount : " << AccountCount << "\nThreadCount" << ThreadCount << ", HotAccountRatio : " << HotAccountRatio << "%\n";
    
    std::vector<std::unique_ptr<Account>> AccountList;
    AccountList.reserve(AccountCount);
    for (uint16_t i = 0; i < AccountCount; ++i)
        AccountList.push_back(std::make_unique<Account>(DefaultBalance));
    
    std::vector<std::thread> ThreadList;
    ThreadList.reserve(ThreadCount);
    
    const uint64_t WorkCount = EventCount / ThreadCount;
    
    std::vector<TransferRequest> RequestList = GenerateRequests(EventCount, AccountCount, HotAccountRatio);
    
    uint64_t BeforeSum = Get_SumAllAccount(AccountList);
    std::cout << "Current All Account Sum : " << BeforeSum << '\n';
    
    BenchMarker BM;
    BM.Start_BenchMark();
    {
        for (uint64_t i = 0; i < ThreadCount; ++i) {
            const uint64_t Begin = WorkCount * i;
            const uint64_t End = (i == ThreadCount - 1) ? EventCount : Begin + WorkCount;
            ThreadList.emplace_back(ThreadWork<T>, std::cref(AccountList), std::cref(RequestList), Begin, End);
        }
        
        for (auto& Thread : ThreadList) {
            Thread.join();
        }
    }
    BM.End_BenchMark();
    
    const uint64_t AfterSum = Get_SumAllAccount(AccountList);
    
    if (AfterSum != BeforeSum || Success.load() != EventCount || Failure.load() != 0) {
        std::cerr << "Benchmark Validation Failed.\n";
        std::abort();
    }
    
    BM.Print_BenchMarkResult();
    BM.Print_BenchMarkThroughput(EventCount);
    
    std::cout << "Current All Account Sum : " << AfterSum << '\n';
    
    std::cout << "Transfer Success : " << Success << ", Transfer Failure : " << Failure << "\n";
    std::cout << "\n" << "=================================================================" << "\n";
}

void Simulator() {
    constexpr uint64_t EventCounts[]     = {10'000, 100'000};
    constexpr uint32_t AccountCounts[]   = {4, 10, 100};
    constexpr uint32_t ThreadCounts[]    = {1, 2, 4, 8};
    constexpr uint32_t HotAccountRatios[] = {0, 30, 60, 90};
    
    for (auto EventCount : EventCounts) {
        for (auto AccountCount : AccountCounts) {
            for (auto ThreadCount : ThreadCounts) {
                for (auto HotAccountRatio : HotAccountRatios) {
                    Work<BankTransferGlobal>("Work_Global", EventCount, AccountCount, ThreadCount, HotAccountRatio);
                    Work<BankTransferPerAccount>("Work_PerAccount", EventCount, AccountCount, ThreadCount, HotAccountRatio);
                }
            }
        }
    }
}

int main() {
    Simulator();
    
    return 0;
}