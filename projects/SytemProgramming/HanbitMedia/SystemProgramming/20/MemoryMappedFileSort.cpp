/*
    MemoryMappedFileSort.cpp
    프로그램 설명: MMF와 정렬 알고리즘.
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

void SortIntData(int * pSortArr, int num);

int main(int argc, TCHAR * argv[])
{
 HANDLE hFile = CreateFile (
  _T("data.dat"),
  GENERIC_READ|GENERIC_WRITE,
  0,
  NULL,
  CREATE_ALWAYS,
  FILE_ATTRIBUTE_NORMAL,
  NULL
  );

 if (hFile == INVALID_HANDLE_VALUE) 
  _tprintf(_T("Could not open file."));

 HANDLE hMapFile = CreateFileMapping (
  hFile,
  NULL,
  PAGE_READWRITE,
  0,
  1024 * 10,  // SIZE IS IMPORTANT! 
  NULL//_T("TEST_DAT")
  );
 if (hMapFile == NULL) 
  _tprintf(_T("Could not create map of file. %d \n"), GetLastError() );


 int * pWrite = (int *)MapViewOfFile (
  hMapFile,
  FILE_MAP_ALL_ACCESS,
  0,
  0,
  0
  );
 if (pWrite == NULL) 
  _tprintf(_T("Could not map view of file. %d \n"), GetLastError());

 pWrite[0] = 1;
 pWrite[1] = 3;
 pWrite[2] = 0;
 pWrite[3] = 2;
 pWrite[4] = 4;
 pWrite[5] = 5;
 pWrite[6] = 8;
 pWrite[7] = 6;
 pWrite[8] = 7;

 SortIntData(pWrite, 9);

 _tprintf(_T("%d %d %d \n"), pWrite[0],  pWrite[1],  pWrite[2]);
 _tprintf(_T("%d %d %d \n"), pWrite[3],  pWrite[4],  pWrite[5]);
 _tprintf(_T("%d %d %d \n"), pWrite[6],  pWrite[7],  pWrite[8]);

 UnmapViewOfFile(pWrite);
 CloseHandle(hMapFile);
 CloseHandle(hFile);

 _tprintf( _T("End of process! \n"));

 return 0;
}


void SortIntData(int * pSortArr, int num)
{
 // bubble sort
 int temp;

 for(int i=0;i<num-1;i++)
 {

  for(int j=1;j<num-i ;j++)
  {

   if(pSortArr[j-1] > pSortArr[j])
   {
    temp = pSortArr[j-1];
    pSortArr[j-1] = pSortArr[j];
    pSortArr[j] = temp;                 
   }
  }
 }

}
