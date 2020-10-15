#include "stdafx.h"

struct WinDbg_Struct
{
    int _a;
    double _b;
    WinDbg_Struct * FLink;
};

char mySrc[256] = "hello world";
char myDest[256] = { 0, };

int WinDbg_Test()
{
    WinDbg_Struct a;
    a._a = 1;
    a._b = 1;
    WinDbg_Struct b;
    b._a = 2;
    b._b = 2;
    WinDbg_Struct c;
    c._a = 3;
    c._b = 3;

    a.FLink = &b;
    b.FLink = &c;
    //c.FLink = nullptr;
    c.FLink = &a;
    
    memcpy(myDest, mySrc, strlen(mySrc) + 1);
    printf(myDest);

    //__asm int 3;
    return a._a * a._b;
}

void WinDbg()
{
    auto result = WinDbg_Test();
}