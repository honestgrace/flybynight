#pragma once

class matrix98_t
{
private:
    size_t _rows;
    size_t _columns;
    void* _p = nullptr;

public:

    matrix98_t() : _rows(0), _columns(0)
    {
        _p = nullptr;
    }

    matrix98_t(size_t rows, size_t columns) : _rows(rows), _columns(columns)
    {
        _p = new char[rows * columns];
    }

    // copy-constructor
    matrix98_t(const matrix98_t& rhs)
    {
        if (_p)
        {
            delete _p;
            _p = nullptr;
        }
        _rows = rhs.rows();
        _columns = rhs.columns();

        // dynamically allocates memory for its elements;
        _p = new char[_rows * _columns];
    }

    // assignment operator
    matrix98_t& operator=(const matrix98_t& rhs)
    {
        if (_p)
        {
            delete _p;
            _p = nullptr;
        }

        _rows = rhs.rows();
        _columns = rhs.columns();

        // dynamically allocates memory for its elements;
        _p = new char[_rows * _columns];

        return *this;
    }

    int rows() const
    {
        return _rows;
    }

    int columns() const
    {
        return _columns;
    }

    ~matrix98_t()
    {
        if (_p)
        {
            delete _p;
        }
    }

};


// The returned object of type matrix98_t is unnamed.
// So, the value category of the return value of opertator+() is RValue.
//
// What is the type category of the return value of this function?
// The type category of the return value of this function is matrix98_t.
// What is the value category of the return value of this function?
// The value category of the return value of this function is RValue.
matrix98_t operator+(const matrix98_t& L, const matrix98_t& R) // RV operator+(LR, LR)
{
    matrix98_t T(L.rows(), L.columns());

    // We evaluate L+R and store the result in matrix T
    return T; // T is LValue in this function block
}