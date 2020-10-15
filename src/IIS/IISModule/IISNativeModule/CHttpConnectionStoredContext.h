#pragma once

class IHttpConnectionStoredContext;
class MyHttpContext;

class CHttpConnectionStoredContext : public IHttpConnectionStoredContext
{
private:
	MyHttpContext * pMyHttpContext;

public:
	CHttpConnectionStoredContext(MyHttpContext * pMyHttpContext)
		: pMyHttpContext(pMyHttpContext)
	{
	}

	__override VOID
		NotifyDisconnect()
	{
		// IIS calls NotifyDisconnect() when the current connection is terminated with various reasons such as client side disconnection and etc.
		pMyHttpContext->HttpConnectionDisconnected = true;
	}

	__override VOID
		CleanupStoredContext()
	{
		// IIS calls CleanupStoredContext() when this object is okay to delete
		delete this;
	}

private:
	// ToDo: Add comment why destructor is declared with private.
	~CHttpConnectionStoredContext()
	{
	}
};