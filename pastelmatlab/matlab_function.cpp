#include "pastelmatlab/matlab_function.h"
#include "pastelmatlab/matlab_argument.h"

#include "pastel/sys/callfunction.h"
#include "pastel/sys/ensure.h"

#include <map>

#include <tbb/task_scheduler_init.h>

namespace Pastel
{

	namespace
	{

		using FunctionMap = std::map<std::string, MatlabFunction*>;
		using FunctionIterator = FunctionMap::const_iterator;	

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
		functionMap().insert(
			std::make_pair(name, function));
	}

	namespace
	{

		void matlabEntry(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				FunctionName,
				Inputs
			};

			ENSURE_OP(inputs, >=, Inputs);

			// The first parameter is the name of the
			// function that should be called.
			std::string name = matlabAsString(inputSet[FunctionName]);

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
			else
			{
				// There is no such function.
				std::cout << "PastelMatlab: There is no function named " << name << std::endl;
			}

		}

		void matlabListFunctions(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			// This function lists all the functions
			// that are callable.
			// It is itself such a function.

			enum
			{
				Inputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			FunctionIterator iter = functionMap().begin();
			FunctionIterator iterEnd = functionMap().end();
			
			while(iter != iterEnd)
			{
				std::cout << iter->first << std::endl;
				++iter;
			}
		}

		void matlabInitialize()
		{
			// Initialize Threading Building Blocks.
			static tbb::task_scheduler_init theTbbInit(
				tbb::task_scheduler_init::automatic);

			matlabAddFunction(
				"list_functions",
				matlabListFunctions);
		}

		CallFunction run(matlabInitialize);

	}

}

// This is the actual mex entry point.
extern "C" void mexFunction(
	int outputs, mxArray *outputSet[],
	int inputs, const mxArray *inputSet[])
{
	// We'd like to see the logging done into
	// the Matlab window. This is convenient when
	// an invariant failure occurs, or a file can't 
	// be found, etc.
	std::streambuf* cout_sbuf = std::cout.rdbuf();
    std::stringbuf sbuf;
    std::cout.rdbuf(&sbuf);

	try
	{
		Pastel::matlabEntry(
			outputs, outputSet,
			inputs, inputSet);
	}
	catch(const Pastel::InvariantFailure&)
	{
		// There was an invariant failure.
		// The error has already been logged.
		// We simply absorb the exception here.
	};

	// Print any output to Matlab.
    mexPrintf(sbuf.str().c_str());

    // Restore std::cout.
    std::cout.rdbuf(cout_sbuf);
}

