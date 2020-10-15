#include "stdafx.h"

#include <iostream>
#include <string>
using namespace std;

/*
class APPLICATION_KEY
{
public:
    APPLICATION_KEY(
        VOID
    ) : INLINE_STRU_INIT(m_struKey)
    {
    }

    HRESULT
        Initialize(
            _In_ LPCWSTR pszKey
        )
    {
        return m_struKey.Copy(pszKey);
    }

    BOOL
        GetIsEqual(
            const APPLICATION_KEY * key2
        ) const
    {
        return m_struKey.Equals(key2->m_struKey);
    }

    DWORD CalcKeyHash() const
    {
        return Hash(m_struKey.QueryStr());
    }

private:
    INLINE_STRU(m_struKey, 1024);
};

class APPLICATION
{
public:

    APPLICATION()
    {
    }

    APPLICATION_KEY *
        QueryApplicationKey()
    {
        return &m_applicationKey;
    }

    VOID
        SetAppOfflineFound(
            BOOL found
        )
    {
        m_fAppOfflineFound = found;
    }

    BOOL
        AppOfflineFound()
    {
        return m_fAppOfflineFound;
    }

    VOID
        ReferenceApplication() const
    {
        InterlockedIncrement(&m_cRefs);
    }

    VOID
        DereferenceApplication() const
    {
        if (InterlockedDecrement(&m_cRefs) == 0)
        {
            delete this;
        }
    }

    ~APPLICATION() 
    {
    }

    HRESULT
        Initialize(
            _In_ LPCWSTR  pszApplication,
            _In_ LPCWSTR  pszPhysicalPath
        ) 
    {
        m_applicationKey.Initialize(pszApplication);
        return S_OK;
    }

private:
    mutable LONG         m_cRefs;
    APPLICATION_KEY      m_applicationKey;
    BOOL                 m_fAppOfflineFound;
};

class APPLICATION_HASH : public HASH_TABLE<APPLICATION, APPLICATION_KEY *>
{
public:
    APPLICATION_HASH()
    {
    }
    
    APPLICATION_KEY *
        ExtractKey(
            APPLICATION *pApplication
        )
    {
        return pApplication->QueryApplicationKey();
    }

    DWORD
        CalcKeyHash(
            APPLICATION_KEY *key
        )
    {
        return key->CalcKeyHash();
    }

    BOOL
        EqualKeys(
            APPLICATION_KEY *key1,
            APPLICATION_KEY *key2
        )
    {
        return key1->GetIsEqual(key2);
    }

    VOID
        ReferenceRecord(
            APPLICATION *pApplication
        )
    {
        pApplication->ReferenceApplication();
    }

    VOID
        DereferenceRecord(
            APPLICATION *pApplication
        )
    {
        pApplication->DereferenceApplication();
    }

private:
    APPLICATION_HASH(const APPLICATION_HASH &)
    {
    }
    void operator=(const APPLICATION_HASH &)
    {
    }
}; 

void HASH_TABLE()
{
    APPLICATION a, b, c;
    a.Initialize(L"1", L"abc");
    b.Initialize(L"2", L"abc");
    c.Initialize(L"3", L"abc");

    APPLICATION_HASH hash;
    hash.Initialize(100);
    hash.InsertRecord(&a);
    hash.InsertRecord(&b);
    hash.InsertRecord(&c);

    APPLICATION *p = nullptr;
    hash.FindKey(a.QueryApplicationKey(), &p);
    if (p != nullptr)
    {
        cout << "Found key" << endl;
    }
} */

int global_index = 0;

class RecordKey
{
public:
    RecordKey()
    {
    }

    void Initialize(WCHAR* pData)
    {
        m_struKey.Copy(pData);
    }

    DWORD CalcKeyHash() const
    {
        return Hash(m_struKey.QueryStr());
    }

private:
    int m_key;
    INLINE_STRU(m_struKey, 1024);
};

class Record
{
public:
    STRU * m_pData;

    Record(WCHAR* pData)
    {
        m_pData = new STRU(pData, 255);
        m_key.Initialize(m_pData->QueryStr());        
    }
    ~Record()
    {
        delete m_pData;
    }
    RecordKey * GetKey()
    {
        return &m_key;
    }

private:
    RecordKey m_key;
};

class Test : public HASH_TABLE<Record, RecordKey *>
{
public:
    Test()
    {
    }

    RecordKey * ExtractKey(Record *pApplication)
    {
        return pApplication->GetKey();
    }

    DWORD CalcKeyHash(RecordKey *key)
    {
        return (key->CalcKeyHash());
    }

    BOOL EqualKeys(RecordKey *key1, RecordKey *key2)
    {
        return key1 == key2;
    }

    VOID ReferenceRecord(Record *pApplication)
    {
        // do nothing
    }

    VOID DereferenceRecord(Record *pApplication)
    {
        // do nothing
    }

private:
    Test(const Test &)
    {
    }
    void operator=(const Test &)
    {
    }
};

void IISLibTest()
{
    Test hashTable;
    hashTable.Initialize(555);
    Record data(L"foo");
    hashTable.InsertRecord(&data);

    Record data2(L"bar");
    hashTable.InsertRecord(&data2);

    Record data3(L"zzz");
    hashTable.InsertRecord(&data3);

    Record *p = nullptr;
    hashTable.FindKey(data.GetKey(), &p);

    wcout << p->m_pData->QueryStr() << endl;
}

