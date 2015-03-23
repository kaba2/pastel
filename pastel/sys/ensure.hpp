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

		template <
			typename Type,
			typename... TypeSet>
		void reportParameters(
			ParameterInfo<Type> parameter, 
			ParameterInfo<TypeSet>... parameterSet)
		{
			if (parameter.name)
			{
				log() << parameter.name << " == "
					<< parameter.value << logNewLine;
			}

			reportParameters(parameterSet...);
		}

		inline void reportParameters()
		{
		}

		template <typename... TypeSet>
		void report(
			const char* testText,
			const char* functionName,
			const char* fileName, 
			int lineNumber,
			ParameterInfo<TypeSet>... parameterSet)
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

			if (sizeof...(TypeSet) > 0)
			{
				log() << "where" << logNewLine;
				reportParameters(parameterSet...);
			}

			log() << logNewLine << logNewLine;
		}

		template <typename... TypeSet>
		void error(
			const char* testText,
			const char* functionName,
			const char* fileName, 
			int lineNumber,
			ParameterInfo<TypeSet>... parameterSet)
		{
			log() << logNewLine;
			log() << "Precondition check failed."
				<< logNewLine;

			report(testText,
				functionName,
				fileName, 
				lineNumber,
				parameterSet...);

			invariantFailure();
		}

		template <typename... TypeSet>
		void assertionError(
			const char* testText,
			const char* functionName,
			const char* fileName, 
			int lineNumber,
			ParameterInfo<TypeSet>... parameterSet)
		{
			log() << logNewLine;
			log() << "Internal check failed." 
				<< logNewLine;

			report(testText,
				functionName,
				fileName, 
				lineNumber,
				parameterSet...);

			invariantFailure();
		}

	}

}

#endif
