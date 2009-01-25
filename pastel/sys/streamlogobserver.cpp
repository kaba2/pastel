#include "pastel/sys/streamlogobserver.h"

namespace Pastel
{

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
