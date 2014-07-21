#include "pastel/sys/testreport.h"
#include "pastel/sys/log.h"

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

	PASTELSYS void generateTestReport(
		const TestReport& testReport,
		Log& output)
	{
		const std::string title = 
			testReport.name() + " test report";

		output << title << logNewLine;

		std::string underline;
		for (integer i = 0;i < title.size();++i)
		{
			underline += '=';
		}

		output << underline << logNewLine << logNewLine;

		output << "There were ";
		if (testReport.totalErrors() > 0)
		{
			output << testReport.totalErrors() << " errors.";
		}
		else
		{
			output << "no errors.";
		}

		output << logNewLine;

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
					output << errorInfo.fileName() << logNewLine;
					prevFileName = errorInfo.fileName();
				}

				output << logNewLine;
			}

			output << "--";
			if (errorInfo.lineNumber() >= 0)
			{
				output << " " << errorInfo.lineNumber() << " --";
			}
			output << logNewLine;

			if (errorInfo.hitCount() > 1)
			{
				output << errorInfo.hitCount() << " hits, showing a few." << logNewLine;
			}

			if (!errorInfo.testText().empty())
			{
				output << errorInfo.testText();
				output << logNewLine;
			}

			if (errorInfo.parameters() > 0)
			{
				output << "where" << logNewLine;
				for (integer i = 0;i < errorInfo.parameters();++i)
				{
					const TestReport::ParameterInfo& parameterInfo =
						errorInfo.parameter(i);

					output << parameterInfo.name() << " == "
						<< parameterInfo.value() << logNewLine;
				}
			}
		}	
	}

}
