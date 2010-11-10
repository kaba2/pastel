// Description: Stream_Logger class
// Detail: Outputs logging information into a standard library stream

#ifndef PASTEL_STREAM_LOGGER_H
#define PASTEL_STREAM_LOGGER_H

#include "pastel/sys/logger.h"

#include <iostream>

namespace Pastel
{

	class PASTELSYS Stream_Logger
		: public Logger
	{
	public:
		explicit Stream_Logger(std::ostream* stream);

		virtual ~Stream_Logger();

		virtual Stream_Logger& operator<<(const std::string& value);

		virtual void finalize();

	private:
		std::ostream* stream_;
	};

	typedef CountedPtr<Stream_Logger> Stream_LoggerPtr;

	PASTELSYS Stream_LoggerPtr streamLogger(std::ostream* stream);

}

#endif
