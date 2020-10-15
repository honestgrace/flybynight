#pragma once

class matrixc11_t
{
private:
    size_t _rows;
    size_t _columns;
    void* _p = nullptr;

public:

    matrixc11_t() : _rows(0), _columns(0)
    {
        _p = nullptr;
    }

    matrixc11_t(size_t rows, size_t columns) : _rows(rows), _columns(columns)
    {
        _p = new char[rows * columns];
    }

    // copy-constructor
    matrixc11_t(const matrixc11_t& rhs)
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

    matrixc11_t(const matrixc11_t&& rhs)
    {
        // the internal of rhs is moved to this instance.
        // no dynamic memory allocation,
        // no element-wise copy operation
    }
    

    // assignment operator
    matrixc11_t& operator=(const matrixc11_t& rhs)
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

    matrixc11_t& operator +=(const matrixc11_t& rhs)  // operator+=(LR)
    {
        // operation *this += rhs  
        // no memory allocation
        // *this += rhs is to explain conceptially. Don't do that to avoid recursion.
        // You can just return *this without doing anything like this.
        return *this;
    }
    
    // this function is not actually required in the code example
    // So, commented out.
    /*
    matrixc11_t& operator +=(const matrixc11_t&& rhs)  // operator+=(RR)
    {
        // operation *this += rhs
        // no memory allocation
        return *this;
    }
    */

    int rows() const
    {
        return _rows;
    }

    int columns() const
    {
        return _columns;
    }

    ~matrixc11_t()
    {
        if (_p)
        {
            delete _p;
        }
    }

};


// The returned object of type matrixc11_t is unnamed.
// So, the value category of the return value of opertator+() is RValue.
//
// What is the type category of the return value of this function?
// The type category of the return value of this function is matrixc11_t.
// What is the value category of the return value of this function?
// The value category of the return value of this function is RValue.
matrixc11_t operator+(const matrixc11_t& L, const matrixc11_t& R) // RV operator+(LR, LR)
{
    matrixc11_t T(L.rows(), L.columns());

    // We evaluate L+R and store the result in matrix T
    return T; // T is LValue in this function block
}

// return value type category is RValue reference
//
// What is the type category of the return value of this function?
// The type category of the return value of this function is matrixc11_t&& - RValue reference.
// What is the value category of the return value of this function?
// The value category of the return value of this function is XValue or eXpiring Value.


matrixc11_t&& operator+(matrixc11_t&& L, const matrixc11_t& R) // RR operator+(RR, LR)
{
    L += R; // in-place addition;

    // Since L is named, it is LValue.
    // Its type category is RValue reference.
    return std::move(L); // Since L is Lvalue, we have to transfer its ownership using std::move.
                         // L is RValue reference, so we can transfer its ownership.
                         // L is recycled.
}

matrixc11_t && operator+(const matrixc11_t& L, matrixc11_t&& R) // RR operator+(LR, RR)
{
    R += L; // because matrix is commutative over addition +

    return std::move(R); // R is recylced.
}

matrixc11_t && operator+(matrixc11_t&& L, matrixc11_t&& R) // RR operator+(RR, RR)
{
   L += R; // in-place addition;
   return std::move(L);  // L is recycled.
}