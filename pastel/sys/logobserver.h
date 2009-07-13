// Description: LogObserver class
// Detail: An abstract class for handling logging information

#ifndef PASTEL_LOGOBSERVER_H
#define PASTEL_LOGOBSERVER_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/countedptr.h"

#include <string>

namespace Pastel
{

	class PASTELSYS LogObserver
		: public ReferenceCounted
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		virtual ~LogObserver();

		virtual LogObserver& operator<<(const std::string& value) = 0;
	};

	typedef CountedPtr<LogObserver> LogObserverPtr;
	typedef CountedPtr<const LogObserver> ConstLogObserverPtr;

}

#endif
