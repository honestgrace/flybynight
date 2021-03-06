#include "stdafx.h"

HINSTANCE           g_hModule     = NULL;
HTTP_MODULE_ID      g_pModuleId   = NULL;
IHttpServer *       g_pHttpServer = NULL;
IHttpServer *       g_pGlobalInfo = NULL;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		DisableThreadLibraryCalls(hModule); // Optimize thread calling
		break;
    }
    return TRUE;
}

int counter = 0;

class MyGlobalModule : public CGlobalModule
{
public:

	GLOBAL_NOTIFICATION_STATUS
		OnGlobalPreBeginRequest(IN IPreBeginRequestProvider * pProvider)
	{
		HRESULT hr = S_OK;
		IHttpContext* pHttpContext = pProvider->GetHttpContext();
		IHttpResponse * pHttpResponse = pHttpContext->GetResponse();

		IHttpTraceContext * pTraceContext = pHttpContext->GetTraceContext();
		pTraceContext->QuickTrace(L"from IIS Native Module", 0, 0, 4);

        /********
        pHttpContext->CancelIo();
        pHttpResponse->ResetConnection();
        counter++;

        if (counter > 4)
        {
            IHttpContext3* _pHttpContext;
            hr = HttpGetExtendedInterface(g_pGlobalInfo, pHttpContext, &_pHttpContext);
            _pHttpContext->CancelIo();
        }
        ********/
		return GL_NOTIFICATION_CONTINUE;
	}

	VOID Terminate()
	{
		delete this;
	}

	MyGlobalModule()
	{
	}

	~MyGlobalModule()
	{
	}
};



HRESULT __stdcall RegisterModule(DWORD dwServerVersion, IHttpModuleRegistrationInfo * pModuleInfo, IHttpServer * pGlobalInfo)
{
	UNREFERENCED_PARAMETER(dwServerVersion);
	UNREFERENCED_PARAMETER(pGlobalInfo);

	MyGlobalModule * pGlobalModule = new MyGlobalModule;

	if (NULL == pGlobalModule)
	{
		return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
	}

    g_pGlobalInfo = pGlobalInfo;

    if (NULL == g_pGlobalInfo)
    {
        return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
    }

	return pModuleInfo->SetGlobalNotifications(
		pGlobalModule, GL_PRE_BEGIN_REQUEST);
}

