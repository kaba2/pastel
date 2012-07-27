// Description: CallFunction class
// Detail: Turns initialization into a function call
// Documentation: miscellaneous.txt

#ifndef PASTEL_CALLFUNCTION_H
#define PASTEL_CALLFUNCTION_H

#include "pastel/sys/mytypes.h"

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
		CallFunction() PASTEL_DELETE;
		CallFunction(const CallFunction&) PASTEL_DELETE;
		CallFunction operator=(const CallFunction&) PASTEL_DELETE;
	};

}

#include "pastel/sys/callfunction.hpp"

#endif
