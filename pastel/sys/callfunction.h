// Description: CallFunction class
// Detail: Turns initialization into a function call

/*!
\file
\brief A class for turning an initialization into a function call
*/

#ifndef PASTEL_CALLFUNCTION_H
#define PASTEL_CALLFUNCTION_H

namespace Pastel
{

	//! Turns initialization into a function call.
	/*!
	Sometimes it is required to call functions in
	the time when global variables are initialized.
	This class takes a function pointer in initialization
	and calls it if it is non-zero. The given function
	pointer must be of the type 'void(*)()';
	*/
	class CallFunction
	{
	public:
		typedef void (*Function)();

		//! Calls the given function.
		explicit CallFunction(Function function);

	private:
		// Prohibited
		CallFunction();
		// Prohibited
		CallFunction(const CallFunction&);
		// Prohibited
		CallFunction operator=(const CallFunction&);
	};

}

#include "pastel/sys/callfunction.hpp"

#endif
