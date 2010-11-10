#include "pastel/sys/stream_logger.h"

namespace Pastel
{

	PASTELSYS Stream_LoggerPtr streamLogger(std::ostream* stream)
	{
		return Stream_LoggerPtr(new Stream_Logger(stream));
	}

	Stream_Logger::Stream_Logger(std::ostream* stream)
		: stream_(stream)
	{
	}

	Stream_Logger::~Stream_Logger()
	{
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
