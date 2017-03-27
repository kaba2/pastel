#ifndef PASTELSYS_ENSURE_HPP
#define PASTELSYS_ENSURE_HPP

#include "pastel/sys/ensure.h"
#include <iostream>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

namespace Pastel
{

	namespace Ensure_
	{

		inline void invariantFailure()
		{
			std::cout << std::flush;

			#ifdef _MSC_VER
				// If on Visual Studio, create a breakpoint
				// here, and get back to the debugger.
				_CrtDbgBreak();
			#endif

			throw InvariantFailure();
		}

		inline void reportParameters()
		{
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
				std::cout << parameter.name << " == "
					<< parameter.value << std::endl;
			}

			reportParameters(parameterSet...);
		}

		template <typename... TypeSet>
		void report(
			const char* testText,
			const char* functionName,
			const char* fileName, 
			int lineNumber,
			ParameterInfo<TypeSet>... parameterSet)
		{
			std::cout << "File: ";
			if (fileName)
			{
				std::cout << fileName;
			}
			else
			{
				std::cout << "Not available";
			}
			std::cout << std::endl;

			std::cout << "Line: ";
			if (lineNumber >= 0)
			{
				std::cout << lineNumber;
			}
			else
			{
				std::cout << "Not available";
			}
			std::cout << std::endl;

			std::cout << "Function: ";
			if (functionName)
			{
				std::cout << functionName;
			}
			else
			{
				std::cout << "Not available";
			}
			std::cout << std::endl;

			std::cout << "Expression: ";
			if (testText)
			{
				std::cout << testText;
			}
			else
			{
				std::cout << "Not available";
			}
			std::cout << std::endl;

			if (sizeof...(TypeSet) > 0)
			{
				std::cout << "where" << std::endl;
				reportParameters(parameterSet...);
			}

			std::cout << std::endl << std::endl;
		}

		template <typename... TypeSet>
		void error(
			const char* testText,
			const char* functionName,
			const char* fileName, 
			int lineNumber,
			ParameterInfo<TypeSet>... parameterSet)
		{
			std::cout << std::endl;
			std::cout << "Precondition check failed."
				<< std::endl;

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
			std::cout << std::endl;
			std::cout << "Internal check failed." 
				<< std::endl;

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
