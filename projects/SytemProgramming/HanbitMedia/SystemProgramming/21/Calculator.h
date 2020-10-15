/*
    Calculator.h
    DLL용 헤더파일.
*/

#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#ifdef __cplusplus
	extern "C"
	{
#endif

	#ifdef _COMPLING_DLL_CALCULATOR
		#define LIBSPEC __declspec(dllexport)
	#else
		#define LIBSPEC __declspec(dllimport)
	#endif

		LIBSPEC int Add(int a, int b);
		LIBSPEC int Min(int a, int b);
		LIBSPEC double Div(double a, double b);
		LIBSPEC double Mul(double a, double c); 

#ifdef __cplusplus
	}
#endif

#endif

