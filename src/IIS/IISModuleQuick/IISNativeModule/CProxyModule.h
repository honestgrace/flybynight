#pragma once

extern HINSTANCE      g_hModule;
extern IHttpServer *  g_pHttpServer;
extern HTTP_MODULE_ID g_pModuleId;

class MyHttpContext;

class CProxyModule : public CHttpModule
{
public:
	MyHttpContext * pContext;

	CProxyModule()
		: pContext(NULL)
	{
	}
	~CProxyModule()
	{
		if (pContext != NULL)
		{
			delete pContext;
		}
	}

	//
	// Delegate allocating memory to IIS Core.
	// NOTE: 
	//       This method requires operator delete shoud do nothing because the memory is managed by IIS Core
	//
	void * operator new(size_t size, IModuleAllocator * pPlacement)
	{
		return pPlacement->AllocateMemory(static_cast<DWORD>(size));
	}

	//
	// Nothing done here because the allocated memory is managed by the IIS Core.
	//
	void
	operator delete(
		void *
	)
	{}

	__override REQUEST_NOTIFICATION_STATUS
		OnExecuteRequestHandler(
			IHttpContext * pHttpContext,
			IHttpEventProvider *  pProvider
		);

	__override REQUEST_NOTIFICATION_STATUS
		OnAsyncCompletion(
			IHttpContext *          pHttpContext,
			DWORD                   dwNotification,
			BOOL                    fPostNotification,
			IHttpEventProvider *    pProvider,
			IHttpCompletionInfo *   pCompletionInfo
		);

private:
};