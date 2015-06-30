// Description: TestSuite class
// Detail: Gathers the results of a unit test into a TestReport.
// Documentation: unit_testing.txt

#ifndef PASTELSYS_TESTSUITE_H
#define PASTELSYS_TESTSUITE_H

#include "pastel/sys/testing/testreport.h"
#include "pastel/sys/ensure.h"

// The 'this->' prefixes here avoid triggering 
// a bug in g++ 5.1.0.

#define TEST_ENSURE(expr)\
{if (!(expr)) {this->reportError(#expr, __FILE__, __LINE__);}}

#define TEST_ENSURE1(expr, a)\
{if (!(expr)) {this->reportError(#expr, __FILE__, __LINE__, #a, (real64)(a));}}

#define TEST_ENSURE2(expr, a, b)\
{if (!(expr)) {this->reportError(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b));}}

#define TEST_ENSURE3(expr, a, b, c)\
{if (!(expr)) {this->reportError(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c));}}

#define TEST_ENSURE4(expr, a, b, c, d)\
{if (!(expr)) {this->reportError(#expr, __FILE__, __LINE__, #a, (real64)(a), #b, (real64)(b), #c, (real64)(c), #d, (real64)(d));}}

#define TEST_ENSURE_OP(x, op, y) TEST_ENSURE2(x op y, x, y)

namespace Pastel
{

	//! Gathers the results of a unit test into a TestReport.
	class TestSuite
	{
	public:
		explicit TestSuite(
			TestReport* testReport)
			: testReport_(testReport)
		{
			ENSURE(testReport);
		}

		virtual ~TestSuite()
		{
		}

		virtual void run() = 0;

		void reportError(
			const std::string& testText = "",
			const std::string& fileName = "", integer lineNumber = -1,
			const std::string& info1Name = "", real64 info1 = 0,
			const std::string& info2Name = "", real64 info2 = 0,
			const std::string& info3Name = "", real64 info3 = 0,
			const std::string& info4Name = "", real64 info4 = 0)
		{
			testReport_->reportError(
				testText, 
				fileName, lineNumber,
				info1Name, info1,
				info2Name, info2,
				info3Name, info3,
				info4Name, info4);
		}
	
	private:
		TestReport* testReport_;
	};

}

#endif
