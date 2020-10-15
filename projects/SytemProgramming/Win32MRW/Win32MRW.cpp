/*
==========================================================================

Purpose:

This is a sample code that demonstrates for the following:

* Use of the Shared Memory in Win32.
* Inter-process synchronization using events in Win32.
* Solution to multiple reader and writer processes problem.

Notes:

* Run multiple instances of this program for testing
* Although, this code is a C++, it can easily be converted to C.  
  This program doesn't use any OO concept or design.
* The program includes <tchar.h> and uses generic function at many places.  
  However, this program is compiled and tested with ascii builds only. 
  The program may not compile or work for Unicode builds.
* Input strings need to be entered without spaces.  This limitation is 
  because of the use of std::cin

Author:

* Swarajya Pendharkar

Date:

* 31st January 2006

Updates:

* Minor tweaks - 1st February 2006
* Removed wrong statement - CloseHandle(g_hReadEvent); - 2nd February 2006

==========================================================================
*/

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <tchar.h>

using namespace std; //for cin/cout

#define MAX_SH_MEM_SIZE 1024   //1k, Size of Shared Memory
#define MAX_READ_PROCESSES_ALLOWED 3  
#define WAIT_TIME_OUT 100

//global variables
HANDLE  g_hReadEvent[MAX_READ_PROCESSES_ALLOWED];
TCHAR   g_szReadEventName[] = _T("My Read Event");  //Name of the read event, required for a Named Kernal Object

HANDLE  g_hWriteEvent = NULL;
TCHAR   g_szWriteEventName[] = _T("My Write Event");  //Name of the write event, required for a Named Kernal Object

HANDLE  g_hSharedMemory = NULL; //global handle to shared memory
LPTSTR  g_pBuffer = NULL;       //shared memory pointer
TCHAR   g_szShareMemoryName[] = _T("My Shared Memory");  //Name of the shared memory, required for a Named Kernal Object

//global functions
bool Initialize();
void DeInitialize();
void DisplayOptions();
bool RecvAndProcessOption();
void WriteAndPrint();
void ReadAndPrint();

int main(int argc, char* argv[])
{
     if (Initialize())
     {
          cout << "Initialization of the process was successful\n";
     }
     else
     {
          DeInitialize();
          cout << "Initialization of the process was not successful\n";
          return 1; //error
     }
     
     bool bContinue = true;
     
     while(bContinue)
     {
          DisplayOptions();
          bContinue = RecvAndProcessOption();
     }
     
     DeInitialize();
     
     return 0; //success
}

bool Initialize()
{
     //Trying to be defensive
     for (int ii = 0; ii < MAX_READ_PROCESSES_ALLOWED; ii++)
     {
          g_hReadEvent[ii] = NULL;
     }
     
     TCHAR szBuffer[32];  //big enough to hold the name
     
     //Creating Read events
     for (int ii = 0; ii < MAX_READ_PROCESSES_ALLOWED; ii++)
     {
          _stprintf(szBuffer, _T("%s %d"), g_szReadEventName, ii);
          
          //Creates or opens event depending, on whether already exists or not
          g_hReadEvent[ii] = CreateEvent( 
               NULL,             //default security
               false,            //auto reset
               true,             //default state signaled
               szBuffer);
          
          if (NULL == g_hReadEvent[ii])
          {
               return false;
          }
     }
     
     //Write Event
     //Creates or opens event, depending on whether already exists or not
     g_hWriteEvent = CreateEvent( 
          NULL,             //default security
          false,            //auto reset
          true,             //default state signaled
          g_szWriteEventName);
     
     if (NULL == g_hWriteEvent)
     {
          return false;
     }
     
     //Shared Memory Stuff
     //Creates or opens shared memory, depending on whether already exists or not
     g_hSharedMemory = CreateFileMapping(
          INVALID_HANDLE_VALUE,    // use paging file
          NULL,                    // default security 
          PAGE_READWRITE,          // read/write access
          0,                       // max. object size 
          MAX_SH_MEM_SIZE,         // buffer size  
          g_szShareMemoryName);    // name of mapping object
     
     if (NULL == g_hSharedMemory || INVALID_HANDLE_VALUE == g_hSharedMemory) 
     { 
          cout << "Error occured while creating file mapping object :" << GetLastError() << "\n";
          return false;
     }
     
     g_pBuffer = (LPTSTR) MapViewOfFile(g_hSharedMemory,   // handle to map object
          FILE_MAP_ALL_ACCESS, // read/write permission
          0,                   
          0,                   
          MAX_SH_MEM_SIZE);           
     
     if (NULL == g_pBuffer) 
     { 
          cout << "Error occured while mapping view of the file :" << GetLastError() << "\n";
          return false;
     }
     
     return true;
}

