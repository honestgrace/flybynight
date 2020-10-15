#include <windows.h>
#include <stdio.h>

#define ARRAY_SIZE 100
#define THREAD_NUM 4

struct data_info
{
	 int *d_point;
    int idx;
};
int *data_array;
int sum_array[THREAD_NUM];
CRITICAL_SECTION CSLock;
CONDITION_VARIABLE CalcReady;

DWORD WINAPI t_func(void *data)
{
	 int i=0;
    int sum = 0;
    struct data_info d_info;

    d_info = *((struct data_info *)data);
    EnterCriticalSection(&CSLock);
    SleepConditionVariableCS(&CalcReady, &CSLock, INFINITE);
    LeaveCriticalSection(&CSLock);

    for (i = 0; i < 25; i++)
    {
   	 sum += d_info.d_point[d_info.idx * 25];
    }

    printf("%d %d\n", d_info.idx, sum);
    sum_array[d_info.idx] = sum;
    return 0;
}

int main(int argc, char **argv)
{
	 int i = 0;
    int sum = 0;
    struct data_info d_info;

    DWORD dwThreadArray[THREAD_NUM];
    HANDLE hThreadArray[THREAD_NUM];

    InitializeCriticalSection (&CSLock);
    InitializeConditionVariable (&CalcReady);

    data_array = (int *)malloc(sizeof(int)*ARRAY_SIZE);

    for (i = 0; i < THREAD_NUM; i++)
    {
   	 d_info.d_point = data_array;
   	 d_info.idx = i;
   	 hThreadArray[i] = CreateThread (NULL, 0, t_func, (void *)&d_info, 0, &dwThreadArray[i]);
   	 Sleep(10);
    }

    for(i = 0; i < ARRAY_SIZE; i++)
    {
   	 *data_array = i;
   	 *data_array++;
    }
    WakeAllConditionVariable(&CalcReady);

    WaitForMultipleObjects( THREAD_NUM, hThreadArray, TRUE, INFINITE);
    for (i =0; i < THREAD_NUM; i++)
    {
   	 CloseHandle(hThreadArray[i]);
   	 sum += sum_array[i];
    }
    return 0;
}
