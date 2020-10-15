#pragma once

#include <functional>
#include <type_traits>
#include <Windows.h>

namespace tpf
{
    template<typename HType, // handle type
        HType invalid_value, // either NULL or INVALID_HANDLE_VALUE
        typename DType>      // deleter type
        class SmartHandleT
    {
    protected:
        HType m_hHandle;
        DType m_Deleter;

    public:
        SmartHandleT(DType deleter) :
            m_hHandle(invalid_value), m_Deleter(deleter) {}

        SmartHandleT(HType handle, DType deleter) :
            m_hHandle(handle), m_Deleter(deleter) {}

        SmartHandleT(DType deleter, HType handle) :
            m_hHandle(handle), m_Deleter(deleter) {}

        bool IsValid() const
        {
            return this->m_hHandle != invalid_value;
        }

        // implicit conversion to bool
        operator bool() const
        {
            return IsValid();
        }

        template<typename T,
        typename = std::enable_if_t<!std::is_same<T, bool>::value>>
        operator T() const
        {
            return static_cast<T>(this->m_hHandle);
        }

        HType Handle() { return this->m_hHandle; }
        const HType Handle() const { return this->m_hHandle; }

        HType* operator&() { return &this->m_hHandle; }
        HType& operator*() { return this->m_hHandle;  }
        operator HType() { return this->m_hHandle; }

        void Close()
        {
            if (IsValid())
            {
                this->m_Deleter(this->m_hHandle);
                this->m_hHandle = invalid_value;

                std::cout << "Handle closed" << std::endl;
            }
        }

        HType Release()
        {
            HType handle = this->m_hHandle;
            this->m_hHandle = invalid_value;
            
            return handle;
        }

        // disable copy constructor
        SmartHandleT(const SmartHandleT&) = delete;

        // disable copy assignment
        SmartHandleT& operator=(const SmartHandleT&) = delete;

        // move constructor
        SmartHandleT(SmartHandleT&& sh)
        {
            this->m_hHandle = sh.Release();
            this->m_Deleter = std::move(sh.m_Deleter);
        }

        // move copy assignment
        SmartHandleT& operator=(SmartHandleT&& sh)
        {
            this->Close();
            this->m_hHandle = sh.Release();
            this->m_Deleter = std::move(sh.m_Deleter);
        }

        constexpr size_t ByteSize() const
        {
            return sizeof(HType);
        }

        ~SmartHandleT() { this->Close(); }
    };
    using SmartHandle = SmartHandleT<HANDLE, INVALID_HANDLE_VALUE,
        std::function<decltype(CloseHandle)>>;

    using TokenHandle = SmartHandleT<HANDLE, NULL,
        std::function<decltype(CloseHandle)>>;
    
    using LocalMem = SmartHandleT<HLOCAL, NULL,
        std::function<decltype(LocalFree)>>;

    using GlobalMem = SmartHandleT< HGLOBAL, NULL,
        std::function<decltype(GlobalFree)>>;

    using SmartSid =
        SmartHandleT <PSID, NULL, std::function<decltype(FreeSid)>>;

    /*
    template<typename HType>
    constexpr HType InvalidHandleValue = static_cast<HType>(INVALID_HANDLE_VALUE);

    template<typename HType>
    constexpr HType HandleNull = static_cast<HType>(NULL);
    
    using SmartHandle = SmartHandleT<HANDLE, InvalidHandleValue<HANDLE>,
        std::function<decltype(CloseHandle)>>;

    using LocalMem = SmartHandleT<HLOCAL, HandleNull<HLOCAL>,
        std::function<decltype(LocalFree)>>;
    */
}

#define WinAPI_TokenHandle(hToken) tpf::TokenHandle hToken(CloseHandle)
#define WinAPI_SmartSid(pSid) tpf::SmartSid pSid(FreeSid)
#define WinAPI_GlobalMem(pMem) tpf::GlobalMem pMem(GlobalFree)
#define WinAPI_LocalMem(pMem) tpf::LocalMem(LocalFree)