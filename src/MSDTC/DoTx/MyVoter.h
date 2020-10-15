class MyVoter : public ITransactionVoterNotifyAsync2
{
public:
    MyVoter( bool bVoteYes, HANDLE phase2Event );
    ~MyVoter();
    virtual STDMETHODIMP QueryInterface( REFIID i_iid, LPVOID FAR* o_ppv );
    virtual STDMETHODIMP_ (ULONG) AddRef();
    virtual STDMETHODIMP_ (ULONG) Release();
    STDMETHODIMP VoteRequest(void);
    STDMETHODIMP Committed(
        BOOL fRetaining,
        XACTUOW * pNewUOW,
        HRESULT hr
        );
    STDMETHODIMP Aborted(
        BOID * pboidReason,
        BOOL fRetaining,
        XACTUOW * pNewUOW,
        HRESULT hr
        );
    STDMETHODIMP HeuristicDecision(
        DWORD dwDecision,
        BOID * pboidReason,
        HRESULT hr
        );
    STDMETHODIMP Indoubt(void);
    
	HRESULT CreateVoter(ITransactionVoterFactory2* pVoterFactory, ITransaction* pTx);
	void SetBallot( ITransactionVoterBallotAsync2* ballotAsync );
    HRESULT VoteNow();

	ITransactionVoterBallotAsync2* m_BallotAsync;    
private:
    LONG m_cRef;
    HANDLE m_phase2Event;
    BOOL m_bVoteYes;
    CRITICAL_SECTION m_CritSec;

};
