// Description: TestReport class
// Detail: Stores error reports from unit tests.
// Documentation: testing.txt

#ifndef PASTEL_TESTREPORT_H
#define PASTEL_TESTREPORT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/log.h"

#include <string>
#include <vector>

namespace Pastel
{

	//! Stores error reports from unit tests.
	class PASTELSYS TestReport
	{
	public:
		class ParameterInfo
		{
		public:
			ParameterInfo()
				: name_()
				, value_(0)
			{
			}

			ParameterInfo(
				const std::string& name,
				real64 value)
				: name_(name)
				, value_(value)
			{
			}

			const std::string& name() const
			{
				return name_;
			}

			real64 value() const
			{
				return value_;
			}

		private:
			std::string name_;
			real64 value_;
		};

		class ErrorInfo
		{
		public:
			ErrorInfo()
				: testText_()
				, fileName_()
				, lineNumber_(-1)
				, parameterList_()
				, hitCount_(1)
			{
			}

			ErrorInfo(
				const std::string& testText,
				const std::string& fileName,
				integer lineNumber)
				: testText_(testText)
				, fileName_(fileName)
				, lineNumber_(lineNumber)
				, parameterList_()
				, hitCount_(1)
			{
			}

			void addParameter(
				const std::string& name,
				real64 value)
			{
				parameterList_.push_back(
					ParameterInfo(name, value));
			}

			const std::string& testText() const
			{
				return testText_;
			}

			const std::string& fileName() const
			{
				return fileName_;
			}

			integer lineNumber() const
			{
				return lineNumber_;
			}

			integer parameters() const
			{
				return parameterList_.size();
			}

			const ParameterInfo& parameter(integer index) const
			{
				ENSURE1(index >= 0 && index < parameterList_.size(), index);

				return parameterList_[index];
			}

			void increaseHitCount()
			{
				++hitCount_;
			}

			integer hitCount() const
			{
				return hitCount_;
			}

		private:
			std::string testText_;
			std::string fileName_;
			integer lineNumber_;
			std::vector<ParameterInfo> parameterList_; 
			integer hitCount_;
		};

		TestReport()
			: name_()
			, errorSet_()
			, totalErrors_(0)
		{
		}

		explicit TestReport(
			const std::string& name)
			: name_(name)
			, errorSet_()
			, totalErrors_(0)
		{
		}

		integer totalErrors() const
		{
			return totalErrors_;
		}

		integer errors() const
		{
			return errorSet_.size();
		}

		const ErrorInfo& error(integer index) const
		{
			ENSURE_OP(index, >=, 0);
			ENSURE_OP(index, <, errorSet_.size());

			return errorSet_[index];
		}

		void reportError(
			const std::string& testText = "",
			const std::string& fileName = "", int lineNumber = -1,
			const std::string& info1Name = "", real64 info1 = 0,
			const std::string& info2Name = "", real64 info2 = 0,
			const std::string& info3Name = "", real64 info3 = 0,
			const std::string& info4Name = "", real64 info4 = 0)
		{
			++totalErrors_;

			if (lineNumber >= 0)
			{
				for (integer i = 0;i < errors();++i)
				{
					if (errorSet_[i].lineNumber() == lineNumber &&
						errorSet_[i].fileName() == fileName)
					{
						// No multiple occurences allowed.
						errorSet_[i].increaseHitCount();
						return;
					}
				}
			}

			ErrorInfo errorInfo(
				testText, fileName, lineNumber);

			if (!info1Name.empty())
			{
				errorInfo.addParameter(info1Name, info1);
			}

			if (!info2Name.empty())
			{
				errorInfo.addParameter(info2Name, info2);
			}
			
			if (!info3Name.empty())
			{
				errorInfo.addParameter(info3Name, info3);
			}

			if (!info4Name.empty())
			{
				errorInfo.addParameter(info4Name, info4);
			}
			
			errorSet_.push_back(errorInfo);
		}

		const std::string& name() const
		{
			return name_;
		}
	
	private:
		std::string name_;
		std::vector<ErrorInfo> errorSet_;
		integer totalErrors_;
	};

	PASTELSYS void generateTestReport(
		const TestReport& testReport,
		Log& output);

}

#endif
