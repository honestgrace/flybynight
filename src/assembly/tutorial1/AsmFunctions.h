#pragma once
extern "C" int GetValueFromASM();  // "C" (C calling convention) makes not to function name mangled by C++ compiler
extern "C" int TestFunction();
extern "C" int AddASM(int a, int b);
extern "C" void ZeroArrayASM(void *array, const int countInByte);
extern "C" void * ReturnMyArray();
extern "C" int Multiply(int a, int b);
extern "C" int Divide(int a, int b);
extern "C" int Remainder(int a, int b);
extern "C" int CallCppASM();
extern "C" HANDLE CreateThreadInASM(int* i);
extern "C" void ThreadStartProc(int *i);