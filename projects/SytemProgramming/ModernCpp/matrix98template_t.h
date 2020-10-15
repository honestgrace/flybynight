#pragma once
#include <vector>
#include <iostream>

template <typename ElementType>
class matrix98template_t
{
private:
    size_t m_rows, m_columns;
    ElementType * m_array = NULL;

public:
    
    matrix98template_t() :
        m_rows{0}, m_columns{0}, m_array{0}
    {
    }

    ~matrix98template_t()
    {
        if (m_array)
        {
            delete [] m_array;
        }
    }

    matrix98template_t(size_t rows, size_t columns)
        : m_rows{ rows }, m_columns{ columns }
    {
        m_array = new ElementType[m_rows * m_columns];
    }
    
    ElementType& operator()(size_t i, size_t j)
    {
        size_t index = i * m_columns + j;
        return m_array[index];
    }

    const ElementType& operator()(size_t i, size_t j) const
    {
        size_t index = i * m_columns + j;
        return m_array[index];
    }

    // Copy-Constructor
    matrix98template_t(const matrix98template_t& rhs)
    {
        if (m_array)
        {
            delete[] m_array;
            m_array = NULL;
        }

        m_rows = rhs.m_rows;
        m_columns = rhs.m_columns;
        size_t count = m_rows * m_columns;
        m_array = new ElementType[count];

        for (int i = 0; i < count; i++)
        {
            m_array[i] = rhs.m_array[i];
        }
    }

    matrix98template_t& operator=(const matrix98template_t& rhs)
    {
        if (m_array)
        {
            delete [] m_array;
            m_array = NULL;
        }

        m_rows = rhs.m_rows;
        m_columns = rhs.m_columns;
        size_t count = m_rows * m_columns;
        m_array = new ElementType[count];

        for (int i = 0; i < count; i++)
        {
            m_array[i] = rhs.m_array[i];
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const matrix98template_t& mat)
    {
        if (!mat.m_array)
        {
            os << "empty" << std::endl;
        }
        else
        {
            for (int i = 0; i < mat.m_rows; i++)
                for (int j = 0; j < mat.m_columns; j++)
                    os << "mat(" << i << "," << j << ") = " << mat(i, j) << std::endl;
        }
        return os;
    }

    friend matrix98template_t operator+(const matrix98template_t& L, const matrix98template_t& R)
    {
        matrix98template_t T{ L.m_rows, L.m_columns };

        for (int i = 0; i < L.m_rows; i++)
            for (int j = 0; j < L.m_columns; j++)
                T(i, j) = L(i, j) + R(i, j);

        return T;
    }
};

