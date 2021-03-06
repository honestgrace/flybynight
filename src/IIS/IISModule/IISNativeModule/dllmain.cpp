/* 
Q&A 
1. If you can't load symbol file, check the link option is set with DebugFull as the following:
	<Link>
		<GenerateDebugInformation>DebugFull</GenerateDebugInformation>
		...
2. Powershell script to copy dll/pdb files 

"C:\Program Files\Debugging Tools for Windows (x64)\kill.exe" w3wp.exe
copy /y C:\Users\jhkim\source\repos\ConsoleApp1\x64\Debug\IISNativeModule.dll c:\windows\system32\inetsrv
copy /y C:\Users\jhkim\source\repos\ConsoleApp1\x64\Debug\IISNativeModule.pdb c:\windows\system32\inetsrv
copy /y C:\Users\jhkim\source\repos\ConsoleApp1\Debug\IISNativeModule.dll c:\windows\syswow64\inetsrv
copy /y C:\Users\jhkim\source\repos\ConsoleApp1\Debug\IISNativeModule.pdb c:\windows\syswow64\inetsrv

*/

#include "stdafx.h"

HINSTANCE           g_hModule     = NULL;
HTTP_MODULE_ID      g_pModuleId   = NULL;
IHttpServer *       g_pHttpServer = NULL;

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

class CProxyModuleFactory : public IHttpModuleFactory
{
public:
	__override
		HRESULT
		GetHttpModule(
			CHttpModule **      ppModule,
			IModuleAllocator *  pAllocator
		)
	{
		CProxyModule *pModule = new (pAllocator) CProxyModule();
		if (pModule == NULL)
		{
			return E_OUTOFMEMORY;
		}

		*ppModule = pModule;
		pModule = NULL;
		return S_OK;
	}

	__override
		VOID
		Terminate()
	{
		delete this;
	}
};


HRESULT __stdcall RegisterModule(DWORD dwServerVersion, IHttpModuleRegistrationInfo * pModuleInfo, IHttpServer * pHttpServer)
{
	HRESULT hr = S_OK;
	CProxyModuleFactory * pFactory = nullptr;
	MyGlobalModule * pGlobalModule = nullptr;
	g_pHttpServer = pHttpServer;
	g_pModuleId = pModuleInfo->GetId();

	pFactory = new CProxyModuleFactory;
	if (pFactory == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto Finished;
	}

	hr = pModuleInfo->SetRequestNotifications(pFactory, RQ_EXECUTE_REQUEST_HANDLER, 0);
	if (FAILED(hr))
	{
		goto Finished;
	}
	pFactory = nullptr;  // give the ownership of this memory to IISCore
	
	pGlobalModule = new MyGlobalModule;
	if (NULL == pGlobalModule)
	{
		hr = E_OUTOFMEMORY;
		goto Finished;
	}

	hr = pModuleInfo->SetGlobalNotifications(pGlobalModule, GL_CONFIGURATION_CHANGE | GL_STOP_LISTENING);
	if (FAILED(hr))
	{
		goto Finished;
	}
	pGlobalModule = nullptr;  // give the ownership of this memory to IISCore
	
Finished:
	if (pFactory != nullptr)
	{
		pFactory->Terminate();
		pFactory = nullptr;
	}

	if (pGlobalModule != nullptr)
	{
		delete pGlobalModule;
		pGlobalModule = nullptr;
	}
	return hr;
}

