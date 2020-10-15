// ModernCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "matrix98_t.h"
#include "matrix98template_t.h"
#include "matrixc11_t.h"
#include "matrix_t.h"

/*
Do not get confused LValue with LValue reference, RValue with RValue reference.

1. LValue - value category
    - Any object in C++ that is named by the programmer is LValue.
    - LValue is initialzied with RValue.
    - The programmer should concern about the lifetime of the LValue.
        in other words, the programmer should be careful of the owenrshi of LValue.
    - LValue has its ownership.

2. RValue - value category
    - The programmer never have a chance to name RValue.
    - RValue is never initialized. It itself is a value.
    - 3, 4.5, "a", true, literals in C++ are RValue.

3. LValue reference - type category
    - is a reference, which references to an LValue object.
    - is an alias to LValue, it does have the ownership of the object it references to.
    
4. RValue reference - type category
    - is a reference, which references to an RValue.
    - is a standalone object, it has the sole ownership of the object it references to.
    - RValue refrence has the ownership of the object that it references to.
    - RValue refrence is meant to transfer the ownership of the object it references to.

int a = 5;
    "a" is a named object, so "a" is LValue.
    "5" is unnamed object, so "5" is RValue.
    "5" is used to initialize LValue "a"
    so, "5" is an RValue.

int sum(int a, int b) { int c = a + b; return c; }
int d = sum(5, 6);

    What is the return value type of sum()?

    The "type" of the return value of sum() is "int"

    What is the value category of the function sum()?

    The value category of the function sum() is RValue!!!

int& sum(int& a, int b) { a += b; return a; }

    What is the value category of sum()?

    LValue reference? No!!!

    The type category of sum() is "int&" or LValue reference.
    The value category of sum() is LValue!!!
    

*/

int pro() { return 5; }

int& sum(int& a, int b) { a += b; return a; }

int sum2(int&& a, int& b) { return int(); }  // int &&a => RValue reference

void test_rvalue_lvalue()
{
    // Test sum
    int result = 5;
    int r = sum(result, 6);
    std::cout << result << " " << r << std::endl;

    // Test sum2
    int a = 5;
    int b = 5;

    //int c = sum2(a, b); // it does not compile.
                          // because int&& a requires ownership.

    int a2 = sum2(std::move(a), b); // this is correct!!
    int b2 = sum2(a + 0, b); // this is correct!! "a+0" is RValue

    b = 6;

    std::cout << b2 << std::endl;

    int d = sum2(pro(), b);  // this works
        // pro() returns a value of type int, that is not ownend by any object.
        // the value category of return value of pro() is RValue.
        // the type category of return value of pro() is int.

    int test = 555;
    // int&& test2 = test; // this does not work
    int&& test2 = std::move(test);
}

size_t rows = 5, columns = 5;
matrix98_t X(rows, columns);
matrix98_t Y(rows, columns);
matrix98_t Z = X + Y;

size_t rows2 = 5, columns2 = 5;
matrix98_t X2(rows2, columns2);
matrix98_t Y2(rows2, columns2);
matrix98_t Z2; // C intially does not have allocated memory

/*
    FACT 1. D is copy-constructed. So the copy constructor is called.
    FACT 2. A, B, C are Lvalues, because they are named.
    FACT 3. To evaluate A + B, opertator+() is called.
    FACT 4. opertator+() returns an RValue.
        In this discussion, I will denote RValues of type matrix98_t by R1, R2, R3, etc.
*/

/*
    To construct matrix D in C++98 style (matrix98_t D = A + B + C), C++98 style program
    requires 3 times dynamic memory allocations, and 2 times destructor calls,
    one for R1, another for R2, and element by element copy operation.

    STEP 1. operator+(A, B) is called, the result is R1 = A+B. T is dynamically created (1).
    STEP 2. To evaluate R1(==A+B) + C, the result is R2 = R1+C, in which T is dynamically created (2).
            The memory for R1 is destroyed by calling its destructor.
    STEP 3. To construct D, a copy-constructor is called, in which dynamic memory allocation occurs (3).
    STEP 4. Each elements of R2, the return value of R1+C is copied to its corresponding element of D,
            the final destination.
    STEP 5. The destructor of R2 is called, then the memory for its elements is released.

        What about the memory for R1?
*/
void test_c98_matrix()
{
    matrix98_t A(5, 5), B(5, 5), C(5, 5);
    matrix98_t D = A + B + C;
}

