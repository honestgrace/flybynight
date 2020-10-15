#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>

#define NTDDI_VERSION 0x06020000
#define _WIN32_WINNT 0x0602
#define _WINSOCKAPI_
#include <windows.h>
#include <atlbase.h>
#include <pdh.h>

