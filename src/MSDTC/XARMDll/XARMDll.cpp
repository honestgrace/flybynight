// XARMDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <string>
using namespace std;

#pragma hdrstop
#include "xa.h"
#include <tchar.h>

typedef DWORD XA_SWITCH_FLAGS;
#define MAX_DSN_LEN 128

#define DEBUG(msg) OutputDebugStringA(msg)

int __cdecl xa_open(char *xa_info, int rmid, long flag);
int __cdecl xa_close(char *xa_info, int rmid, long flag);
int __cdecl xa_start(XID *xid, int rmid, long flag);
int __cdecl xa_end(XID *xid, int rmid, long flag);
int __cdecl xa_rollback(XID *xid, int rmid, long flag);
int __cdecl xa_prepare(XID *xid, int rmid, long flag);
int __cdecl xa_commit(XID *xid, int rmid, long flag);
int __cdecl xa_recover(XID *xid, long count, int rmid, long flag);
int __cdecl xa_forget(XID *xid, int rmid, long flag);

xa_switch_t g_XaSwitch = {{'\0'}, 0, 0, xa_open, xa_close, xa_start, xa_end, xa_rollback, xa_prepare, xa_commit, xa_recover, xa_forget};

extern "C" HRESULT __cdecl GetXaSwitch(XA_SWITCH_FLAGS XaSwitchFlags, xa_switch_t** ppXaSwitch)
{
	DEBUG("GetXaSwitch\r\n");
	*ppXaSwitch = &g_XaSwitch;
	return S_OK;
}

int __cdecl xa_open(char *xa_info, int rmid, long flags)
{
	DEBUG("xa_open\r\n");
	return XA_OK;
}

int __cdecl xa_close(char *xa_info, int rmid, long flags)
{
	DEBUG("xa_close\r\n");
	return XA_OK; 
}

int __cdecl xa_start(XID *xid, int rmid, long flags)
{
	DEBUG("xa_start\r\n");
	return XA_OK;
}

int __cdecl xa_end(XID *xid, int rmid, long flags)
{
	DEBUG("xa_end\r\n");
	return XA_OK;
}

int __cdecl xa_prepare(XID *xid, int rmid, long flags)
{
	DEBUG("xa_prepare\r\n");
	return XA_OK;
}

int __cdecl xa_commit(XID *xid, int rmid, long flags)
{
	DEBUG("xa_commit\r\n");
	return XA_OK;
}

int __cdecl xa_rollback(XID *xid, int rmid, long flags)
{
	DEBUG("xa_rollback\r\n");
	return XA_OK;
} 

int __cdecl xa_recover(XID *xid, long count, int rmid, long flags)
{
	DEBUG("xa_recover\r\n");
	return XA_OK;
}

int __cdecl xa_forget(XID *xid, int rmid, long flags)
{
	DEBUG("xa_forget\r\n");
	return XA_OK;
}