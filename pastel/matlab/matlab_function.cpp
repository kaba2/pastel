#include "pastel/matlab/matlab_function.h"
#include "pastel/matlab/matlab_argument.h"

#include "pastel/sys/callfunction.h"

#include <map>

namespace Pastel
{

	typedef std::map<std::string, MatlabFunction*> FunctionMap;
	typedef FunctionMap::const_iterator FunctionIterator;	

	namespace
	{

		FunctionMap& functionMap()
		{
			// Note: It is important to use this delayed
			// initialization idiom to guarantee correct
			// initialization order of global variables.
			static FunctionMap theFunctionMap;
			return theFunctionMap;
		}

	}

	void matlabAddFunction(
		const std::string& name,
		MatlabFunction* function)
	{
		functionMap().insert(std::make_pair(name, function));
	}

	void matlabEntry(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[])
	{
		enum
		{
			FunctionName
		};

		// The first parameter is the name of the
		// function that should be called.
		const std::string name = asString(inputSet[FunctionName]);

		// See if a function with that name has
		// been registered.
		FunctionIterator iter = functionMap().find(name);
		
		if (iter != functionMap().end())
		{
			// There is a function with that
			// name registered.

			// Find out its function pointer.
			MatlabFunction* function = iter->second;

			// Call that function, but omit the
			// first name parameter.
			(*function)(
				outputs, outputSet,
				inputs - 1, inputSet + 1);
		}

	}

	namespace
	{

		void matlabListFunctions(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			// This function lists all the functions
			// that are callable with tim_matlab().
			// It is itself such a function.

			FunctionIterator iter = functionMap().begin();
			const FunctionIterator iterEnd = functionMap().end();
			
			while(iter != iterEnd)
			{
				printf(iter->first.c_str());
				printf("\n");
				++iter;
			}
		}

		void addFunction()
		{
			matlabAddFunction(
				"list_functions",
				matlabListFunctions);
		}

		CallFunction run(addFunction);

	}

}

PASTEL_MATLAB_ENTRY();
