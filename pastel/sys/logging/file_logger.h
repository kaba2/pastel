// Description: File_Logger class
// Detail: Outputs logging information into a file

#ifndef PASTELSYS_FILE_LOGGER_H
#define PASTELSYS_FILE_LOGGER_H

#include "pastel/sys/logging/logger.h"

#include <fstream>

namespace Pastel
{

	class File_Logger
		: public Logger
	{
	public:
		explicit File_Logger(const std::string& fileName)
		: file_(fileName.c_str())
		{
		}

		virtual ~File_Logger()
		{
			finalize();
		}

		virtual File_Logger& operator<<(const std::string& value)
		{
			file_ << value;
			return *this;
		}

		virtual void finalize()
		{
			file_.close();
		}

	private:
		std::ofstream file_;
	};

}

#endif
