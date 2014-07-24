#include "pastel/matlab/matlab_function.h"
#include "pastel/matlab/matlab_argument.h"

#include "pastel/sys/callfunction.h"
#include "pastel/sys/logging.h"
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
				mexPrintf(text.c_str());
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
				FunctionName,
				Inputs
			};

			ENSURE_OP(inputs, >=, Inputs);

			// The first parameter is the name of the
			// function that should be called.
			std::string name = asString(inputSet[FunctionName]);

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
				mexPrintf("There is no function named %s.", name.c_str());
				mexPrintf("\n");
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
				mexPrintf(iter->first.c_str());
				mexPrintf("\n");
				++iter;
			}
		}

		void matlabInitialize()
		{
			// If std::abort() is called in a mex file,
			// it causes a crash in Matlab's side.
			// Therefore, we set the invariant action to
			// throwing an exception, which we catch
			// in 'mexFunction' just before getting back 
			// to Matlab's side.
			setInvariantFailureAction(
				InvariantFailureAction::Throw);

			// We'd like to see the logging done into
			// the Matlab window. This is convenient when
			// an invariant failure occurs, or a file can't 
			// be found, etc.
			static Pastel::Matlab_Logger matlabLogger;
			log().addLogger(&matlabLogger);

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
}

