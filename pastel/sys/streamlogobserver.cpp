#include "pastel/sys/streamlogobserver.h"

namespace Pastel
{

	PASTELSYS StreamLogObserverPtr streamLogObserver(std::ostream* stream)
	{
		return StreamLogObserverPtr(new StreamLogObserver(stream));
	}

	StreamLogObserver::StreamLogObserver(std::ostream* stream)
		: stream_(stream)
	{
	}

	StreamLogObserver::~StreamLogObserver()
	{
	}

	StreamLogObserver& StreamLogObserver::operator<<(const std::string& value)
	{
		if (stream_)
		{
			(*stream_) << value;
		}

		return *this;
	}

}
