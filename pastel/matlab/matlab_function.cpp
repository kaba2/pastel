#include "pastel/matlab/matlab_function.h"
#include "pastel/matlab/matlab_argument.h"

#include "pastel/sys/callfunction.h"
#include "pastel/sys/logging.h"
#include "pastel/sys/ensure.h"

#include <map>

namespace Pastel
{

	namespace
	{

		typedef std::map<std::string, MatlabFunction*> FunctionMap;
		typedef FunctionMap::const_iterator FunctionIterator;	

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
			// that are callable.
			// It is itself such a function.

			enum
			{
				Inputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			FunctionIterator iter = functionMap().begin();
			const FunctionIterator iterEnd = functionMap().end();
			
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
			log().addLogger(
				LoggerPtr(new Pastel::Matlab_Logger));

			// We will add one callable function automatically.
			// This one lists all the callable functions,
			// including itself.
			matlabAddFunction(
				"list_functions",
				matlabListFunctions);

			// This function call is needed because
			// of a bug in some versions of Matlab (e.g. 2008a).
			// Matlab sets the OMP_NUM_THREADS environment variable
			// to 1 in start-up, which is then propagated into all
			// mex files which use OpenMP. This can be get around
			// in Matlab by setting OMP_NUM_THREADS to the number
			// cores. However, this must be done _before_ loading
			// a mex file in memory. Because it is easy to forget
			// to do this, or to do this in wrong order, a better
			// workaround is to set the number of threads here.
			setNumberOfThreads(numberOfProcessors());
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

