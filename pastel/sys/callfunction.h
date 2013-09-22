// Description: CallFunction class
// Detail: Turns initialization into a function call
// Documentation: miscellaneous.txt

#ifndef PASTELSYS_CALLFUNCTION_H
#define PASTELSYS_CALLFUNCTION_H

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
		CallFunction() = delete;
		CallFunction(const CallFunction&) = delete;
		CallFunction operator=(const CallFunction&) = delete;
	};

}

#include "pastel/sys/callfunction.hpp"

#endif
