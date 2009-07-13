/*!
\file
\brief Tools for inconsistency checking
*/

#ifndef PASTEL_ENSURE_H
#define PASTEL_ENSURE_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/mytypes.h"

// Reports

#define REPORT(expr)\
	((expr) && (Pastel::Detail::report(#expr, __FILE__, __LINE__), true))

#define REPORT1(expr, a)\
	((expr) && (Pastel::Detail::report(#expr, __FILE__, __LINE__, #a, (real64)(a)), true))

#define REPORT2(expr, a, b)\
	((expr) && (Pastel::Detail::report(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b)), true))

#define REPORT3(expr, a, b, c)\
	((expr) && (Pastel::Detail::report(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c)), true))

#define REPORT4(expr, a, b, c, d)\
	((expr) && (Pastel::Detail::report(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c), #d, (real64)(d)), true))

// Errors

#define ENSURE(expr)\
{if (!(expr)) {Pastel::Detail::error(#expr, __FILE__, __LINE__);}}

#define ENSURE1(expr, a)\
{if (!(expr)) {Pastel::Detail::error(#expr, __FILE__, __LINE__, #a, (real64)(a));}}

#define ENSURE2(expr, a, b)\
{if (!(expr)) {Pastel::Detail::error(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b));}}

#define ENSURE3(expr, a, b, c)\
{if (!(expr)) {Pastel::Detail::error(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c));}}

#define ENSURE4(expr, a, b, c, d)\
{if (!(expr)) {Pastel::Detail::error(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c), #d, (real64)(d));}}

#define ENSURE_OP(x, op, y) ENSURE2(x op y, x, y)

// PENSURES

#if (PASTEL_ENABLE_PENSURES != 0)

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

#if (PASTEL_DEBUG_MODE == 0)

#define ASSERT(expr)
#define ASSERT1(expr, a)
#define ASSERT2(expr, a, b)
#define ASSERT3(expr, a, b, c)
#define ASSERT4(expr, a, b, c, d)

#else

#define ASSERT(expr)\
{if (!(expr)) {Pastel::Detail::assertionError(#expr, __FILE__, __LINE__);}}

#define ASSERT1(expr, a)\
{if (!(expr)) {Pastel::Detail::assertionError(#expr, __FILE__, __LINE__, #a, (real64)(a));}}

#define ASSERT2(expr, a, b)\
{if (!(expr)) {Pastel::Detail::assertionError(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b));}}

#define ASSERT3(expr, a, b, c)\
{if (!(expr)) {Pastel::Detail::assertionError(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c));}}

#define ASSERT4(expr, a, b, c, d)\
{if (!(expr)) {Pastel::Detail::assertionError(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c), #d, (real64)(d));}}

#endif

namespace Pastel
{

	namespace Detail
	{

		// Prints a report message. Used by REPORT macros.

		PASTELSYS void report(
			const char* testText = 0,
			const char* fileName = 0, int lineNumber = -1,
			const char* info1Name = 0, real64 info1 = 0,
			const char* info2Name = 0, real64 info2 = 0,
			const char* info3Name = 0, real64 info3 = 0,
			const char* info4Name = 0, real64 info4 = 0);

		// Prints an error message and aborts the program.

		PASTELSYS void error(
			const char* testText = 0,
			const char* fileName = 0, int lineNumber = -1,
			const char* info1Name = 0, real64 info1 = 0,
			const char* info2Name = 0, real64 info2 = 0,
			const char* info3Name = 0, real64 info3 = 0,
			const char* info4Name = 0, real64 info4 = 0);

		// Prints an error message and aborts the program.

		PASTELSYS void assertionError(
			const char* testText = 0,
			const char* fileName = 0, int lineNumber = -1,
			const char* info1Name = 0, real64 info1 = 0,
			const char* info2Name = 0, real64 info2 = 0,
			const char* info3Name = 0, real64 info3 = 0,
			const char* info4Name = 0, real64 info4 = 0);

	}

}

#include "pastel/sys/ensure.hpp"

#endif
