// Description: File_Logger class
// Detail: Outputs logging information into a file

#ifndef PASTEL_FILE_LOGGER_H
#define PASTEL_FILE_LOGGER_H

#include "pastel/sys/logger.h"

#include <fstream>

namespace Pastel
{

	class PASTELSYS File_Logger
		: public Logger
	{
	public:
		explicit File_Logger(const std::string& fileName);

		virtual ~File_Logger();

		virtual File_Logger& operator<<(const std::string& value);

		virtual void finalize();

	private:
		std::ofstream file_;
	};

	typedef CountedPtr<File_Logger> File_LoggerPtr;

	PASTELSYS File_LoggerPtr fileLogger(const std::string& fileName);

}

#endif
