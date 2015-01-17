#ifndef PASTELSYS_ENSURE_HPP
#define PASTELSYS_ENSURE_HPP

#include "pastel/sys/ensure.h"
#include "pastel/sys/logging/log.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

namespace Pastel
{

	namespace Ensure_
	{

		inline void invariantFailure()
		{
			log().finalize();

			#ifdef _MSC_VER
				// If on Visual Studio, create a breakpoint
				// here, and get back to the debugger.
				_CrtDbgBreak();
			#endif

			throw InvariantFailure();
		}

		inline void report(
			const char* testText,
			const char* functionName,
			const char* fileName, int lineNumber,
			const char* info1Name, real64 info1,
			const char* info2Name, real64 info2,
			const char* info3Name, real64 info3,
			const char* info4Name, real64 info4)
		{
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

			log() << "Function: ";
			if (functionName)
			{
				log() << functionName;
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
				log() << "where" << logNewLine;

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

			log() << logNewLine << logNewLine;
		}

		inline void error(
			const char* testText,
			const char* functionName,
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
				functionName,
				fileName, lineNumber,
				info1Name, info1,
				info2Name, info2,
				info3Name, info3,
				info4Name, info4);

			invariantFailure();
		}

		inline void assertionError(
			const char* testText,
			const char* functionName,
			const char* fileName, int lineNumber,
			const char* info1Name, real64 info1,
			const char* info2Name, real64 info2,
			const char* info3Name, real64 info3,
			const char* info4Name, real64 info4)
		{
			log() << logNewLine;
			log() << "Internal check failed." 
				<< logNewLine;

			report(testText,
				functionName,
				fileName, lineNumber,
				info1Name, info1,
				info2Name, info2,
				info3Name, info3,
				info4Name, info4);

			invariantFailure();
		}

	}

}

#endif
