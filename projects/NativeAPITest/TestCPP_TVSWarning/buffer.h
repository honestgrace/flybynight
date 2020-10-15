/*++

Copyright (c) 1996  Microsoft Corporation

Module Name:

    buffer.hxx

Abstract:

    Another buffer class

Author:

    Murali R. Krishnan (MuraliK) 9-July-1996  Recreated from old buffer.hxx

Revision History:

--*/

#ifndef _BUFFER_HXX_
#define _BUFFER_HXX_

#include <crtdbg.h>

/*************************************************************************

    NAME:       BUFFER (buf)

    SYNOPSIS:   A resizable object which lives in the application heap.

                Note that a buffer may have size 0, in which case it
                keeps no allocated storage.

    INTERFACE:  BUFFER()        - Constructor, naming initial size in bytes

                QuerySize()     - return size in bytes
                QueryPtr()      - return pointer to data buffer

                Resize()        - resize the object to the given number
                                  of bytes.  Returns TRUE if the resize was
                                  successful; otherwise returns FALSE (use
                                  GetLastError for error code)

**************************************************************************/

//
// Starting off with space for a '\0' greatly simplifies STRA and STRU implementation
//
#define INLINED_BUFFER_LEN (sizeof(WCHAR))

//
//  Align this class to 16, which is also the alignment chosen by the heap
//  manager, to avoid alignment faults when the internal buffer, which is at
//  the beginning of this class, is used to store aligned data.
//
#pragma warning( push )
#pragma warning( disable: 4324 )    //  structure was padded due to __declspec(align())
__declspec( align( 8 ) )
class BUFFER 
{

public:

    BUFFER()
      : m_pb( m_rgb ),
        m_cb( INLINED_BUFFER_LEN ),
        m_fIsDynAlloced( false )
    {
        //
        // do nothing
        //
    }

    BUFFER(
        PBYTE pbInit, 
        DWORD cbInit
    ) : m_pb( pbInit ),
        m_cb( cbInit ),
        m_fIsDynAlloced( false )
    /*++
        Description:

            Instantiate BUFFER, initially using pbInit as buffer
            This is useful for stack-buffers and inline-buffer class members 
            (see STACK_BUFFER and INLINE_BUFFER_INIT below)

            BUFFER does not free pbInit

        Arguments:

            pbInit - initial buffer to use
            cbInit - sizeof pbInit, in bytes

        Returns:
            
            n/a

    --*/
    {
        _ASSERTE( NULL != pbInit );
        _ASSERTE( cbInit > 0 );
#if DBG
        SecureZeroMemory( pbInit, cbInit );
#else
        m_pb[0] = '\0';
#endif
    }

    ~BUFFER()
    {
        if( IsDynAlloced() )
        {
            _ASSERTE( NULL != m_pb );
            HeapFree( GetProcessHeap(), 0, m_pb );
            m_pb = NULL;
            m_cb = 0;
            m_fIsDynAlloced = false;
        }
    }

    PVOID 
    QueryPtr(
        VOID
    ) const 
    { 
        return m_pb;
    }

    DWORD 
    QuerySize(
        VOID
    ) const  
    { 
        return m_cb; 
    }

    BOOL 
    Resize( 
        DWORD cbNewSize,
        BOOL  fZeroMemBeyondOldSize = FALSE
    )
    /*++
        Description:

            Resizes BUFFER

        Arguments:

            cbNewSize - size, in bytes, to grow to
            fZeroMemBeyondOldSize - whether to zero the region of memory of the
                                    new buffer beyond the original size

        Returns:
            
            BUGBUG: should switch to HRESULT for consistency's sake

            TRUE on success, FALSE on failure

    --*/
    {
        PVOID pNewMem;

        if ( cbNewSize <= m_cb )
        {
            return TRUE;
        }

        DWORD dwHeapAllocFlags = fZeroMemBeyondOldSize ? HEAP_ZERO_MEMORY : 0;

        if( IsDynAlloced() )
        {
            pNewMem = HeapReAlloc( GetProcessHeap(), dwHeapAllocFlags, m_pb, cbNewSize );
        }
        else
        {
            pNewMem = HeapAlloc( GetProcessHeap(), dwHeapAllocFlags, cbNewSize );
        }

        if( pNewMem == NULL )
        {
            SetLastError( ERROR_NOT_ENOUGH_MEMORY );
            return FALSE;
        }

        if( !IsDynAlloced() ) 
        {
            //
            // First time this block is allocated. Copy over old contents.
            //
            _ASSERTE( INLINED_BUFFER_LEN > 0 );
            CopyMemory( pNewMem, m_pb, m_cb );
            m_fIsDynAlloced = true;
        }

        m_pb = (PBYTE)pNewMem;
        m_cb = cbNewSize;

        _ASSERTE( m_pb != NULL );

        return TRUE;
    }

private:

    BOOL 
    IsDynAlloced(
        VOID
    ) const 
    {   
        return m_fIsDynAlloced; 
    }

    //
    // the default buffer object This member should be at the beginning
    // for alignment purposes.
    //
    BYTE    m_rgb[INLINED_BUFFER_LEN];
    
    //
    // is m_pb dynamically allocated?
    //
    bool    m_fIsDynAlloced;

    //
    // size of storage, as requested by client
    //
    DWORD   m_cb;

    //
    // pointer to storage
   
    PBYTE   m_pb; //

}; // class BUFFER

#pragma warning( pop )

//
// Assumption of macros below for pointer alignment purposes
//
C_ASSERT( sizeof(VOID*) <= sizeof(ULONGLONG) );

//
//  Declare a BUFFER that will use stack memory of <size>
//  bytes.  If the buffer overflows then a heap buffer will be allocated
//
#define STACK_BUFFER( _name, _size )    \
    ULONGLONG   __aqw##_name[ ( ( (_size) + sizeof(ULONGLONG) - 1 ) / sizeof(ULONGLONG) ) ]; \
    BUFFER      _name( (BYTE*)__aqw##_name, sizeof(__aqw##_name) )

//
// Macros for declaring and initializing a BUFFER that will use inline memory
// of <size> bytes as a member of an object.
//

#define INLINE_BUFFER( _name, _size )   \
    ULONGLONG   __aqw##_name[ ( ( (_size) + sizeof(ULONGLONG) - 1 ) / sizeof(ULONGLONG) ) ]; \
    BUFFER      _name;

#define INLINE_BUFFER_INIT( _name )     \
    _name( (BYTE*)__aqw##_name, sizeof( __aqw##_name ) )

#endif // _BUFFER_HXX_
