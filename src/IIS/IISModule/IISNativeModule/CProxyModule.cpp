#include "stdafx.h"

void _STDCALL DoThreadWork(IHttpContext * pContext, MyHttpContext * pMyHttpContext);

__override REQUEST_NOTIFICATION_STATUS
CProxyModule::OnExecuteRequestHandler(
	IHttpContext * pHttpContext,
	IHttpEventProvider *  pProvider
)
{
	HRESULT hr = S_OK;
	REQUEST_NOTIFICATION_STATUS retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_CONTINUE;

	pContext = new MyHttpContext(pHttpContext);
	MyHttpContext & context = *pContext;

	hr = context.Initialize();
	if (hr != S_OK)
	{
		// Failed to initialize MyHttpContext object
		return REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_FINISH_REQUEST;
	}

	// Set FilePath response header
	context.SetResponseHeader((char *) "FilePath", context.GetPhysicalPathA(), (DWORD) strlen(context.GetPhysicalPathA()), true);

	// set Content-Type response header
	context.SetResponseHeader((char *) "Content-Type", context.GetResponseHeaderContentTypeA(), (DWORD) strlen(context.GetResponseHeaderContentTypeA()), true);
		
	// set Accept-Range response header
	context.SetResponseHeader((char *) "Accept-Range", (char *) "bytes", (DWORD)strlen("bytes"), true);

	// Read If-Modified-Since request Header and compare the LastWriteTime
	if (context.GetRequestIfModifiedSince() != NULL && context.GetFileLastWriteTime() != NULL)
	{
		if (0 == string(context.GetRequestIfModifiedSince()).compare(string(context.GetFileLastWriteTime())))
		{
			// if matched, return 304, Not Modified
			pHttpContext->GetResponse()->SetStatus(304, "Not Modified", 0, 0, NULL, true);
			retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_CONTINUE;
			return retVal;
		}
	}

	// get Range value
	DWORD rangeStart = context.GetRequestRangeStart();
	DWORD rangeEnd = context.GetRequestRangeEnd();

	// Calculate Range Size with give rangeStart and rangeEnd
	context.GetFileRangeSize(rangeStart, rangeEnd);

	DWORD contentLength = 0;

	// Set Last-Modified response header
	context.SetResponseHeader((char *) "Last-Modified", context.GetFileLastWriteTime(), (DWORD)strlen(context.GetFileLastWriteTime()), true);

	if (rangeStart != IISNM_NOT_FOUND)
	{
		// Set 206 for this Range request
		pHttpContext->GetResponse()->SetStatus(206, "Partial Content", 0, 0, NULL, true);

		// set Content-Range response header if range request header was found
		DWORD fileSize = context.GetFileContentLength();
		rangeEnd = (rangeEnd != 0 && rangeEnd < fileSize) ? rangeEnd : fileSize - 1;

		std::stringstream ss_content_range;
		ss_content_range << "bytes " << rangeStart << "-" << rangeEnd << "/" << fileSize;
		context.SetResponseHeader((char *) "Content-Range", (char *)ss_content_range.str().c_str(), (DWORD)strlen(ss_content_range.str().c_str()), true);
	}

	// Set Content-Length response header
	std::stringstream ss_content_length;
	ss_content_length << context.m_RangeSize;
	context.SetResponseHeader((char *) "Content-Length", (char *)ss_content_length.str().c_str(), (DWORD)strlen(ss_content_length.str().c_str()), true);


	// states machine start with 0
	context.m_requestMode = 0;

	// read FileContent with range and get rangeSize; if range was set to D_NOT_FOUND, it reads whole file
	
	// if the initial value of fCompletionExpected is set to true, it will return also true
	BOOL fCompletionExpected = true;  
	context.GetFileContentA(rangeStart, rangeEnd, &fCompletionExpected);

	if (fCompletionExpected)
	{
		retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_PENDING;

		// read file should be done asynchronously with creating a new thread
		context.t = thread(DoThreadWork, pHttpContext, &context);
	}
	else
	{
		// states machine is moved to next state to indicate that reading file was done.
		// next state is to send the file content to client.
		context.m_requestMode = 1;

		// Send file content to client.
		// WriteResponseMessageAsync() has a loop. Inside the loop, flushing buffer need to be asynchronously, it will return RQ_NOTIFICATION_PENDING.
		// When the async tas is done, OnAsynchCompeletion() is called to continue.
		retVal = context.WriteResponseMessageAsync();
	}

	return retVal;
}

void _STDCALL DoThreadWork(IHttpContext * pContext, MyHttpContext * pMyHttpContext)
{
	// if the initial value of fCompletionExpected is set to true, it will return also true
	BOOL fCompletionExpected = false;
	MyHttpContext & context = *pMyHttpContext;

	// ToDo: read file with USHORT size and consider reading file asynchronously as well
	do 
	{
		context.GetFileContentA(context.GetRequestRangeStart(), context.GetRequestRangeEnd(), &fCompletionExpected);
	} 
	while (fCompletionExpected);
		
	// Now this work completes and let's call OnAsyncCompletion to complete the remainig part
	pContext->PostCompletion(0);

	// NOTE: 
	// if IndicateCompletion(...Continue) is used instead of PostCompletion, it returns to IIS pipeline immediately
	// pContext->IndicateCompletion(REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_CONTINUE);
}

__override REQUEST_NOTIFICATION_STATUS
CProxyModule::OnAsyncCompletion(
	IHttpContext *          pHttpContext,
	DWORD                   dwNotification,
	BOOL                    fPostNotification,
	IHttpEventProvider *    pProvider,
	IHttpCompletionInfo *   pCompletionInfo
)
{
	REQUEST_NOTIFICATION_STATUS retVal = REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_CONTINUE;
	MyHttpContext & context = *pContext;
	
    // Below code should run when DoThreadWork() function is completed
	if (context.m_requestMode == 0)
	{
		// This part starts when PostCompletion() is called from the DoThreadWork() thread

		// states machine is moved to next state to indicate that reading file was done.
		// next state is to send the file content to client.
		context.m_requestMode = 1;
	}

	if (context.m_requestMode == 1)
	{
		// Send file content to client.
		// WriteResponseMessageAsync() has a loop. Inside the loop, flushing buffer need to be asynchronously, it will return RQ_NOTIFICATION_PENDING.
		// When the async tas is done, OnAsynchCompeletion() is called to continue.
		retVal = context.WriteResponseMessageAsync();

		if (retVal == REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_CONTINUE)
		{
			// states machine is moved to next state to indicate that sending file content was done.
			// next state is to clean up
			context.m_requestMode = 2;
		}
	}

	if (context.m_requestMode == 2)
	{
		// Clean up state

		// ToDo: Consider reusing the thread so that we don't need to create a new thread and close here

		// wait the DoThreadWork() thread exits
		context.t.join();
	}

	// NOTE: 
	// You can use IndicateCompletion(...Continue) as well. In that case, return value for this AyncCompletion is meaningless.
	//pHttpContext->IndicateCompletion(RQ_NOTIFICATION_CONTINUE);
	//return REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_PENDING;

	return retVal;
}