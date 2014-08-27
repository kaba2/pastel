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
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a)), true))

#define REPORT2(expr, a, b)\
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b)), true))

#define REPORT3(expr, a, b, c)\
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c)), true))

#define REPORT4(expr, a, b, c, d)\
	((expr) && (Pastel::Ensure_::report(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c), #d, (real64)(d)), true))

#define REPORT_OP(x, op, y) REPORT2(x op y, x, y)

// Errors

#define ENSURE(expr)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__);}}

#define ENSURE1(expr, a)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a));}}

#define ENSURE2(expr, a, b)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b));}}

#define ENSURE3(expr, a, b, c)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c));}}

#define ENSURE4(expr, a, b, c, d)\
{if (!(expr)) {Pastel::Ensure_::error(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c), #d, (real64)(d));}}

#define ENSURE_OP(x, op, y) ENSURE2(x op y, x, y)

// PENSURES

#if defined(DEBUG) || defined(_DEBUG)

#define PENSURE(expr) ENSURE(expr)
#define PENSURE1(expr, a) ENSURE1(expr, a)
#define PENSURE2(expr, a, b) ENSURE2(expr, a, b)
#define PENSURE3(expr, a, b, c) ENSURE3(expr, a, b, c)
#define PENSURE4(expr, a, b, c, d) ENSURE4(expr, a, b, c, d)
#define PENSURE_OP(x, op, y) ENSURE_OP(x, op, y)

#else

#define PENSURE(expr)
#define PENSURE1(expr, a)
#define PENSURE2(expr, a, b)
#define PENSURE3(expr, a, b, c)
#define PENSURE4(expr, a, b, c, d)
#define PENSURE_OP(x, op, y)

#endif

// Assertions

#if defined(DEBUG) || defined(_DEBUG)

#define ASSERT(expr)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__);}}

#define ASSERT1(expr, a)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a));}}

#define ASSERT2(expr, a, b)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b));}}

#define ASSERT3(expr, a, b, c)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c));}}

#define ASSERT4(expr, a, b, c, d)\
{if (!(expr)) {Pastel::Ensure_::assertionError(#expr, PASTEL_FUNCTION_NAME, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c), #d, (real64)(d));}}

#define ASSERT_OP(x, op, y) ASSERT2(x op y, x, y)

#else

#define ASSERT(expr)
#define ASSERT1(expr, a)
#define ASSERT2(expr, a, b)
#define ASSERT3(expr, a, b, c)
#define ASSERT4(expr, a, b, c, d)
#define ASSERT_OP(x, op, y)

#endif

namespace Pastel
{

	class InvariantFailure {};

	enum class InvariantFailureAction : integer
	{
		// std::abort();
		Abort,
		// assert(false); std::abort();
		AssertAndAbort,
		// throw InvariantFailure();
		Throw,
		// assert(false); throw InvariantFailure();
		AssertAndThrow
	};

	//! Sets the action on invariant failure.
	void setInvariantFailureAction(
		InvariantFailureAction action);

	//! Returns the current invariant failure action.
	InvariantFailureAction invariantFailureAction();

	namespace Ensure_
	{

		// Prints a report message. Used by REPORT macros.

		void report(
			const char* testText = 0,
			const char* functionName = 0,
			const char* fileName = 0, int lineNumber = -1,
			const char* info1Name = 0, real64 info1 = 0,
			const char* info2Name = 0, real64 info2 = 0,
			const char* info3Name = 0, real64 info3 = 0,
			const char* info4Name = 0, real64 info4 = 0);

		// Prints an error message and aborts the program.

		void error(
			const char* testText = 0,
			const char* functionName = 0,
			const char* fileName = 0, int lineNumber = -1,
			const char* info1Name = 0, real64 info1 = 0,
			const char* info2Name = 0, real64 info2 = 0,
			const char* info3Name = 0, real64 info3 = 0,
			const char* info4Name = 0, real64 info4 = 0);

		// Prints an error message and aborts the program.

		void assertionError(
			const char* testText = 0,
			const char* functionName = 0,
			const char* fileName = 0, int lineNumber = -1,
			const char* info1Name = 0, real64 info1 = 0,
			const char* info2Name = 0, real64 info2 = 0,
			const char* info3Name = 0, real64 info3 = 0,
			const char* info4Name = 0, real64 info4 = 0);

	}

}

#include "pastel/sys/ensure.hpp"

#endif
