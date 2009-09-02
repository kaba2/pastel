// Description: StreamLogObserver class
// Detail: Outputs logging information to a standard library stream
// Documentation: log.txt

#ifndef PASTEL_STREAMLOGOBSERVER_H
#define PASTEL_STREAMLOGOBSERVER_H

#include "pastel/sys/logobserver.h"

#include <iostream>

namespace Pastel
{

	class PASTELSYS StreamLogObserver
		: public LogObserver
	{
	public:
		explicit StreamLogObserver(std::ostream* stream);

		virtual ~StreamLogObserver();

		virtual StreamLogObserver& operator<<(const std::string& value);

	private:
		std::ostream* stream_;
	};

	typedef CountedPtr<StreamLogObserver> StreamLogObserverPtr;

	PASTELSYS StreamLogObserverPtr streamLogObserver(std::ostream* stream);

}

#endif