void test_c98template_matrix() noexcept
{
    matrix98template_t<int> a{ 2, 2 };
    a(0, 0) = 0;
    a(0, 1) = 1;
    a(1, 0) = 2;
    a(1, 1) = 3;

    matrix98template_t<int> b;
    b = a;
    b = a + b;
    b = a + b + a;
    std::cout << b;
}

/*
    To construct matrix D (matrix98_t D = A + B + C) in C++11.

    STEP 1. To evalue A+B, RV operator+(LR, LR) is called as R1 = operator(A,B),
            where R1 is RValue. In this step, a temporary matrix T is created and it allocates memory dynamically internally for its elements.
    STEP 2. R1 + C should now be evaluated. R2 = operator+(R1, C) is operated.
            R1 is T created in STEP 1. So, T is still alive.
    STEP 3. The move constructor of D (matrix98_t(matrix98_t&& rhs) is calle,
            as matrix98_t(R2), in which the resource of R2 is moved to D.
            After moving resource of R2 to D, the constructor matrix(matrix98_t&& rhs)
            invalidates R2.
            R2 is R1, which is T created in STEP 1. So, T is still alive.
            The move constructor of D invalidates R2, that is T is invalidated.
    STEP 4. matrix D, the final destination, is completed, and R2 is destoryed by calling its destructor.
            When R2 (== T) is destructed, that is, When T is destructed,
            its internal memory resource is already transferred or moved to D.

    In this configuration, the memory dynamically allocated in the T in STEP 1 is effectively move to D.
    That is, the resource of T is recycled to D.
    No element-wise copy operation for the construction of D.

    Also note that T was created to evaluate A+B, and survived until D is fully constructed.
    T is destoyed on after matrix98_t D = A + B + C; completed.

    This is the promise or covenant of C++11 standard.
*/

void test_c11_matrix()
{
    matrixc11_t A(5, 5), B(5, 5), C(5, 5);
    matrixc11_t D = A + B + C;
    matrixc11_t E = A + (B + C);
    matrixc11_t F = (A + B) + (A + C);
}

/*
    To construct matrix D (matrix98_t D = A + B + C) in C++11.

    STEP 1. To evalue A+B, RV operator+(LR, LR) is called as R1 = operator(A,B),
            where R1 is RValue. In this step, a temporary matrix T is created and it allocates memory dynamically internally for its elements.
    STEP 2. R1 + C should now be evaluated. R2 = operator+(R1, C) is operated.
            R1 is T created in STEP 1. So, T is still alive.
    STEP 3. The move constructor of D (matrix98_t(matrix98_t&& rhs) is calle,
            as matrix98_t(R2), in which the resource of R2 is moved to D.
            After moving resource of R2 to D, the constructor matrix(matrix98_t&& rhs)
            invalidates R2.
            R2 is R1, which is T created in STEP 1. So, T is still alive.
            The move constructor of D invalidates R2, that is T is invalidated.
    STEP 4. matrix D, the final destination, is completed, and R2 is destoryed by calling its destructor.
            When R2 (== T) is destructed, that is, When T is destructed,
            its internal memory resource is already transferred or moved to D.

    In this configuration, the memory dynamically allocated in the T in STEP 1 is effectively move to D.
    That is, the resource of T is recycled to D.
    No element-wise copy operation for the construction of D.

    Also note that T was created to evaluate A+B, and survived until D is fully constructed.
    T is destoyed on after matrix98_t D = A + B + C; completed.

    This is the promise or covenant of C++11 standard.
*/

void test_matrix()
{
    matrix_t<double> m(2, 2);
    m(0, 0) = 1; m(0, 1) = 1;
    m(1, 0) = 2; m(1, 1) = 3;

    matrix_t<double> n(2, 2);
    n(0, 0) = 1; n(0, 1) = 1;
    n(1, 0) = 2; n(1, 1) = 3;
    
    std::cout << "m = \n" << m << std::endl;
    std::cout << "n = \n" << n << std::endl;
    std::cout << "m + n = \n" << m + n << std::endl;
}


int main()
{
    test_rvalue_lvalue();
    test_c98_matrix();
    test_c98template_matrix();
    test_c11_matrix();
    test_matrix();
}
