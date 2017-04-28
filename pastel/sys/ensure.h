// Description: Invariant checking
// Detail: Contains ENSURE, PENSURE, and ASSERT macros.

#ifndef PASTELSYS_ENSURE_H
#define PASTELSYS_ENSURE_H

#include "pastel/sys/mytypes.h"

#if (defined _WIN32 || defined _WIN64)
#	define PASTEL_FUNCTION_NAME __FUNCTION__
#else
#	define PASTEL_FUNCTION_NAME __func__
#endif

// Reports

#define REPORT(expr)\
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__), true))

#define REPORT1(expr, a)\
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a)), true))

#define REPORT2(expr, a, b)\
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b)), true))

#define REPORT3(expr, a, b, c)\
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b), Pastel::Ensure_::parameterInfo(#c, c)), true))

#define REPORT4(expr, a, b, c, d)\
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b), Pastel::Ensure_::parameterInfo(#c, c), Pastel::Ensure_::parameterInfo(#d, d)), true))

#define REPORT_OP(x, op, y) REPORT2(x op y, x, y)

// Errors

#define ENSURE(expr)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__);}}

#define ENSURE1(expr, a)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a));}}

#define ENSURE2(expr, a, b)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b));}}

#define ENSURE3(expr, a, b, c)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b), Pastel::Ensure_::parameterInfo(#c, c));}}

#define ENSURE4(expr, a, b, c, d)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b), Pastel::Ensure_::parameterInfo(#c, c), Pastel::Ensure_::parameterInfo(#d, d));}}

#define ENSURE_OP(x, op, y) ENSURE2(x op y, x, y)
#define ENSURE_RANGE(x, min, max) ENSURE3(min <= x && x < max, x, min, max)

// PENSURES

#if (defined(DEBUG) || defined(PASTEL_ENABLE_ASSERTS))

#define PENSURE(expr) ENSURE(expr)
#define PENSURE1(expr, a) ENSURE1(expr, a)
#define PENSURE2(expr, a, b) ENSURE2(expr, a, b)
#define PENSURE3(expr, a, b, c) ENSURE3(expr, a, b, c)
#define PENSURE4(expr, a, b, c, d) ENSURE4(expr, a, b, c, d)
#define PENSURE_OP(x, op, y) ENSURE_OP(x, op, y)
#define PENSURE_RANGE(x, min, max) ENSURE_RANGE(x, min, max)

#else

#define PENSURE(expr) 
#define PENSURE1(expr, a) 
#define PENSURE2(expr, a, b) 
#define PENSURE3(expr, a, b, c) 
#define PENSURE4(expr, a, b, c, d) 
#define PENSURE_OP(x, op, y) 
#define PENSURE_RANGE(x, min, max)

#endif

// Assertions

#if (defined(DEBUG) || defined(PASTEL_ENABLE_ASSERTS))

#define ASSERT(expr)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__);}}

#define ASSERT1(expr, a)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a));}}

#define ASSERT2(expr, a, b)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b));}}

#define ASSERT3(expr, a, b, c)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b), Pastel::Ensure_::parameterInfo(#c, c));}}

#define ASSERT4(expr, a, b, c, d)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, Pastel::Ensure_::parameterInfo(#a, a), Pastel::Ensure_::parameterInfo(#b, b), Pastel::Ensure_::parameterInfo(#c, c), Pastel::Ensure_::parameterInfo(#d, d));}}

#define ASSERT_OP(x, op, y) ASSERT2(x op y, x, y)
#define ASSERT_RANGE(x, min, max) ASSERT3(min <= x && x < max, x, min, max)

#else

#define ASSERT(expr)
#define ASSERT1(expr, a)
#define ASSERT2(expr, a, b)
#define ASSERT3(expr, a, b, c)
#define ASSERT4(expr, a, b, c, d)
#define ASSERT_OP(x, op, y)
#define ASSERT_RANGE(x, min, max)

#endif

namespace Pastel
{

	class InvariantFailure {};

	namespace Ensure_
	{

		template <typename Type>
		struct ParameterInfo
		{
			const char* name;
			Type value;
		};

		template <typename Type>
		ParameterInfo<Type> parameterInfo(
			const char* name, Type&& value)
		{
			return {name, std::forward<Type>(value)};
		}

		//! Prints a report message. Used by REPORT macros.
		template <typename... TypeSet>
		void report(
			const char* testText,
			const char* functionName,
			const char* fileName, 
			int lineNumber,
			ParameterInfo<TypeSet>... parameterSet);

		//! Prints an error message and aborts the program.
		template <typename... TypeSet>
		void error(
			const char* testText,
			const char* functionName,
			const char* fileName, 
			int lineNumber,
			ParameterInfo<TypeSet>... parameterSet);

		//! Prints an error message and aborts the program.
		template <typename... TypeSet>
		void assertionError(
			const char* testText,
			const char* functionName,
			const char* fileName, 
			int lineNumber,
			ParameterInfo<TypeSet>... parameterSet);

	}

}

#include "pastel/sys/ensure.hpp"

#endif
