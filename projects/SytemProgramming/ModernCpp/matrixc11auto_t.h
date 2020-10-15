#pragma once
#include <type_traits>
class matrixc11auto_t
{
private:
    size_t _rows;
    size_t _columns;
    void* _p = nullptr;

public:

    matrixc11auto_t() : _rows(0), _columns(0)
    {
        _p = nullptr;
    }

    matrixc11auto_t(size_t rows, size_t columns) : _rows(rows), _columns(columns)
    {
        _p = new char[rows * columns];
    }

    // copy-constructor
    matrixc11auto_t(const matrixc11auto_t& rhs)
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

    matrixc11auto_t(const matrixc11auto_t&& rhs)
    {
        // the internal of rhs is moved to this instance.
        // no dynamic memory allocation,
        // no element-wise copy operation
    }


    // assignment operator
    matrixc11auto_t& operator=(const matrixc11auto_t& rhs)
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

    matrixc11auto_t& operator +=(const matrixc11auto_t& rhs)  // operator+=(LR)
    {
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

    ~matrixc11auto_t()
    {
        if (_p)
        {
            delete _p;
        }
    }

};

// C++14 Standard introduce decltype(auto) as return-value type specifier.
using r_ref_t = std::true_type;
using l_ref_t = std::false_type;

decltype(auto) matrix_addition(const matrixc11auto_t& L, const matrixc11auto_t& R, l_ref_t, l_ref_t)
{
    matrixc11auto_t T(L.rows(), L.columns());
    return T;
}

decltype(auto) matrix_addition(matrixc11auto_t&& L, const matrixc11auto_t& R, r_ref_t, l_ref_t)
{
    L += R;
    return std::move(L);
}

decltype(auto) matrix_addition(const matrixc11auto_t& L, matrixc11auto_t&& R, l_ref_t, r_ref_t)
{
    R += L;
    return std::move(R);
}

decltype(auto) matrix_addition(matrixc11auto_t&& L, matrixc11auto_t&& R, r_ref_t, r_ref_t)
{
    L += R;
    return std::move(L);
}

//template<typename LeftType, typename RightType>
//decltype(auto) operator+(LeftType&& L, RightType&& R)
//{
//    // BUGBUG. For some reasons I got complication error. tempoary put a wrong one which makes it compilable anyway.
//    return std::move(L);
//
//    /*return matrix_addition(
//        std::forward<LeftType>(L),
//        std::forward<RightType>(R),
//        std::is_rvalue_reference<LeftType>{},
//        std::is_rvalue_reference<RightType>{});*/
//}