// WinAPIMultithreadingSynchronization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <math.h>
#include <vector>

const DWORD dwSleepTime = 100; // millisecond

size_t GetProcessorCount()
{
    SYSTEM_INFO si{};
    ::GetSystemInfo(&si);
    return (size_t)si.dwNumberOfProcessors;
}

typedef struct alignas (sizeof(long long))
{
    long long m_sum, m_start, m_end;
    alignas (sizeof(long long)) volatile long m_bRun;
    alignas (sizeof(long long)) CRITICAL_SECTION m_cs;
} Param, * PParam;


// Utility function for easy casting
template<typename T>
constexpr inline T& cast_ref(void* ptr)
{
    return *static_cast<T*>(ptr);
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    // Param& p = *((Param*)(lpParameter));
    // Param& p = *static_cast<Param*>(lpParameter);
    auto& p = cast_ref<Param>(lpParameter);

    while (::InterlockedExchange(&p.m_bRun, p.m_bRun))
    {
        ::EnterCriticalSection(&p.m_cs);
        if (p.m_sum == 0)
        {
            for (long long i = p.m_start; i < p.m_end; ++i)
                p.m_sum += i;
        }
        ::LeaveCriticalSection(&p.m_cs);
        ::Sleep(dwSleepTime);
    }
    return TRUE;
}

int test_2_threads()
{
    Param p{}; // initialize all members to zero

    // we have to initialize the critical sectioin before use
    ::InitializeCriticalSection(&p.m_cs);

    // we create a thread with ThreadProc as thread function.
    // initially this thread is suspended
    // if you want to start the thread, call ResumeThread()
    HANDLE hThread = ::CreateThread(NULL, 0, ThreadProc, &p, CREATE_SUSPENDED, NULL);
    p.m_bRun = TRUE;
    p.m_sum = 0;
    p.m_start = 1;
    p.m_end = 11;

    // make the hThread schedulable
    ::ResumeThread(hThread);

    long long i = 1, limit = 5;
    // while (p.m_bRun)
    // for atomic read of p.m_bRun
    while(TRUE)
    {
        ::Sleep(dwSleepTime);
        ::EnterCriticalSection(&p.m_cs);

        if (p.m_sum)
        {
            std::cout << "Sum = " << p.m_sum << std::endl;
            ++i;

            p.m_sum = 0;
            p.m_end = (long long) pow(10.0, i) + 1;
        }

        if (i >= limit)
        {
            p.m_bRun = FALSE;
            ::LeaveCriticalSection(&p.m_cs);
            break;
        }
        ::LeaveCriticalSection(&p.m_cs);
    }

    // wait until hThread returns (or terminates)
    ::WaitForSingleObject(hThread, INFINITE);

    ::DeleteCriticalSection(&p.m_cs);
    CloseHandle(hThread);
    return 0;
}

typedef struct alignas (sizeof(long long))
{
    size_t m_current_idx;
    long long m_start, m_end;
    alignas (sizeof(long long)) volatile long m_bRun;
    alignas (sizeof(long long)) CRITICAL_SECTION m_cs;
    std::vector<long long> m_sum;
} ParamFixed, * PParamFixed;


DWORD WINAPI ThreadProcFixed(LPVOID lpParameter)
{
    // ParamFixed& p = *((ParamFixed*)(lpParameter));
    // ParamFixed& p = *static_cast<ParamFixed*>(lpParameter);
    auto& p = cast_ref<ParamFixed>(lpParameter);

    long long start, end, sum;
    size_t idx, size = p.m_sum.size();

    while (::InterlockedExchange(&p.m_bRun, p.m_bRun))
    {
        ::EnterCriticalSection(&p.m_cs);

        std::cout << "Thread ID: " << GetCurrentThreadId() << std::endl;

        if (p.m_current_idx < size)
        {
            idx = p.m_current_idx;
            start = p.m_start;
            end = p.m_end;
            sum = p.m_sum[idx];

            ::LeaveCriticalSection(&p.m_cs);
        }
        else
        {
            ::LeaveCriticalSection(&p.m_cs);
            break;
        }
        
        if (sum == 0)
        {
            for (long long i = start; i < end; ++i)
                sum += i;

            long long* ptr = &p.m_sum[idx];
            ::InterlockedExchange64(ptr, sum);  // This is actually p.m_sum[idx] = sum
        }
        
        ::Sleep(dwSleepTime);
    }
    return TRUE;
}

int test_multi_threads()
{
    ParamFixed p{}; // initialize all members to zero

    // we have to initialize the critical sectioin before use
    ::InitializeCriticalSection(&p.m_cs);

    size_t size = 7;

    // we create a thread with ThreadProc as thread function.
    // initially this thread is suspended
    // if you want to start the thread, call ResumeThread()
    size_t ProcessorCount = GetProcessorCount();
    size_t ThreadCount = ProcessorCount - 1;

    std::vector<HANDLE> hThread;
    hThread.resize(ThreadCount);

    for (size_t i=0; i<ThreadCount; ++i)
        hThread[i] = ::CreateThread(NULL, 0, ThreadProcFixed, &p, CREATE_SUSPENDED, NULL);

    p.m_sum.resize(size); // allocate for the vector

    p.m_current_idx = 0;
    p.m_bRun = TRUE;
    p.m_sum[0] = 0;
    p.m_start = 1;
    p.m_end = 11;

    // make the hThread schedulable
    for (size_t i = 0; i < ThreadCount; ++i)
        ::ResumeThread(hThread[i]);
        
    // while (p.m_bRun)
    // for atomic read of p.m_bRun
    while (TRUE)
    {
        ::Sleep(dwSleepTime);
        ::EnterCriticalSection(&p.m_cs);

        if (p.m_sum[p.m_current_idx])
        {
            std::cout << "Thread ID: " << GetCurrentThreadId() << std::endl;
            ++p.m_current_idx;

            if (p.m_current_idx < size)
            {
                p.m_sum[p.m_current_idx] = 0;
                p.m_end = (long long)pow(10.0, p.m_current_idx+1) + 1;
            }
            else
            {
                p.m_bRun = FALSE;
                ::LeaveCriticalSection(&p.m_cs);
                break;
            }
        }

        ::LeaveCriticalSection(&p.m_cs);
    }

    // wait until hThread returns (or terminates)
    ::WaitForMultipleObjects(ThreadCount, &hThread[0], TRUE, INFINITE);
    ::DeleteCriticalSection(&p.m_cs);

    for (size_t i = 0; i < ThreadCount; ++i)
        CloseHandle(hThread[i]);

    for (auto& v : p.m_sum)
    {
        std::cout << v << std::endl;
    }
    return 0;
}

int main()
{
    //test_2_threads();

    test_multi_threads();

    return 0;
}