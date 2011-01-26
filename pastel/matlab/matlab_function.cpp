#include "pastel/matlab/matlab_function.h"
#include "pastel/matlab/matlab_argument.h"

#include "pastel/sys/callfunction.h"
#include "pastel/sys/log_all.h"
#include "pastel/sys/ensure.h"

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

	namespace
	{

		class Matlab_Logger
			: public Logger
		{
		public:
			virtual ~Matlab_Logger()
			{
			}

			virtual Matlab_Logger& operator<<(
				const std::string& text)
			{
				// Matlab redefines 'printf' by a macro.
				printf(text.c_str());
				return *this;
			}

			virtual void finalize()
			{
			}
		};

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

		void matlabInitialize()
		{
			setInvariantFailureAction(
				InvariantFailureAction::Throw);

			log().addLogger(
				LoggerPtr(new Pastel::Matlab_Logger));

			matlabAddFunction(
				"list_functions",
				matlabListFunctions);

			setNumberOfThreads(0);
		}

		CallFunction run(matlabInitialize);

	}

}

// This is the actual mex entry point.
extern "C" void mexFunction(
	int outputs, mxArray *outputSet[],
	int inputs, const mxArray *inputSet[])
{
	try
	{
		Pastel::matlabEntry(
			outputs, outputSet,
			inputs, inputSet);
	}
	catch(const Pastel::InvariantFailure&)
	{
		// The error has already been logged.
		// We simply absorb the exception here.
	};
}

