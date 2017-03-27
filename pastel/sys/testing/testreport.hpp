#ifndef PASTELSYS_TESTREPORT_HPP
#define PASTELSYS_TESTREPORT_HPP

#include "pastel/sys/testing/testreport.h"
#include "pastel/sys/logging/log.h"

#include <algorithm>

namespace Pastel
{

	class SortErrorByFileName
	{
	public:
		bool operator()(
			const TestReport::ErrorInfo* left,
			const TestReport::ErrorInfo* right) const
		{
			return left->fileName() < right->fileName();
		}
	};

	class SortErrorByLineNumber
	{
	public:
		bool operator()(
			const TestReport::ErrorInfo* left,
			const TestReport::ErrorInfo* right) const
		{
			return left->lineNumber() < right->lineNumber();
		}
	};

	inline void generateTestReport(
		const TestReport& testReport,
		Log& output)
	{
		std::string title = 
			testReport.name() + " test report";

		output << title << std::endl;

		std::string underline;
		for (integer i = 0;i < title.size();++i)
		{
			underline += '=';
		}

		output << underline << std::endl << std::endl;

		output << "There were ";
		if (testReport.totalErrors() > 0)
		{
			output << testReport.totalErrors() << " errors.";
		}
		else
		{
			output << "no errors.";
		}

		output << std::endl;

		integer errors = testReport.errors();

		std::vector<const TestReport::ErrorInfo*> sortedList;
		sortedList.reserve(errors);
		for (integer i = 0;i < errors;++i)
		{
			sortedList.push_back(&testReport.error(i));
		}

		std::stable_sort(sortedList.begin(), sortedList.end(), 
			SortErrorByLineNumber());

		std::stable_sort(sortedList.begin(), sortedList.end(), 
			SortErrorByFileName());

		std::string prevFileName;
		for (integer i = 0;i < errors;++i)
		{
			const TestReport::ErrorInfo& errorInfo =
				*sortedList[i];

			if (!errorInfo.fileName().empty())
			{
				if (errorInfo.fileName() != prevFileName)
				{
					output << errorInfo.fileName() << std::endl;
					prevFileName = errorInfo.fileName();
				}

				output << std::endl;
			}

			output << "--";
			if (errorInfo.lineNumber() >= 0)
			{
				output << " " << errorInfo.lineNumber() << " --";
			}
			output << std::endl;

			if (errorInfo.hitCount() > 1)
			{
				output << errorInfo.hitCount() << " hits, showing a few." << std::endl;
			}

			if (!errorInfo.testText().empty())
			{
				output << errorInfo.testText();
				output << std::endl;
			}

			if (errorInfo.parameters() > 0)
			{
				output << "where" << std::endl;
				for (integer j = 0;j < errorInfo.parameters();++j)
				{
					const TestReport::ParameterInfo& parameterInfo =
						errorInfo.parameter(j);

					output << parameterInfo.name() << " == "
						<< parameterInfo.value() << std::endl;
				}
			}
		}	
	}

}

#endif
