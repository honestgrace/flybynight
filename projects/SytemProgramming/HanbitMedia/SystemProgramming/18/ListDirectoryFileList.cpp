/*
	ListDirectoryFileList.cpp
	프로그램 설명: 디렉터리 내에 존재하는 파일 리스트 출력.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc,  TCHAR* argv[])
{
   WIN32_FIND_DATA FindFileData;
   HANDLE hFind = INVALID_HANDLE_VALUE;

   TCHAR DirSpec[MAX_PATH];

   _tprintf ( _T("Insert target directory: ") );
   _tscanf( _T("%s"), DirSpec);
   _tcsncat (DirSpec, _T("\\*"), 3);

   hFind = FindFirstFile(DirSpec, &FindFileData);

   if (hFind == INVALID_HANDLE_VALUE) 
   {
      _tprintf ( _T("Invalid file handle \n") );
      return -1;
   } 
   else 
   {
      _tprintf ( _T("First file name is %s\n"), FindFileData.cFileName);
      while (FindNextFile(hFind, &FindFileData) != 0)
         _tprintf ( _T("Next file name is %s\n"), FindFileData.cFileName);
    
      FindClose(hFind);
   }

   return 0;

}
