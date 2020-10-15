#include "stdafx.h"

HRESULT MyHttpContext::Initialize()
{
	HRESULT hr = S_OK;
	
	// initialize pHttpConnection
	pHttpConnection = pHttpContext->GetConnection();
	if (pHttpConnection == NULL)
	{
		hr = HRESULT_FROM_WIN32(WSAECONNRESET);
	}
	else
	{
		// Register a new connection object in order to handle NotifyDisconnect() event.
		// This connection object are used by multiple requests which are served in the same connection while the connection is opened.
		pConnectionStoredContext = static_cast<CHttpConnectionStoredContext *> (pHttpConnection->GetModuleContextContainer()->GetConnectionModuleContext(g_pModuleId));
		if (pConnectionStoredContext == NULL)
		{
			pConnectionStoredContext = new CHttpConnectionStoredContext(this);
			pHttpConnection->GetModuleContextContainer()->SetConnectionModuleContext(pConnectionStoredContext, g_pModuleId);
		}
	}
	return hr;
}

char * MyHttpContext::GetPhysicalPathA()
{
	if (pszPhysicalPath == NULL)
	{
		PCWSTR pwszScriptTranslated;
		DWORD cbScriptTranslated = 0;
		pwszScriptTranslated = pHttpContext->GetScriptTranslated(&cbScriptTranslated);
		size_t destBufflen = 0;
		if (pwszScriptTranslated != NULL && cbScriptTranslated > 0)
		{
			//wstring filePath = pwszScriptTranslated;
			physicalPathLength = (DWORD)wcslen(pwszScriptTranslated);
			destBufflen = physicalPathLength + 1;  // null string
			pszPhysicalPath = (char *)pHttpContext->AllocateRequestMemory((DWORD) destBufflen);
			size_t length = 0;
			if (pszPhysicalPath != NULL)
			{
				wcstombs_s(&length, pszPhysicalPath, destBufflen, pwszScriptTranslated, destBufflen);
			}
			pszPhysicalPath[physicalPathLength] = '\0';
		}
	}
	return pszPhysicalPath;
}

//
// Utility function to read If-Modified-Since request header
//
char * MyHttpContext::GetRequestIfModifiedSince()
{
	if (pIfModifiedSince == NULL)
	{
		USHORT cchHeaderIfModifiedSince;
		
		pHttpContext->GetRequest()->GetHeader(HTTP_HEADER_ID::HttpHeaderIfModifiedSince, &cchHeaderIfModifiedSince);
		if (cchHeaderIfModifiedSince != 0)
		{
			pIfModifiedSince = (char *)pHttpContext->AllocateRequestMemory(cchHeaderIfModifiedSince + 1);
			pIfModifiedSince = (char *)pHttpContext->GetRequest()->GetHeader(HTTP_HEADER_ID::HttpHeaderIfModifiedSince, &cchHeaderIfModifiedSince);
			string temp = pIfModifiedSince;

			size_t start = temp.find_first_not_of(" ");
			if (start != string::npos)
			{
				temp = temp.substr(start);
			}
			size_t end = temp.find_last_not_of(" ");
			if (end != string::npos)
			{
				temp = temp.substr(0, end + 1);
			}
			strcpy_s(pIfModifiedSince, strlen(temp.c_str()) + 1, temp.c_str());
		}
	}
	return pIfModifiedSince;
}

