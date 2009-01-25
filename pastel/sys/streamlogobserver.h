#ifndef PASTELSYS_STREAMLOGOBSERVER_H
#define PASTELSYS_STREAMLOGOBSERVER_H

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

}

#endif
