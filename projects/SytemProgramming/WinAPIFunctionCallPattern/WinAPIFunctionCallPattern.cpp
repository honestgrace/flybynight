#include <iostream>
#include <Windows.h>

typedef int TypeGroup;
typedef struct _MyTokenGroup
{
    int GroupCount;
    TypeGroup Groups[1];
} MyTokenGroup, *PMyTokenGroup;

// if successful, returns non-zero value (or TRUE or non-null pointer)
// if fails, returns value zero (or FALSE, or NULL pointer)
BOOL WinAPIFunctionPattern1(PMyTokenGroup* ppMyTokenGroup,
    int byte_count, int* byte_size)
{
    if (byte_count == 0 && byte_size != NULL)
    {
        // this value will be defined by the Windows API function
        int group_count = 5;

        // this calcuates the byte size of memory buffer
        // this is determined by Windows API
        *byte_size = sizeof(MyTokenGroup) +
            (group_count - 1) * sizeof(TypeGroup);
    }
    else if (ppMyTokenGroup != NULL && byte_count >= sizeof(MyTokenGroup)) //
    {
        // ppMyTokenGroup is pointer to pointer to MyTokenGroup
        PMyTokenGroup ptr = *ppMyTokenGroup;

        // calculate element count of Groups
        int count = ptr->GroupCount =
            (byte_count - sizeof(MyTokenGroup)) / sizeof(TypeGroup) + 1;

        // Windows API fills the Groups
        for (int i = 0; i < count; ++i)
            ptr->Groups[i] = (i + 1);

        return TRUE;
    }
    else
        return FALSE;
}

int main()
{
    int byte_size;
    
    // STEP 1. Query the byte size or element count of return value of Windows API function
    if (!WinAPIFunctionPattern1(NULL, 0, &byte_size))
    {
        std::cout << "Failed to query byte size " << std::endl;
        return 0;
    }

    // STEP 2. We, the programmar, have to allocate memory buffer
    PMyTokenGroup pMyTokenGroup = (PMyTokenGroup)GlobalAlloc(GPTR, byte_size);
    
    // STEP 3. We call Windows API function by passing the allocated buffer
    if (!WinAPIFunctionPattern1(&pMyTokenGroup, byte_size, &byte_size))
    {
        std::cout << "Failed to retrieve information from Windows API function" << std::endl;

        GlobalFree(pMyTokenGroup);

        return 0;
    }

    // STEP 4. We use the retrieved information.
    for (int i = 0; i < pMyTokenGroup->GroupCount; ++i)
    {
        std::cout << i << " : " << pMyTokenGroup->Groups[i] << std::endl;
    }

    // STEP 5. We have to free the allocated memory buffer.
    GlobalFree(pMyTokenGroup);

    return 0;
}
