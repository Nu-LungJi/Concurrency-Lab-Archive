# C++ Concurrency Lab

C++20까지의 동시성 도구로 배우는 멀티스레드 프로젝트 커리큘럼입니다.

**초급 10개** · **중급 7개** · **고급 선택** · 하루 2~3시간

<a id="contents"></a>

## 목차

- [시작하기](#start)
- [대상과 최종 목표](#overview)
- [통합 원칙](#principles)
- [일정](#schedule)
- [매 프로젝트의 작업 순서](#workflow)
- [공통 검증 기준](#validation)
- [공통 벤치마크 규약](#benchmark)
- [학습 점검 지점](#checkpoints)
- [결과물 구조](#deliverables)
- [C++20까지의 도구 배치](#tools)
- [원본 48개 항목의 반영 위치](#mapping)
- [참고 자료 사용법](#references)

---

<a id="start"></a>

## 시작하기

| 과정    | 구성                | 바로가기                                         |
| ----- | ----------------- | -------------------------------------------- |
| 초급    | 10개 프로젝트 · 각 3~5일 | [초급 목차](MTH_BeginnerProject.md#projects)     |
| 중급    | 7개 프로젝트 · 각 7~10일 | [중급 목차](MTH_IntermediateProject.md#projects) |
| 고급·응용 | 관심 분야별 선택         | [선택 목록](MTH_AdvancedOptions.md#tracks)       |

**처음 시작한다면:** [B01 · 병렬 배열 통계 계산기](MTH_BeginnerProject.md#b01)부터 진행하세요.

<a id="overview"></a>

## 대상과 최종 목표

C++ 문법·STL·OOP는 알고 있지만 스레드 구현 경험은 없는 학습자를 위한 과정이다. 초급에서는 스레드의 실행·공유·대기·종료를 작은 프로그램으로 익힌다. 중급에서는 직접 만든 큐와 스레드 풀로 실제 작업을 수행하고, 마지막에 의존성 기반 미니 Job System으로 통합한다.

필수 과정의 목표는 다음 네 가지다.

1. 어떤 데이터를 누가 소유하고 어떤 접근을 동기화해야 하는지 설명한다.
2. 정상 결과뿐 아니라 실패·대기·종료까지 처리한다.
3. 같은 조건에서 올바른 구현들을 비교해 병목을 분석한다.
4. 작은 모듈을 후속 프로젝트에 재사용한다.

<a id="principles"></a>

## 통합 원칙

기존 MTH 문서의 검증 기준과 프로젝트 아이디어를 초급 10개·중급 7개의 필수 과정으로 재구성했다. 기존 48개 항목 중 심화 주제는 선택 과정으로 분리했다. 원본 P 번호는 대응표에 보존하고, 필수 과정은 B01~B10과 M01~M07로 구분한다.

- 기존 파일에서 유지: 불변식, 소유권, 종료 계약, 실패 검증, 반복 측정과 원자료.
- 학습 순서 조정: 공유 쓰기 없는 작업 분할로 시작하고, 결과 전달과 중단을 별도 프로젝트로 연습한다.
- 중급 순서 조정: 스레드 풀을 먼저 만들고 검색·정렬·이미지 처리에 재사용한다.
- 범위 축소: 초급의 범용 타입 지원, 스핀락·메모리 순서 심화, 그래픽스·서버 응용은 선택으로 분리한다.
- 고급 선택: 중급을 마친 뒤 관심과 필요에 따라 고른다. 고급을 하지 않아도 필수 과정은 완결된다.

<a id="schedule"></a>

## 일정

하루 2~3시간의 집중 학습을 가정한다. 일수는 구현·검증·벤치마크·정리를 포함한 계획값이며, 완료를 보장하는 마감일은 아니다. 개발 환경 설정과 예상 밖의 디버깅에는 별도 여유를 둔다.

| 단계 | 개수 | 프로젝트당 기간 | 상세 카드의 권장 합계 |
|---|---:|---|---:|
| 초급 | 10 | 3~5학습일 | 41학습일 |
| 중급 | 7 | 7~10학습일 | 61학습일 |
| 전체 필수 | 17 | — | 102학습일 |

총 약 204~306시간이다. 주 5일 학습하면 약 20~21주이며, 휴식·환경 설정·추가 디버깅은 별도다. B05·B09·B10과 M01·M06·M07에 상대적으로 여유를 배정했다.

| 순서 | 프로젝트 | 권장 일수 |
|---|---|---:|
| [**B01**](MTH_BeginnerProject.md#b01) | [병렬 배열 통계 계산기](MTH_BeginnerProject.md#b01) | 3 |
| [**B02**](MTH_BeginnerProject.md#b02) | [이벤트 카운터 비교기](MTH_BeginnerProject.md#b02) | 4 |
| [**B03**](MTH_BeginnerProject.md#b03) | [은행 계좌 이체 시뮬레이터](MTH_BeginnerProject.md#b03) | 4 |
| [**B04**](MTH_BeginnerProject.md#b04) | [스레드 안전 스택](MTH_BeginnerProject.md#b04) | 4 |
| [**B05**](MTH_BeginnerProject.md#b05) | [용량 제한 생산자·소비자 큐](MTH_BeginnerProject.md#b05) | 5 |
| [**B06**](MTH_BeginnerProject.md#b06) | [공유 설정 저장소](MTH_BeginnerProject.md#b06) | 4 |
| [**B07**](MTH_BeginnerProject.md#b07) | [제한된 자원 대여소](MTH_BeginnerProject.md#b07) | 3 |
| [**B08**](MTH_BeginnerProject.md#b08) | [비동기 계산 결과 수집기](MTH_BeginnerProject.md#b08) | 4 |
| [**B09**](MTH_BeginnerProject.md#b09) | [중단 가능한 백그라운드 작업자](MTH_BeginnerProject.md#b09) | 5 |
| [**B10**](MTH_BeginnerProject.md#b10) | [비동기 파일 로거](MTH_BeginnerProject.md#b10) | 5 |
| [**M01**](MTH_IntermediateProject.md#m01) | [결과를 반환하는 스레드 풀](MTH_IntermediateProject.md#m01) | 10 |
| [**M02**](MTH_IntermediateProject.md#m02) | [잠금을 분할한 인메모리 저장소](MTH_IntermediateProject.md#m02) | 8 |
| [**M03**](MTH_IntermediateProject.md#m03) | [병렬 파일 검색기](MTH_IntermediateProject.md#m03) | 7 |
| [**M04**](MTH_IntermediateProject.md#m04) | [병렬 병합 정렬](MTH_IntermediateProject.md#m04) | 8 |
| [**M05**](MTH_IntermediateProject.md#m05) | [병렬 이미지 필터와 반복 처리](MTH_IntermediateProject.md#m05) | 8 |
| [**M06**](MTH_IntermediateProject.md#m06) | [용량 제한 데이터 처리 파이프라인](MTH_IntermediateProject.md#m06) | 10 |
| [**M07**](MTH_IntermediateProject.md#m07) | [의존성 기반 미니 Job System](MTH_IntermediateProject.md#m07) | 10 |

<a id="workflow"></a>

## 매 프로젝트의 작업 순서

1. **작은 명세:** 입력·결과·공유 상태·소유권·오류·종료 규칙을 먼저 적는다.
2. **기준 구현:** 순차 버전이나 가장 단순한 올바른 구현을 만든다.
3. **병렬 구현:** 필수 기능만 완성한다. 선택 확장은 아직 추가하지 않는다.
4. **정확성 검증:** 결과·경계·경쟁·실패·종료를 확인한다.
5. **측정:** Release 빌드에서 같은 입력으로 반복 비교한다.
6. **분석:** 빨라지거나 느려진 이유와 적용 한계를 기록한다.

초급은 첫 1일에 명세와 기준 구현, 중간에 병렬 구현, 마지막 1~2일에 검증·측정을 배치한다. 중급은 첫 1~2일 설계, 다음 3~4일 구현, 나머지 기간 검증·측정에 사용한다. 복잡한 오류가 남으면 확장을 줄인다. 안전성 검증을 생략해 일수를 맞추지 않는다.

<a id="validation"></a>

## 공통 검증 기준

| 영역 | 필수 확인 | 해석 시 주의 |
|---|---|---|
| 결과 | 기준 결과, ID 집합, 순서 또는 불변식과 비교 | 최종값 일치만으로 모든 동시 실행의 안전성이 증명되지는 않음 |
| 수명 | 캡처 대상·버퍼·큐가 사용 중 살아 있음, 종료 시 합류 | detach나 임의의 sleep으로 수명 문제를 덮지 않음 |
| 대기 | 빈 상태·포화 상태·처리 중 상태에서 종료 | 대기 조건과 깨우는 주체를 설명해야 함 |
| 실패 | 예외·접수 거절·파일 실패 등 해당 상황의 최종 상태 | 실행하지 않은 작업을 성공으로 집계하지 않음 |
| 스트레스 | 고정 시드 여러 개, 작은 용량, 여러 스레드 수 | 미관측 오류가 없다는 것과 오류가 불가능하다는 것은 다름 |
| 진단 도구 | 지원 환경에서 데이터 레이스·메모리 오류 검사 | 미실행은 사유를 기록하고 통과로 표시하지 않음 |

데이터 레이스가 있는 버전은 필요할 때 별도 진단용 실행 파일로 분리한다. 성능 비교에는 올바른 구현만 사용한다. sleep은 부하나 지연을 만드는 용도로만 쓰며, 올바른 실행 순서를 보장하는 수단으로 쓰지 않는다.

초급에서는 단순한 데이터와 잠금으로 안전성 근거를 설명한다. 중급 컨테이너는 짧은 경쟁 사례의 호출·반환 기록을 추가해 API 규칙과 모순되지 않는지 검토한다. 자동 선형화 검사기 구현은 선택이다.

<a id="benchmark"></a>

## 공통 벤치마크 규약

| 항목 | 규칙 |
|---|---|
| 환경 | CPU·논리 프로세서 수·OS·컴파일러·표준 라이브러리·빌드 옵션 기록 |
| 빌드 | 같은 최적화 옵션의 Release 빌드. 진단 도구 빌드와 분리 |
| 입력 | 고정된 데이터·시드·작업 수. 결과와 정확도 계약도 동일하게 유지 |
| 반복 | 준비 실행 3회, 측정 실행 10회부터 시작. 변동이 크면 원인부터 조사 |
| 실행 순서 | 비교 구현의 순서를 교차하거나 섞어 온도·캐시 편향 완화 |
| 시간 | 경과 시간은 steady_clock. 생성·초기화·입출력 포함 여부 명시 |
| 최적화 제거 | 계산 결과를 실제로 사용하고, 정확성 검사는 측정 구간 밖에서 수행 |
| 스레드 수 | 1/2/4/8… 중 장비에 맞는 값. 초과 할당 여부 기록 |
| 통계 | 실행시간 중앙값과 범위. 요청별 지연은 충분한 개별 표본으로 분위수 계산 |
| 공정성 | 성공·실패·거절·취소 수를 함께 기록하고 완료 의미를 맞춤 |

- Speedup(p) = 순차 기준 시간 / p개 작업 스레드의 병렬 시간.
- Efficiency(p) = Speedup(p) / p. CPU 사용률과 다른 지표다.
- 별도 순차 알고리즘 대비와 같은 병렬 구현의 worker=1 대비를 구분한다.
- 실행 전체를 10번 측정한 최대값을 요청 p99라고 부르지 않는다.
- 비동기 프로그램은 제출 완료 시간과 모든 작업의 최종 완료 시간을 구분한다.
- 파일 작업은 OS 캐시 영향을 기록한다. 통제하지 않은 첫 실행을 확정적인 cold-cache 실험으로 표현하지 않는다.
- 더 느린 결과도 정합성과 원인 분석이 충실하면 완료할 수 있다.

### 측정의 단계

- B01~B04: 경과 시간·처리량·스레드 수 변화.
- B05~B10: 필요한 경우 큐 대기·자원 대기·중단 응답·전체 완료 시간 추가.
- M01~M07: 프로젝트의 질문에 맞춰 요청 지연 분포·큐 깊이·메모리·작업 분배 추가.

모든 지표를 모든 프로젝트에 강제하지 않는다. CPU 사용량이나 문맥 전환을 측정했다면 도구와 단위를 적는다. 측정하지 못한 항목은 NA로 남긴다.

<a id="checkpoints"></a>

## 학습 점검 지점

**B05 완료 후:** 큐의 상태, 기다리는 조건, close가 깨워야 할 스레드를 코드 없이 설명한다. 어렵다면 B06 진행 전에 큐를 다시 검토한다.

**M01 완료 후:** 작업 접수 시점, 예외 전달, 종료와 submit의 경쟁, 모든 future의 최종 상태를 설명한다. 풀 내부에서 다른 풀 작업의 결과를 기다리는 위험을 이해한다.

**M07 완료 후:** 같은 작업 그래프의 순차·병렬 결과와 실행 순서 제약을 검증하고, 작은 작업에서 관리 비용이 커지는 이유를 분석한다.

<a id="deliverables"></a>

## 결과물 구조

```text
CppConcurrencyLab/
├── beginner/B01_ParallelStats/ ... B10_AsyncLogger/
├── intermediate/M01_ThreadPool/ ... M07_JobSystem/
├── common/        # 검증된 Queue, ThreadPool, 측정 보조 도구
├── tests/
├── results/
└── CMakeLists.txt
```

처음부터 범용 라이브러리를 설계하지 않는다. B05 큐가 B10에서 필요해질 때 공통 모듈로 옮기고, 기존 검증을 유지한다. M01 풀도 같은 방식으로 재사용한다.

각 프로젝트는 README, 소스, 검증 코드, 측정 원자료 CSV, 짧은 분석을 남긴다. 초급 분석은 한 페이지 이내로 충분하다. 보고서 분량보다 재현 가능한 근거를 우선한다.

<a id="tools"></a>

## C++20까지의 도구 배치

| 도구 | 학습 위치 |
|---|---|
| thread, join, mutex, lock_guard | B01~B04 |
| scoped_lock | B03 |
| atomic, fetch_add | B02, 이후 상태·통계 관리 |
| condition_variable, unique_lock | B05 |
| shared_mutex, shared_lock | B06, M02 |
| async, future, promise | B08 |
| packaged_task | M01 |
| counting_semaphore (C++20) | B07, M06의 비교 확장 |
| jthread, stop_token, stop_source (C++20) | B09, 이후 종료 처리 |
| stop_token을 받는 condition_variable_any 대기 (C++20) | B09 |
| barrier (C++20) | M05 |
| latch (C++20) | M07 |
| atomic::wait/notify (C++20) | 필수 과정 이후 대기 전략 선택 실습 |

메모리 순서 심화나 모든 동시성 API의 사용을 수료 조건으로 삼지 않는다. 초급 atomic은 기본 메모리 순서를 사용한다. C++23 전용 기능을 필수 구현에 섞지 않는다.

<a id="mapping"></a>

## 원본 48개 항목의 반영 위치

<details>
<summary>원본 P01~P48 대응표 펼치기</summary>

원본의 제목과 식별자를 보존한 대응표다. '선택'은 필수 17개에 추가된 의무 프로젝트가 아니다.

| 원본 ID·주제 | 통합 위치 또는 처리 |
|---|---|
| P01 카운터 | B01에서 생성·합류 분리, B02에서 공유 카운터 |
| P02 은행 이체 | B03 |
| P03 Mutex vs Atomic | B02 |
| P04 Spinlock | 선택: 메모리 모델·대기 전략 |
| P05 Thread-Safe Stack | B04, 범용 타입·예외 심화는 선택 |
| P06 생산자·소비자 큐 | B05, 시스템 통합은 M06 |
| P07 철학자들의 식사 | B03 선택 실습 |
| P08 Readers-Writers | B06 |
| P09 주차장 관리 | B07 |
| P10 동시 실행 제한 | B07의 자원 제한, B08·M01의 결과·실행 관리로 분리 |
| P11 비동기 로그 | B10 |
| P12 Hash Map | M02 |
| P13 Concurrent Linked List | 선택: 컨테이너·수명 |
| P14 SPSC Ring Buffer | 선택: 메모리 모델 |
| P15 Lock-Free Stack | 선택: 메모리 회수 |
| P16 Lock-Free Queue | 선택: 메모리 회수 |
| P17 MPMC Bounded Queue | 선택: 고급 컨테이너 |
| P18 파일 검색 | M03 |
| P19 Merge Sort | M04 |
| P20 BFS | 선택: 병렬 알고리즘 |
| P21 Dijkstra | 선택: 병렬 알고리즘 |
| P22 이미지 처리 | M05 |
| P23 Thread Pool | M01로 앞당김 |
| P24 스케줄러 시뮬레이터 | 선택: 스케줄링 정책 |
| P25 Job System | M07, 정적 DAG 범위로 축소 |
| P26 Work Stealing | 선택: 스케줄링 |
| P27 파티클 | 선택: 게임·그래픽스 |
| P28 AABB 충돌 | 선택: 게임·그래픽스 |
| P29 A* 요청 처리 | 선택: 게임·그래픽스 |
| P30 Frustum Culling | 선택: 게임·그래픽스 |
| P31 Terrain | 선택: 게임·그래픽스 |
| P32 레이트레이서 | 선택: 게임·그래픽스 |
| P33 리소스 로더 | 선택: 비동기 시스템 |
| P34 애니메이션·다중 버퍼 | 선택: 게임·그래픽스 |
| P35 채팅 서버 | 선택: 네트워크 |
| P36 HTTP 서버 | 선택: 네트워크 |
| P37 비동기 Echo 서버 | 선택: 네트워크 |
| P38 접속자 관리 | 선택: 네트워크 |
| P39 패킷 처리 | 선택: 네트워크 |
| P40 순위표 서버 | 선택: 네트워크 |
| P41 False Sharing | 선택: 성능 분석 |
| P42 경합 심화 | B02·M02 이후 선택 성능 실험 |
| P43 Spinlock vs Blocking | 선택: 대기 전략 |
| P44 Thread-Local Memory Pool | 선택: 메모리 관리 |
| P45 메모리 할당기 | 선택: 메모리 관리 |
| P46 큐 성능 비교 | 선택: 컨테이너 성능 분석 |
| P47 NUMA | 선택: 장비가 지원할 때 수행 |
| P48 최종 성능 최적화 | M07에서 작은 통합 경험, 전체 엔진 통합은 선택 |

추가 기초 실습인 B01의 배열 계산, B08의 결과 전달, B09의 중단 전용 실습과 M06의 파이프라인을 결합했다. 심화 주제인 격자 시뮬레이터·작업 훔치기·SPSC도 선택 목록에 보존한다.

</details>

<a id="references"></a>

## 참고 자료 사용법

아래는 원본 문서에서 계승한 참고 링크다. 프로젝트 완성 코드를 복사하는 자료가 아니라 API 의미·설계·측정 방법을 확인하는 자료로 사용한다. 현재 표준 초안에는 C++20 이후 기능도 있으므로 버전 범위를 확인한다. 도구를 설치할 때는 공식 문서에서 현재 지원 환경을 확인한다.

- [C++ 동시성 표준 초안](https://eel.is/c++draft/thread)
- [데이터 레이스와 happens-before](https://eel.is/c++draft/intro.races)
- [원자 연산 메모리 순서](https://eel.is/c++draft/atomics.order)
- [C++ Core Guidelines: Concurrency](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-concurrency)
- [Google Benchmark 사용 안내](https://github.com/google/benchmark/blob/main/docs/user_guide.md)
- [Clang ThreadSanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html)

지원되는 환경에서 TSan을 사용하고, ASan 등 다른 검사와는 별도 빌드를 둔다. Windows 환경에서 필요한 검사를 실행할 수 없다면 지원되는 Linux 환경을 검토하되, 실제 사용 도구의 지원 여부를 확인한다. 진단 경고가 없다는 사실만으로 안전성 설명을 대체하지 않는다.

---

[↑ 목차](#contents) · [초급 시작 →](MTH_BeginnerProject.md#b01)
