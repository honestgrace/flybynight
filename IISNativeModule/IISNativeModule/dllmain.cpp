// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#define _WINSOCKAPI_
#include <windows.h>
#include <sal.h>
#include <httpserv.h>
#include <string>
using namespace std;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


// Create the module's global class.
class MyGlobalModule : public CGlobalModule
{
public:


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalStopListening(
            _In_ IGlobalStopListeningProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_CACHE_CLEANUP


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalCacheCleanup(
            VOID
        )
    {
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_CACHE_OPERATION


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalCacheOperation(
            _In_ ICacheProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_HEALTH_CHECK


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalHealthCheck(
            VOID
        )
    {
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_CONFIGURATION_CHANGE


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalConfigurationChange(
            _In_ IGlobalConfigurationChangeProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_FILE_CHANGE 


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalFileChange(
            _In_ IGlobalFileChangeProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_PRE_BEGIN_REQUEST 


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalPreBeginRequest(
            _In_ IPreBeginRequestProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_APPLICATION_START 


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalApplicationStart(
            _In_ IHttpApplicationStartProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_APPLICATION_RESOLVE_MODULES


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalApplicationResolveModules(
            _In_ IHttpApplicationResolveModulesProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_APPLICATION_STOP


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalApplicationStop(
            _In_ IHttpApplicationStopProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_RSCA_QUERY


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalRSCAQuery(
            _In_ IGlobalRSCAQueryProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_TRACE_EVENT


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalTraceEvent(
            _In_ IGlobalTraceEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_CUSTOM_NOTIFICATION


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalCustomNotification(
            _In_ ICustomNotificationProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_THREAD_CLEANUP


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalThreadCleanup(
            _In_ IGlobalThreadCleanupProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_APPLICATION_PRELOAD


    GLOBAL_NOTIFICATION_STATUS
        OnGlobalApplicationPreload(
            _In_ IGlobalApplicationPreloadProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    // GL_SUSPEND_PROCESS 

    GLOBAL_NOTIFICATION_STATUS
        OnSuspendProcess(
            _Outptr_ IGlobalSuspendProcessCallback** pCallback
        )
    {
        UNREFERENCED_PARAMETER(pCallback);
        OutputDebugStringA(
            "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__); 
        OutputDebugStringA("\n");
        // DebugBreak();

        return GL_NOTIFICATION_CONTINUE;
    }

    VOID Terminate()
    {
        // Remove the class from memory.
        delete this;
    }

    MyGlobalModule()
    {
    }

    ~MyGlobalModule()
    {
    }

private:

    // Define a method that writes to the Event Viewer.
    BOOL Debug(LPCWSTR szNotification)
    {
        wstring temp(szNotification);
        temp.append(L"\n");
        OutputDebugString(temp.c_str());
        return FALSE;
    }
};

// Create the module class.
class MyHttpModule : public CHttpModule
{
public:


    REQUEST_NOTIFICATION_STATUS
        OnBeginRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    REQUEST_NOTIFICATION_STATUS
        OnPostBeginRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    // RQ_AUTHENTICATE_REQUEST


    REQUEST_NOTIFICATION_STATUS
        OnAuthenticateRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IAuthenticationProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostAuthenticateRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    // RQ_AUTHORIZE_REQUEST


    REQUEST_NOTIFICATION_STATUS
        OnAuthorizeRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostAuthorizeRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    // RQ_RESOLVE_REQUEST_CACHE


    REQUEST_NOTIFICATION_STATUS
        OnResolveRequestCache(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostResolveRequestCache(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    // RQ_MAP_REQUEST_HANDLER


    REQUEST_NOTIFICATION_STATUS
        OnMapRequestHandler(
            _In_ IHttpContext* pHttpContext,
            _In_ IMapHandlerProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostMapRequestHandler(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    // RQ_ACQUIRE_REQUEST_STATE


    REQUEST_NOTIFICATION_STATUS
        OnAcquireRequestState(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostAcquireRequestState(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    // RQ_PRE_EXECUTE_REQUEST_HANDLER


    REQUEST_NOTIFICATION_STATUS
        OnPreExecuteRequestHandler(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostPreExecuteRequestHandler(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    // RQ_EXECUTE_REQUEST_HANDLER


    REQUEST_NOTIFICATION_STATUS
        OnExecuteRequestHandler(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostExecuteRequestHandler(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // RQ_RELEASE_REQUEST_STATE


    REQUEST_NOTIFICATION_STATUS
        OnReleaseRequestState(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostReleaseRequestState(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    // RQ_UPDATE_REQUEST_CACHE


    REQUEST_NOTIFICATION_STATUS
        OnUpdateRequestCache(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }



    REQUEST_NOTIFICATION_STATUS
        OnPostUpdateRequestCache(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // RQ_LOG_REQUEST


    REQUEST_NOTIFICATION_STATUS
        OnLogRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    REQUEST_NOTIFICATION_STATUS
        OnPostLogRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // RQ_END_REQUEST


    REQUEST_NOTIFICATION_STATUS
        OnEndRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }


    REQUEST_NOTIFICATION_STATUS
        OnPostEndRequest(
            _In_ IHttpContext* pHttpContext,
            _In_ IHttpEventProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // RQ_SEND_RESPONSE


    REQUEST_NOTIFICATION_STATUS
        OnSendResponse(
            _In_ IHttpContext* pHttpContext,
            _In_ ISendResponseProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");

        IHttpUser* pHttpUser = nullptr;
        pHttpUser = pHttpContext->GetUser();
        if (pHttpUser != NULL)
        {
            PCWSTR pwszUserName = nullptr;
            pwszUserName = pHttpUser->GetUserName();
            OutputDebugStringA("UserName:");
            OutputDebugString(pwszUserName);
            OutputDebugStringA("\n");

            PCWSTR pwszPassword = nullptr;
            pwszPassword = pHttpUser->GetPassword();
            OutputDebugStringA("Password:");
            OutputDebugString(pwszPassword);
            OutputDebugStringA("\n");
        }
        else
        {
            OutputDebugStringA("pHttpUser is null\n");
        }
        

        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // RQ_MAP_PATH


    REQUEST_NOTIFICATION_STATUS
        OnMapPath(
            _In_ IHttpContext* pHttpContext,
            _In_ IMapPathProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // RQ_READ_ENTITY


    REQUEST_NOTIFICATION_STATUS
        OnReadEntity(
            _In_ IHttpContext* pHttpContext,
            _In_ IReadEntityProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // RQ_CUSTOM_NOTIFICATION


    REQUEST_NOTIFICATION_STATUS
        OnCustomRequestNotification(
            _In_ IHttpContext* pHttpContext,
            _In_ ICustomNotificationProvider* pProvider
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(pProvider);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // Completion


    REQUEST_NOTIFICATION_STATUS
        OnAsyncCompletion(
            _In_ IHttpContext* pHttpContext,
            _In_ DWORD                      dwNotification,
            _In_ BOOL                       fPostNotification,
            _In_ IHttpEventProvider* pProvider,
            _In_ IHttpCompletionInfo* pCompletionInfo
        )
    {
        UNREFERENCED_PARAMETER(pHttpContext);
        UNREFERENCED_PARAMETER(dwNotification);
        UNREFERENCED_PARAMETER(fPostNotification);
        UNREFERENCED_PARAMETER(pProvider);
        UNREFERENCED_PARAMETER(pCompletionInfo);
        OutputDebugStringA(
        "This module subscribed to event ");
        OutputDebugStringA(__FUNCTION__);
        OutputDebugStringA("\n");


        // DebugBreak();

        return RQ_NOTIFICATION_CONTINUE;
    }

    // 
    MyHttpModule()
    {
    }

    ~MyHttpModule()
    {
    }

private:

    // Define a method that writes to the Event Viewer.
    BOOL Debug(LPCWSTR szNotification)
    {
        wstring temp(szNotification);
        temp.append(L"\n");
        OutputDebugString(temp.c_str());
        return FALSE;
    }
};

// Create the module's class factory.
class MyHttpModuleFactory : public IHttpModuleFactory
{
public:
    HRESULT
        GetHttpModule(
            OUT CHttpModule** ppModule,
            IN IModuleAllocator* pAllocator
        )
    {
        UNREFERENCED_PARAMETER(pAllocator);

        // Create a new instance.
        MyHttpModule* pModule = new MyHttpModule;

        // Test for an error.
        if (!pModule)
        {
            // Return an error if the factory cannot create the instance.
            return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        }
        else
        {
            // Return a pointer to the module.
            *ppModule = pModule;
            pModule = NULL;
            // Return a success status.
            return S_OK;
        }
    }

    void Terminate()
    {
        // Remove the class from memory.
        delete this;
    }
};

// Create the module's exported registration function.
HRESULT
__stdcall
RegisterModule(
    DWORD dwServerVersion,
    IHttpModuleRegistrationInfo* pModuleInfo,
    IHttpServer* pGlobalInfo
)
{
    UNREFERENCED_PARAMETER(dwServerVersion);
    UNREFERENCED_PARAMETER(pGlobalInfo);

    // Create an instance of the global module class.
    MyGlobalModule* pGlobalModule = new MyGlobalModule;
    // Test for an error.
    if (NULL == pGlobalModule)
    {
        return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
    }
    // Set the global notifications and exit.
    
    HRESULT result = S_OK;

    result = pModuleInfo->SetRequestNotifications(
        // Specify the class factory.
        new MyHttpModuleFactory,
        RQ_BEGIN_REQUEST | 
        RQ_AUTHENTICATE_REQUEST | 
        RQ_AUTHORIZE_REQUEST | 
        RQ_RESOLVE_REQUEST_CACHE | 
        RQ_MAP_REQUEST_HANDLER | 
        RQ_ACQUIRE_REQUEST_STATE | 
        RQ_PRE_EXECUTE_REQUEST_HANDLER | 
        RQ_EXECUTE_REQUEST_HANDLER | 
        RQ_RELEASE_REQUEST_STATE | 
        RQ_UPDATE_REQUEST_CACHE | 
        RQ_LOG_REQUEST | 
        RQ_END_REQUEST | 
        RQ_SEND_RESPONSE_STATUS_AND_HEADERS | 
        RQ_CUSTOM_NOTIFICATION | 
        RQ_SEND_RESPONSE | 
        RQ_READ_ENTITY | 
        RQ_MAP_PATH,        
        0
    );

    if (result != S_OK)
    {
        return result;
    }

    return pModuleInfo->SetGlobalNotifications(
        pGlobalModule,
        GL_STOP_LISTENING | 
        GL_CACHE_CLEANUP | 
        GL_CACHE_OPERATION | 
        GL_HEALTH_CHECK | 
        GL_CONFIGURATION_CHANGE | 
        GL_FILE_CHANGE | 
        GL_PRE_BEGIN_REQUEST | 
        GL_APPLICATION_START | 
        GL_APPLICATION_RESOLVE_MODULES | 
        GL_APPLICATION_STOP | 
        GL_RSCA_QUERY | 
        GL_TRACE_EVENT | 
        GL_CUSTOM_NOTIFICATION | 
        GL_THREAD_CLEANUP | 
        GL_APPLICATION_PRELOAD | 
        GL_SUSPEND_PROCESS);
}