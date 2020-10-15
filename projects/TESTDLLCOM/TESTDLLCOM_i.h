

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for TESTDLLCOM.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __TESTDLLCOM_i_h__
#define __TESTDLLCOM_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILogger_FWD_DEFINED__
#define __ILogger_FWD_DEFINED__
typedef interface ILogger ILogger;

#endif 	/* __ILogger_FWD_DEFINED__ */


#ifndef __Logger_FWD_DEFINED__
#define __Logger_FWD_DEFINED__

#ifdef __cplusplus
typedef class Logger Logger;
#else
typedef struct Logger Logger;
#endif /* __cplusplus */

#endif 	/* __Logger_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ILogger_INTERFACE_DEFINED__
#define __ILogger_INTERFACE_DEFINED__

/* interface ILogger */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILogger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a07e865f-b9a0-45d4-972e-852f5500d317")
    ILogger : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TestCaseNumber( 
            /* [retval][out] */ DOUBLE *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_TestCaseNumber( 
            /* [in] */ DOUBLE newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Log( 
            /* [in] */ BSTR message) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILoggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogger * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILogger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILogger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILogger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILogger * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TestCaseNumber )( 
            ILogger * This,
            /* [retval][out] */ DOUBLE *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TestCaseNumber )( 
            ILogger * This,
            /* [in] */ DOUBLE newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Log )( 
            ILogger * This,
            /* [in] */ BSTR message);
        
        END_INTERFACE
    } ILoggerVtbl;

    interface ILogger
    {
        CONST_VTBL struct ILoggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILogger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILogger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILogger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILogger_get_TestCaseNumber(This,pVal)	\
    ( (This)->lpVtbl -> get_TestCaseNumber(This,pVal) ) 

#define ILogger_put_TestCaseNumber(This,newVal)	\
    ( (This)->lpVtbl -> put_TestCaseNumber(This,newVal) ) 

#define ILogger_Log(This,message)	\
    ( (This)->lpVtbl -> Log(This,message) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogger_INTERFACE_DEFINED__ */



#ifndef __TESTDLLCOMLib_LIBRARY_DEFINED__
#define __TESTDLLCOMLib_LIBRARY_DEFINED__

/* library TESTDLLCOMLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_TESTDLLCOMLib;

EXTERN_C const CLSID CLSID_Logger;

#ifdef __cplusplus

class DECLSPEC_UUID("c630320d-d423-472c-8eef-9129226a14cf")
Logger;
#endif
#endif /* __TESTDLLCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


