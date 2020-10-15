class MyResourceManager : public IResourceManagerSink
{
public:
    MyResourceManager( GUID guid, char* name );
    ~MyResourceManager();
    virtual STDMETHODIMP QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv );
    virtual STDMETHODIMP_ (ULONG) AddRef();
    virtual STDMETHODIMP_ (ULONG)Release();
    virtual STDMETHODIMP TMDown();
    STDMETHODIMP Enlist( IResourceManagerFactory2* pRmFactory2, ITransaction* pTxLocal, ITransactionResourceAsync* pResAsync1, ITransactionEnlistmentAsync** ppEnlistAsync );

private:
    IResourceManager* m_pIRm;
    GUID m_guid;
    char* m_name;
    CRITICAL_SECTION m_CritSec;
    LONG m_cRef;

};

