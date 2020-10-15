#pragma once

#include <Windows.h>
#include <iostream>
#include <exception>
#include <string>
#include <sstream>

#include <sddl.h> // security descriptor description language
#include <AclAPI.h> // access control api

#include "smart_handle.h"

#pragma comment(lib,"advapi32.lib")

namespace tpf
{
    class WinAPIException : public std::runtime_error 
    {
    private:
        std::string filename;
        int line{};
        int step{};
        int error_code{};

    public:
        friend std::ostream& operator<<(std::ostream& os, const WinAPIException& wae)
        {
            std::ostringstream error_msg;

            if (wae.filename != "")
                error_msg << "file: " << wae.filename << " ";
            if (wae.line != 0)
                error_msg << "line: " << wae.line << " ";
            if (wae.step != 0)
                error_msg << "step: " << wae.step << " ";
            if (wae.what() != 0)
                error_msg << "function: " << wae.what() << " ";
            if (wae.error_code != 0)
                error_msg << "error: " << wae.error_code;

            std::string msg = error_msg.str();

            while (!msg.empty() && msg.back() == ' ')
                msg.pop_back();

            os << msg;
            return os;
        }

        WinAPIException(const char* fname = "",
            int line_no = 0,
            const char * msg="") : 
            std::runtime_error(msg), filename(fname), line(line_no) {}

        WinAPIException(const WinAPIException&) = default;  // copy constructor default
        WinAPIException& operator=(const WinAPIException&) = default; // assignment operator default
        //WinAPIException(const WinAPIException&&) = default;  // copy constructor default
        //WinAPIException& operator=(const WinAPIException&&) = default; // assignment operator default

        void increase_step(int line = 0)
        {
            this->line = line;
            ++this->step;
        }

        void set_step(int step, int line = 0)
        {
            this->line = line;
            this->step = step;
        }

        void retrieve_error(int line = 0)
        {
            this->line = line;
            this->error_code = GetLastError();
        }

        void retrieve_error_throw(int line = 0)
        {
            this->line = line;
            this->error_code = GetLastError();
            throw *this;
        }

        void set_error(int error, int line = 0)
        {
            this->line = line;
            this->error_code = error;
        }

        void set_error_throw(int error, int line = 0)
        {
            this->line = line;
            this->error_code = error;
            throw* this;
        }

        // if no error, returns TRUE
        // if error, returns FALSE
        operator bool() const
        {
            return (this->error_code == 0);
        }

        void throw_if_error()
        {
            if (this->error_code != 0)
                throw* this;
        }
    };
}

// this macro should be inside of a function, or member function, static or nonstatic
#define WAE_DeclException(wae) tpf::WinAPIException wae{__FILE__, __LINE__, __FUNCTION__}
#define WAE_IncreaseStep(wae) wae.increase_step(__LINE__)
#define WAE_SetStep(wae, step) wae.step((step), __LINE__)
#define WAE_RetrieveError(wae) wae.retrieve_error(__LINE__)
#define WAE_RetrieveErrorThrow(wae) wae.retrieve_error_throw(__LINE__)
#define WAE_SetError(wae, error) wae.set_error((error), __LINE__)
#define WAE_SetErrorThrow(wae, error) wae.set_error_throw((error), __LINE__)
#define WAE_ThrowIfErroThrow(wae, error) wae.throw_iferror_throw((error), __LINE__)