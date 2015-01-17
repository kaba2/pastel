// Description: Stream_Logger class
// Detail: Outputs logging information into a standard library stream

#ifndef PASTELSYS_STREAM_LOGGER_H
#define PASTELSYS_STREAM_LOGGER_H

#include "pastel/sys/logging/logger.h"

#include <iostream>

namespace Pastel
{

	class Stream_Logger
		: public Logger
	{
	public:
		explicit Stream_Logger(std::ostream* stream)
		: stream_(stream)
		{
		}

		virtual ~Stream_Logger()
		{
			finalize();
		}

		virtual Stream_Logger& operator<<(const std::string& value)
		{
			if (stream_)
			{
				(*stream_) << value;
			}

			return *this;
		}

		virtual void finalize()
		{
			if (stream_)
			{
				stream_->flush();
			}
		}

	private:
		std::ostream* stream_;
	};

}

#endif
