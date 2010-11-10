// Description: Logger class
// Detail: An abstract base class for reporting logging information

#ifndef PASTEL_LOGGER_H
#define PASTEL_LOGGER_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/countedptr.h"

#include <string>

namespace Pastel
{

	class PASTELSYS Logger
		: public ReferenceCounted
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		virtual ~Logger();

		//! Write a string.
		virtual Logger& operator<<(
			const std::string& value) = 0;

		//! Frees any resources.
		/*!
		This is called, for example, before aborting
		program execution via an assertion. With this
		function the logger has the chance to close 
		a file or flush the contents of a stream.
		*/
		virtual void finalize() = 0;
	};

	typedef CountedPtr<Logger> LoggerPtr;
	typedef CountedPtr<const Logger> ConstLoggerPtr;

}

#endif
