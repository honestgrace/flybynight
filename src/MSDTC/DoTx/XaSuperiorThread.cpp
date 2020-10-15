
// I copied this out of a executable that does lots of other stuff and only does xa superior stuff when
// a command line argument is specified. So it is not a complete source file...


Stuff to do to initialize the XA code...

{
    int xaerr = XA_OK;
    loopCount++;

    printf( "Doing xaopen...\n" );

    // the format of the DNS is very specific. The value of the RmRecoveryGuid is something that needs to be persistent.
    xaerr = msqlsrvxa1.xa_open_entry("TM=xaresumetx,RmRecoveryGuid=8FABAA36-FDC5-4bb7-9DFD-DF1CF312A000,Timeout=30000", myRmId, 0 );
    if ( XA_OK != xaerr )
    {
        printf( "Error %d from xa_open\n", xaerr );
        return -1;
    }
    printf( "XaOpen succeeded.\n" );

    // Once we do this xa_open, we can start making other xa calls. These are done by the XASuperiorThreadRoutine here. Yours
    // will be different because you will be doing things in response to requests for the XA DLL and application that is running
    // inside the CorpNet.

    printf( "Firing up %d worker threads...\n", xathreads );

    HANDLE threadWaitHandle = NULL;
    for (DWORD thread=0;thread<xathreads;thread++)
    {
        HANDLE tempHandle = CreateThread( NULL, 0, XASuperiorThreadRoutine, (LPVOID) thread, 0, NULL );
        if ( NULL == tempHandle )
        {
            printf( "Error %d creating thread.\n", GetLastError() );
            return -1;
        }

        if ( 0 == thread )
        {
            threadWaitHandle = tempHandle;
        }

    }

    DWORD waitResult1 = WaitForSingleObject( threadWaitHandle, INFINITE );
    if ( WAIT_FAILED == waitResult1 )
    {
        printf("Wait for thread handle FAILED, error = %d.\n", GetLastError() );
        return -1;
    }
    else if ( WAIT_OBJECT_0 != waitResult1 )
    {
        printf("Wait for thread handle did not complete successfully - 0x%x, error = %d.\n", waitResult1, GetLastError() );
    }


    printf( "Hit any key to loop again.  Hit CTRL-C to exit.\n" );
    getchar();
    continue;
}

DWORD WINAPI XASuperiorThreadRoutine( LPVOID lpParameter )
{
    DWORD loopCount = 0;
    DWORD threadNum = (DWORD) lpParameter;
    ITransaction* pTxThread = NULL;
    XID threadXid;
    HRESULT hr = S_OK;
    IXATransLookup2* pThreadXaLookup = NULL;
    MyResourceAsync* pThreadResAsync1 = NULL;

    printf( "Thread %d - Getting the ITransaction from the XID.\n", threadNum );
    IXATransLookup2 * pXaLookup = NULL;
    hr = pTxDisp->QueryInterface( IID_IXATransLookup2, (void**) &pThreadXaLookup );
    if ( FAILED(hr) )
    {
        printf( "Thread %d - Error %x from QI for IID_IXATransLookup\n", threadNum, hr );
        exit (-1);
    }

    while (TRUE)
    {
        int xaerr = XA_OK;
        loopCount++;

        // Sleep anywhere from 0 millisends to 4 seconds.
        prepareReqThreadSleepTime = (rand() & 0xFFF)/(threadNum+1);

        memset( &threadXid, 0, sizeof(XID) );

        char szLoopCount[20] = "";
        char szThreadNum[20] = "";
        threadXid.formatID = 0x123456;
        strcpy( threadXid.data, "JCARLEYXIDGTRID" );
        sprintf( szThreadNum, "%d", threadNum );
        strcat( threadXid.data, szThreadNum );
        sprintf( szLoopCount, "%d", loopCount );
        strcat( threadXid.data, szLoopCount );
        threadXid.gtrid_length = strlen( threadXid.data );

        printf( "Thread %d - Doing XA Superior for XID %s\n", threadNum, threadXid.data );

        xaerr = msqlsrvxa1.xa_start_entry(&threadXid, myRmId, 0);
        if ( XA_OK != xaerr )
        {
            printf( "Thread %d - Error %d from xa_start\n", threadNum, xaerr );
            exit (-1);
        }
        printf( "Thread %d - XaStart succeeded.\n", threadNum );


        hr = pThreadXaLookup->Lookup( &threadXid, &pTxThread );
        if ( FAILED(hr) )
        {
            printf( "Thread %d - Error %x from QI for IXATransLookup::LookUp\n", threadNum, hr );
            exit (-1);
        }

        // Here is where you would do the work of enlisting on DTC using the OleTx interfaces
        // or invoke SQL or whatever because pTxThread is the ITransaction for the transaction.

        // The following is what you would do when your the XA DLL in the CorpNet told you to
        // prepare/commit the transaction.

        if ( doXaSPC )
        {
            printf( "Thread %d - Trying XA SinglePhaseCommit.\n", threadNum );
            xaerr = msqlsrvxa1.xa_commit_entry(&threadXid, myRmId, TMONEPHASE);
            if ( XA_OK != xaerr )
            {
                printf( "Thread %d - Error %d from xa_commit.\n", threadNum, xaerr );
                exit (-1);
            }
            printf( "Thread %d - XaCommit (SPC) succeeded.\n", threadNum );

        }
        else
        {
            xaerr = msqlsrvxa1.xa_prepare_entry(&threadXid, myRmId, 0);
            if ( XA_OK != xaerr )
            {
                printf( "Thread %d - Error %d from xa_prepare.\n", threadNum, xaerr );
                exit (-1);
            }
            printf( "Thread %d - XaPrepare succeeded.\n", threadNum );

            xaerr = msqlsrvxa1.xa_commit_entry(&threadXid, myRmId, 0);
            if ( XA_OK != xaerr )
            {
                printf( "Thread %d - Error %d from xa_commit.\n", threadNum, xaerr );
                exit (-1);
            }
            printf( "Thread %d - XaCommit succeeded.\n", threadNum );
        }

        pTxThread->Release();
        pTxThread = NULL;

tryAgainXa:
        if ( !continuous )
        {
            printf( "Thread %d - Hit any key to loop again.  Hit CTRL-C to exit.\n", threadNum );
            getchar();
        }
        Sleep(1000);
        continue;
    }
}