//
// Utility function to read Range start value
//
DWORD MyHttpContext::GetRequestRangeStart()
{
	if (requestRangeStart == -1)
	{
		requestRangeStart = IISNM_NOT_FOUND;
		requestRangeEnd = IISNM_NOT_FOUND;

		// Read Range header Ex. Range: bytes=2703360-
		USHORT cchHeaderRange;
		PCSTR pszHeaderRange;

		pHttpContext->GetRequest()->GetHeader(HTTP_HEADER_ID::HttpHeaderRange, &cchHeaderRange);
		if (cchHeaderRange != 0)
		{
			pszHeaderRange = (PCSTR) pHttpContext->AllocateRequestMemory(cchHeaderRange + 1);
			pszHeaderRange = pHttpContext->GetRequest()->GetHeader(HTTP_HEADER_ID::HttpHeaderRange, &cchHeaderRange);
			string temp = pszHeaderRange;

			// tolower
			for (string::iterator it = temp.begin(); it != temp.end(); ++it)
			{
				*it = tolower(*it);
			}

			// remove "bytes" value 
			size_t bytesIndex = temp.find("bytes");
			if (bytesIndex != string::npos)
			{
				temp = temp.substr(bytesIndex + strlen("bytes"));
				size_t nonSpaceIndex = temp.find_first_not_of(" ");
				if (nonSpaceIndex != string::npos)
				{
					temp = temp.substr(nonSpaceIndex);

					// remove "=" value 
					if (temp.c_str()[0] == '=')
					{
						temp = temp.substr(1);
						size_t nonSpaceIndex = temp.find_first_not_of(" ");
						if (nonSpaceIndex != string::npos)
						{
							string valuePart = temp.substr(nonSpaceIndex);

							// get start number string
							size_t startIndex_startValue = valuePart.find_first_of("0123456789");
							if (startIndex_startValue == 0)
							{
								size_t endIndex_startValue = temp.find_first_not_of("0123456789", startIndex_startValue);
								string startValue = temp.substr(startIndex_startValue, endIndex_startValue != string::npos ? endIndex_startValue - startIndex_startValue : endIndex_startValue);

								// convert string to DWORD value
								requestRangeStart = atol(startValue.c_str());

								// initialize with 0
								requestRangeEnd = 0;

								// get end number string
								size_t hipenIndex = valuePart.find_first_of("-");
								if (hipenIndex != string::npos)
								{
									string nextHipen = valuePart.substr(hipenIndex + 1);
									nonSpaceIndex = nextHipen.find_first_not_of(" ");

									if (nonSpaceIndex != string::npos)
									{
										temp = nextHipen.substr(nonSpaceIndex);

										// get end number string
										size_t startIndex_endValue = temp.find_first_of("0123456789");

										if (startIndex_endValue == 0)
										{
											size_t endIndex_endValue = temp.find_first_not_of("0123456789", startIndex_endValue);
											string endValue = temp.substr(startIndex_endValue, endIndex_endValue != string::npos ? endIndex_endValue - startIndex_endValue : endIndex_endValue);

											// convert string to DWORD value
											requestRangeEnd = atol(endValue.c_str());
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return requestRangeStart;
}

//
// Utility function to read Range end value
//
DWORD MyHttpContext::GetRequestRangeEnd()
{
	if (requestRangeEnd == -1)
	{
		GetRequestRangeStart();
	}
	return requestRangeEnd;
}

//
// Utility function to get FileRangeSize
//
DWORD MyHttpContext::GetFileRangeSize(DWORD startIndex, DWORD endIndex)
{
	if (m_RangeSize == -1)
	{
		DWORD start = startIndex;
		DWORD end = endIndex;
		DWORD rangeSize = -1;

		if (start == IISNM_NOT_FOUND || end == IISNM_NOT_FOUND)
		{
			start = 0;
			end = 0;
		}

		if (fileContentLength == -1)
		{
			fileContentLength = GetFileContentLength();
		}
		if (start < fileContentLength)
		{
			// calcurate size of range
			rangeSize = fileContentLength;
			rangeSize -= start;

			if (end > start && end < start + rangeSize)
			{
				rangeSize = end - start + 1;
			}
		}
		m_RangeSize = rangeSize;
	}
	return m_RangeSize;
}

//
// Utility function to get file size of the physicalPath of Url
//
DWORD MyHttpContext::GetFileContentLength()
{
	if (fileContentLength == -1)
	{
		if (!m_file.is_open())
		{
			m_file = ifstream(pszPhysicalPath, ios::in | ios::binary | ios::ate);
		}
		if (m_file.is_open())
		{
			fileContentLength = (DWORD)m_file.tellg();

			// closing the m_file should be inside GetFileContentA() after which we don't use the file anymore
		}
	}
	return fileContentLength;
}

//
// Utility function to get file last write time
//
char * MyHttpContext::GetFileLastWriteTime()
{
	if (pFileLastWriteTime == NULL)
	{
		HRESULT hr = S_OK;
		HANDLE hFile;

		hFile = CreateFileA(pszPhysicalPath, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL);

		FILETIME ftCreate, ftAccess, ftWrite;
		SYSTEMTIME fileLastWriteTime;
		DWORD dwRet;

		// Retrieve the file times for the file.
		GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
		CloseHandle(hFile);
		FileTimeToSystemTime(&ftWrite, &fileLastWriteTime);

		std::stringstream ss_last_modified;
		string dayOfWeek;
		switch (fileLastWriteTime.wDayOfWeek)
		{
			case 0: dayOfWeek = "Sun"; break;
			case 1: dayOfWeek = "Mon"; break;
			case 2: dayOfWeek = "Tue"; break;
			case 3: dayOfWeek = "Wed"; break;
			case 4: dayOfWeek = "Thr"; break;
			case 5: dayOfWeek = "Fri"; break;
			case 6: dayOfWeek = "Sat"; break;
		}
		string month;
		switch (fileLastWriteTime.wMonth)
		{
			case 1: month = "Jan"; break;
			case 2: month = "Feb"; break;
			case 3: month = "Mar"; break;
			case 4: month = "Apr"; break;
			case 5: month = "May"; break;
			case 6: month = "Jun"; break;
			case 7: month = "Jul"; break;
			case 8: month = "Aug"; break;
			case 9: month = "Sep"; break;
			case 10: month = "Oct"; break;
			case 11: month = "Nov"; break;
			case 12: month = "Dec"; break;
		}
		ss_last_modified << dayOfWeek << ", ";
		if (fileLastWriteTime.wDay < 10)
		{
			ss_last_modified << "0";
		}
		ss_last_modified << fileLastWriteTime.wDay << " " << month << " " << fileLastWriteTime.wYear << " ";
		if (fileLastWriteTime.wHour < 10)
		{
			ss_last_modified << "0";
		}
		ss_last_modified << fileLastWriteTime.wHour << ":";
		if (fileLastWriteTime.wMinute < 10)
		{
			ss_last_modified << "0";
		}
		ss_last_modified << fileLastWriteTime.wMinute << ":";
		if (fileLastWriteTime.wSecond < 10)
		{
			ss_last_modified << "0";
		}
		ss_last_modified << fileLastWriteTime.wSecond << " " << "GMT";

		pFileLastWriteTime = (char *)pHttpContext->AllocateRequestMemory((DWORD)strlen(ss_last_modified.str().c_str()) + 1);
		strcpy_s(pFileLastWriteTime, strlen(ss_last_modified.str().c_str()) + 1, ss_last_modified.str().c_str());
	}
	return pFileLastWriteTime;
}

//
// Utility function to return file content of the physicalPath of Url
//
char * MyHttpContext::GetFileContentA(DWORD startIndex, DWORD endIndex, __out BOOL * pfCompletionExpected)
{
	if (*pfCompletionExpected == true)
	{
		// if the initial fCompletionExpected is true, returns immediately, caller should call this function again in order to complete. 
		// This is experimenting this work with async mode. So, in the next calling, it should use false value
		return NULL;
	}

	if (pFileContent == NULL)
	{
		DWORD start = startIndex;
		DWORD end = endIndex;

		if (start == IISNM_NOT_FOUND || end == IISNM_NOT_FOUND)
		{
			start = 0;
			end = 0;
		}
	
		if (!m_file.is_open())
		{
			m_file = ifstream(pszPhysicalPath, ios::in | ios::binary | ios::ate);
		}

		if (m_file.is_open())
		{
			if (fileContentLength == -1)
			{
				fileContentLength = (DWORD)m_file.tellg();
			}
			if (start < fileContentLength)
			{
				m_file.seekg(start, ios::beg);

				if (m_RangeSize == -1)
				{
					m_RangeSize = GetFileRangeSize(startIndex, endIndex);
				}

				pFileContent = (char *)pHttpContext->AllocateRequestMemory((DWORD)m_RangeSize);
				m_file.read(pFileContent, m_RangeSize);
			}
			m_file.close();
		}
	}
	return pFileContent;
}

//
// Utility function to get content type out of from IIS mimeType configuration
//
char * MyHttpContext::GetResponseHeaderContentTypeA()
{
	if (pResponseHeaderContentType == NULL)
	{
		// find Content-Type
		IAppHostElement * pStaticContentElement = NULL;
		g_pHttpServer->GetAdminManager()->GetAdminSection(SysAllocString(IISNM_STATIC_CONTENT_SECTION), SysAllocString(pHttpContext->GetApplication()->GetAppConfigPath()), &pStaticContentElement);

		IAppHostElementCollection * pStaticCollection = NULL;
		pStaticContentElement->get_Collection(&pStaticCollection);
		DWORD totalCount;
		pStaticCollection->get_Count(&totalCount);

		for (DWORD i = 0; i < totalCount; i++)
		{
			IAppHostElement * pElement = NULL;
			VARIANT varIndex;
			varIndex.vt = VT_UI4;
			varIndex.ullVal = i;
			pStaticCollection->get_Item(varIndex, &pElement);
			IAppHostProperty *pProperty2 = NULL;
			pElement->GetPropertyByName(SysAllocString(L"fileExtension"), &pProperty2);
			VARIANT val;
			pProperty2->get_Value(&val);

			// get file extension
			DWORD scriptNameLength = NULL;
			wstring physicalPath = pHttpContext->GetScriptName(&scriptNameLength);
			size_t dotIndx = physicalPath.find_last_of(L".");
			wstring fileExtension = physicalPath.substr(dotIndx);

			//if (_wcsnicmp(val.bstrVal, L".htm", wcslen(L".htm")) == 0)
			if (_wcsnicmp(val.bstrVal, fileExtension.c_str(), wcslen(fileExtension.c_str())) == 0)
			{
				IAppHostProperty *pProperty = NULL;
				pElement->GetPropertyByName(SysAllocString(L"mimeType"), &pProperty);
				pProperty->get_Value(&val);
				const wchar_t * temp = val.bstrVal;

				size_t length = (DWORD)wcslen(temp);
				size_t destBufflen = length + 1;  // null string
				pResponseHeaderContentType = (char *)pHttpContext->AllocateRequestMemory((DWORD)destBufflen);
				size_t length2;
				wcstombs_s(&length2, pResponseHeaderContentType, destBufflen, temp, destBufflen);
				pResponseHeaderContentType[length] = '\0';
				break;
			}
		}
	}
	return pResponseHeaderContentType;
}

//
// Utility function to send response body
//
HRESULT MyHttpContext::WriteResponseMessage(PCSTR pszValue, DWORD valueLength)
{
	// Create an HRESULT to receive return values from methods.
	HRESULT hr;

	// Create a data chunk.
	HTTP_DATA_CHUNK dataChunk;
	// Set the chunk to a chunk in memory.
	dataChunk.DataChunkType = HttpDataChunkFromMemory;
	// Buffer for bytes written of data chunk.
	DWORD cbSent;

	// Set the chunk
	dataChunk.FromMemory.pBuffer =
		(PVOID)pszValue;
	// Set the chunk size to the second buffer size.
	dataChunk.FromMemory.BufferLength =
		(USHORT)valueLength;
	// Insert the data chunk into the response.
	hr = pHttpContext->GetResponse()->WriteEntityChunks(
		&dataChunk, 1, FALSE, TRUE, &cbSent);

	// Test for an error.
	if (FAILED(hr))
	{
		// Return the error status.
		return hr;
	}
	// Return a success status.
	return S_OK;
}

//
// Utility function to set Response header
//
HRESULT MyHttpContext::SetResponseHeader(char* headerName, char* headerValue, DWORD headerValueLength, BOOL fReplace)
{
	pHttpContext->GetResponse()->SetHeader(headerName, headerValue, (USHORT)headerValueLength, fReplace);
	return S_OK;
}

//
// Utility function to WriteResponseMessage flushing buffer asynchronously
//
REQUEST_NOTIFICATION_STATUS MyHttpContext::WriteResponseMessageAsync()
{
	REQUEST_NOTIFICATION_STATUS retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_CONTINUE;
	int bufferSize = 4096;
	if (m_RangeSize > bufferSize * 10)
	{
		// reset bufferSize if file size is bigger than 10 times of buffersize
		bufferSize *= 2;
	}

	// ToDo: Sending fine content can be done asynchrously
	// Send file content
	if (pFileContent != NULL)
	{
		// Send File content
		while (m_buffer_index < m_RangeSize)
		{
			if (m_buffer_index + bufferSize > m_RangeSize)
			{
				bufferSize = m_RangeSize - m_buffer_index;
			}
			if (FAILED(WriteResponseMessage((pFileContent + m_buffer_index), bufferSize)))
			{
				// error happens while WritingResponseMessage
				retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_FINISH_REQUEST;
				break;
			}

			m_buffer_index += bufferSize;

			if (HttpConnectionDisconnected)
			{
				// terminate if the connection is disconnected
				retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_FINISH_REQUEST;
				break;
			}

			// Flush buffer
			DWORD cbFlushSent = 0;
			BOOL fCompletionExpected = false;

			if (FAILED(pHttpContext->GetResponse()->Flush(true, true, &cbFlushSent, &fCompletionExpected)))
			{
				// error happens while flushing
				retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_FINISH_REQUEST;
			}

			if (fCompletionExpected)
			{
				// if Flush() should be done asynchronously, break looping here and return RQ_NOTIFICATION_PENDING
				retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_PENDING;
				break;
			}
		}
	}
	return retVal;
}