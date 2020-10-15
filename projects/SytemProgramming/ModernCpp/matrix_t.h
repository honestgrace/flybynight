#pragma once

#include <vector>
#include <iostream>
#include <type_traits>

//
// This requires c17 standard
// Property Pages => C / C++ => Language => C++ Language Standard => ISO C++17 Standard (/std:c++17)
//

template<typename ElementType>
class matrix_t
{
public:
    using value_type = ElementType;
    using array_type = std::vector<ElementType>;

private:
    size_t m_rows, m_columns;
    array_type m_array;

    void invalidate()
    {
        this->m_rows = 0;
        this->m_columns = 0;
    }

public:
    matrix_t() : m_rows{ 0 }, m_columns{ 0 } {}

    matrix_t(size_t rows, size_t columns):
        m_rows{rows}, m_columns{columns}, m_array(rows*columns)
    {
        // we dynamically allocated memory for rows * columns elements
    }

    matrix_t(const matrix_t& rhs):
        m_rows{rhs.m_rows}, m_columns{rhs.m_columns},
        m_array{ rhs.m_array } 
    {
        // copy constructor, we dynamically allocated and copied from rhs
    }

    matrix_t& operator=(const matrix_t& rhs)
    {
        if (this != std::addressof(rhs))
        {
            // release existing memory and reallocate memory for new elements
            this->m_array.resize(rhs.m_array.size());
            this->m_array = rhs.m_array; // copy from rhs
            this->m_rows = rhs.m_rows;
            this->m_columns = rhs.m_columns;
        }
        return *this;
    }

    // move constructor
    matrix_t(matrix_t&& rhs) : m_rows{ rhs.m_rows }, m_columns{ rhs.m_columns },
        m_array{std::move(rhs.m_array)}
    {
        // please note that we moved elements of rhs.m_array 
        // to this->m_array

        // after move, rhs is invalid
        rhs.invalidate();
    }

    matrix_t& operator=(matrix_t && rhs)
    {
        if (this != std::addressof(rhs))
        {
            // remove existing memory
            this->m_array.clear();
            this->m_array = std::move(rhs.m_array);
            this->m_rows = rhs.m_rows;
            this->m_columns = rhs.m_columns;

            // after move, rhs is invalid
            rhs.invalidate();
        }
        return *this;
    }
    
    ElementType& operator()(size_t i, size_t j)
    {
        return this->m_array[i * m_columns + j];
    }

    const ElementType& operator()(size_t i, size_t j) const
    {
        return this->m_array[i * m_columns + j];
    }

    // in-place matrix addition
    matrix_t& operator+=(const matrix_t& rhs)
    {
        if (this->m_rows != rhs.m_rows || this->m_columns != rhs.m_columns)
            throw std::exception("Invalid Matrix Addition");
        else if (this->m_array.empty())
            return *this;
        else
        {
            for (size_t k{}; k < rhs.m_array.size(); ++k)
            {
                this->m_array[k] += rhs.m_array[k];
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const matrix_t& mat)
    {
        if (mat.m_array.empty())
        {
            os << "{ }"; return os;
        }
        else
        {
            for (size_t i = 0; i < mat.m_rows; ++i)
            {
                for (size_t j = 0; j < mat.m_columns; j++)
                {
                    os << mat.m_array[i * mat.m_columns + j];
                    if ((j + 1) < mat.m_columns)
                        os << ", ";
                    else
                        os << std::endl;
                }
            }
            return os;
        }
    }

    template<typename LeftType, typename RightType>
    friend decltype(auto) operator+(LeftType&& L, RightType&& R)
    {
        // LeftType is an RValue Reference
        if constexpr (std::is_rvalue_reference_v<LeftType>
            && !std::is_rvalue_reference_v<RightType>)
        {
            L += R; // directlyadd to L
            return std::forward<LeftType>(L);
        }
        // RightType is an RValue refrence
        else if constexpr (!std::is_rvalue_reference_v<LeftType>
            && std::is_rvalue_reference_v<RightType>)
        {
            R += L; // directlyadd to R
            return std::forward<LeftType>(R);
        }
        else // both LeftType and RightType are not RValue Reference
        {
            // create an instance of matrix_t 
            // and copy from L
            auto T = L;

            T += R; // evaluate L + R and store it to T
            return T;
        }
    }
};  // end of class matrix_t