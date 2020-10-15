class Outcome : public ITransactionOutcomeEvents
{
public:

    Outcome();
    ~Outcome();

    virtual STDMETHODIMP QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv );
    virtual STDMETHODIMP_ (ULONG) AddRef();
    virtual STDMETHODIMP_ (ULONG)Release();
    virtual STDMETHODIMP Aborted(
                            BOID * pboidReason,
                            BOOL fRetaining,
                            XACTUOW * pNewUOW,
                            HRESULT hr
                            );
    virtual STDMETHODIMP Committed(
                            BOOL fRetaining,
                            XACTUOW * pNewUOW,
                            HRESULT hr
                            );
    virtual STDMETHODIMP HeuristicDecision(
                            DWORD dwDecision,
                            BOID * pboidReason,
                            HRESULT hr
                            );
    virtual STDMETHODIMP Indoubt(void);

	STDMETHODIMP CreateOutcomeConnectionPoint(ITransaction* pTransaction);

private:
    LONG m_cRef;
	DWORD m_dwOutcomeCookie;
	IConnectionPoint* m_pConnPt;
	ITransaction *m_pTx;


};

