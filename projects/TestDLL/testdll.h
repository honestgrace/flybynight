#pragma once

#ifdef TESTDLL_EXPORTS
#define TESTDLL_API __declspec(dllexport)
#else
#define TESTDLL_API __declspec(dllimport)
#endif

#include <string>

extern "C" TESTDLL_API bool Foo();
extern "C" TESTDLL_API bool Log(std::string inputstring);