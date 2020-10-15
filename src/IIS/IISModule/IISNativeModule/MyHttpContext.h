#pragma once

#define IISNM_STATIC_CONTENT_SECTION                L"system.webServer/staticContent"
#define IISNM_NOT_FOUND                             -2

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class CHttpConnectionStoredContext;

class MyHttpContext
{
private:
	IHttpContext * pHttpContext;
	IHttpConnection * pHttpConnection;
	CHttpConnectionStoredContext * pConnectionStoredContext;
	char *  pszPhysicalPath;
	char *  pFileContent;
	char *  pResponseHeaderContentType;
	DWORD  physicalPathLength;
	DWORD  fileContentLength;
	char * pFileLastWriteTime;
	char * pIfModifiedSince;
	DWORD  requestRangeStart;
	DWORD  requestRangeEnd;
	DWORD  m_buffer_index;
	ifstream m_file;

public:
	bool HttpConnectionDisconnected;
	DWORD m_RangeSize;
	DWORD m_requestMode;
	thread t;

	//
	// Constructor
	//
	MyHttpContext(IHttpContext * pHttpContext)
		: pHttpContext(pHttpContext),
		HttpConnectionDisconnected(false),
		pszPhysicalPath(NULL),
		pFileContent(NULL),
		pResponseHeaderContentType(NULL),
		pIfModifiedSince(NULL),
		physicalPathLength(0),
		fileContentLength(-1),
		requestRangeStart(-1),
		requestRangeEnd(-1),
		m_buffer_index(0),
		m_requestMode(0),
		m_RangeSize(-1)
	{
	}

	// 
	// Destructor
	//
	~MyHttpContext()
	{
	}

	HRESULT Initialize();
	char * GetPhysicalPathA();
	DWORD GetFileContentLength();
	char * GetFileLastWriteTime();
	char * GetRequestIfModifiedSince();
	DWORD GetRequestRangeStart();
	DWORD GetRequestRangeEnd();
	DWORD GetFileRangeSize(DWORD startIndex, DWORD endIndex);
	char * GetFileContentA(DWORD startIndex, DWORD endIndex, __out BOOL * pfCompletionExpected);
	char * GetResponseHeaderContentTypeA();
	HRESULT WriteResponseMessage(PCSTR pszValue, DWORD valueLength);
	HRESULT SetResponseHeader(char* headerName, char* headerValue, DWORD headerValueLength, BOOL fReplace);
	REQUEST_NOTIFICATION_STATUS WriteResponseMessageAsync();
};