void DeInitialize()
{
     for (int ii = 0; ii < MAX_READ_PROCESSES_ALLOWED; ii++)
     {
          CloseHandle(g_hReadEvent[ii]);
     }
     
     CloseHandle(g_hWriteEvent);
     
     UnmapViewOfFile(g_pBuffer);
     
     CloseHandle(g_hSharedMemory);
}

void DisplayOptions()
{
     cout << "\nOperation on Shared Memory - (Read/Write)\n";
     cout << "=========================================\n";
     cout << "Enter 1 to write to the shared memory\n";
     cout << "Enter 2 to read the shared memory\n";
     cout << "Enter 3 to exit the application\n";
     cout << "Enter option: ";
}

bool RecvAndProcessOption()
{
     int  nInput;
     bool bReturnValue = true;
     
     cin >> nInput;
     
     switch(nInput)
     {
     case 1:
          cout << "Write Operation selected\n";
          WriteAndPrint();
          bReturnValue = true; //No need to quit
          break;
     case 2:
          cout << "Read Operation selected\n";
          ReadAndPrint();
          bReturnValue = true; //No need to quit
          break;
     case 3:
          cout << "Quit Operation selected\n";
          bReturnValue = false; //Quit!
          break;
     default:
          cout << "Invalid Operation selected\n";
          bReturnValue = true; //No need to quit
          break;
     }
     
     return bReturnValue;
}

void WriteAndPrint()
{
     cout << "Trying to write and print the shared memory...\n";
     
     cout << "Waiting for write operation to complete...\n";
     
     //Wait to make sure all writing operations are done and data is in sync
     if (WAIT_OBJECT_0 == WaitForSingleObject(g_hWriteEvent, INFINITE ))
     {
          cout << "Waiting for all read operations to complete...\n";
          
          DWORD dwWaitResult = WaitForMultipleObjects( 
               MAX_READ_PROCESSES_ALLOWED,   // number of handles in array
               g_hReadEvent,  // array of read-event handles
               TRUE,         // wait until all are signaled
               INFINITE);    // indefinite wait
          
          if (WAIT_OBJECT_0 == dwWaitResult)
          {
               cout << "Enter a string (without spaces): ";
               
               cin >> g_pBuffer;
               
               //Reading the shared memory
               cout << "Shared Memory: " << g_pBuffer << "\n";
          }
          else
          {
               cout << "Error occured while waiting :" << GetLastError() << "\n";
          }
          
          cout << "Setting the Write Event...\n";
          SetEvent(g_hWriteEvent);
          
          cout << "Setting the Read Events...\n";
          for (int ii = 0; ii < MAX_READ_PROCESSES_ALLOWED; ii++)
          {
               SetEvent(g_hReadEvent[ii]);
          }
     }
     else
     {
          cout << "Error occured while waiting :" << GetLastError() << "\n";
     }
}

void ReadAndPrint()
{
     cout << "Trying to read and print the shared memory...\n";
     
     //Wait to make sure all writing operations are done and data is in sync
     bool bContinue = true;
     
     while(bContinue)  //We want to block until we get to read
     {
          cout << "Waiting for write operation to complete...\n";
          
          DWORD dwWaitResult = WaitForSingleObject(g_hWriteEvent, INFINITE );
          
          if (WAIT_OBJECT_0 == dwWaitResult)
          {
               bool bEventFound = false;
               
               for (int ii = 0; ii < MAX_READ_PROCESSES_ALLOWED; ii++)
               {
                    DWORD dwWaitResult = WaitForSingleObject(g_hReadEvent[ii], WAIT_TIME_OUT);
                    
                    if (WAIT_OBJECT_0 == dwWaitResult)  //The state of the specified object is signaled
                    {
                         bEventFound = true;
                         
                         cout << "Setting the Write Event...\n";
                         SetEvent(g_hWriteEvent);
                         
                         //Reading the shared memory
                         cout << "Shared Memory: " << g_pBuffer << "\n";
                         
                         cout << "Setting the Read Event...\n";
                         SetEvent(g_hReadEvent[ii]);
                         
                         bContinue = false;
                         break; //get out of the for loop
                    }
                    else
                    {
                         continue; //The time-out interval elapsed, and the object's state is nonsignaled.
                    }
               } // for
               
               if (false == bEventFound)
               {
                    cout << "Setting the Write Event...\n";
                    SetEvent(g_hWriteEvent);
                    
                    //SwitchToThread();
                    Sleep(WAIT_TIME_OUT);
               }
          }
          else
          {
               cout << "Error occured while waiting :" << GetLastError() << "\n";
          }
          
     } //while
}
