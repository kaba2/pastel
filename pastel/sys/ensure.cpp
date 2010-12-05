#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"

#include <cassert>

namespace Pastel
{

	namespace Detail
	{

		PASTELSYS void report(
			const char* testText,
			const char* fileName, int lineNumber,
			const char* info1Name, real64 info1,
			const char* info2Name, real64 info2,
			const char* info3Name, real64 info3,
			const char* info4Name, real64 info4)
		{
			log() << "Report:" << logNewLine;

			log() << "File: ";

			if (fileName)
			{
				log() << fileName;
			}
			else
			{
				log() << "Not available";
			}

			log() << logNewLine;

			log() << "Line: ";

			if (lineNumber >= 0)
			{
				log() << lineNumber;
			}
			else
			{
				log() << "Not available";
			}

			log() << logNewLine;

			log() << "Expression: ";

			if (testText)
			{
				log() << testText;
			}
			else
			{
				log() << "Not available";
			}

			log() << logNewLine;

			if (info1Name ||
				info2Name ||
				info3Name ||
				info4Name)
			{
				log() << "More information:" << logNewLine;

				if (info1Name)
				{
					log() << info1Name << " == "
						<< info1 << logNewLine;
				}
				if (info2Name)
				{
					log() << info2Name << " == "
						<< info2 << logNewLine;
				}
				if (info3Name)
				{
					log() << info3Name << " == "
						<< info3 << logNewLine;
				}
				if (info4Name)
				{
					log() << info4Name << " == "
						<< info4 << logNewLine;
				}
			}

			log() << "End of report." << logNewLine << logNewLine;
		}

		PASTELSYS void error(
			const char* testText,
			const char* fileName, int lineNumber,
			const char* info1Name, real64 info1,
			const char* info2Name, real64 info2,
			const char* info3Name, real64 info3,
			const char* info4Name, real64 info4)
		{
			log() << logNewLine;
			log() << "Precondition check failed."
				<< logNewLine;

			report(testText,
				fileName, lineNumber,
				info1Name, info1,
				info2Name, info2,
				info3Name, info3,
				info4Name, info4);

			log().finalize();

			// Move control to debugger.
			assert(false);

			std::abort();
		}

		PASTELSYS void assertionError(
			const char* testText,
			const char* fileName, int lineNumber,
			const char* info1Name, real64 info1,
			const char* info2Name, real64 info2,
			const char* info3Name, real64 info3,
			const char* info4Name, real64 info4)
		{
			log() << logNewLine;
			log() << "Internal check failed." << logNewLine;

			report(testText,
				fileName, lineNumber,
				info1Name, info1,
				info2Name, info2,
				info3Name, info3,
				info4Name, info4);

			log().finalize();

			// Move control to debugger.
			assert(false);

			std::abort();
		}

	}

}
