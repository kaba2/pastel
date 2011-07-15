#include "pastel/sys/stream_logger.h"

namespace Pastel
{

	Stream_Logger::Stream_Logger(std::ostream* stream)
		: stream_(stream)
	{
	}

	Stream_Logger::~Stream_Logger()
	{
		finalize();
	}

	void Stream_Logger::finalize()
	{
		if (stream_)
		{
			stream_->flush();
		}
	}

	Stream_Logger& Stream_Logger::operator<<(const std::string& value)
	{
		if (stream_)
		{
			(*stream_) << value;
		}

		return *this;
	}

}
