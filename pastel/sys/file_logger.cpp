#include "pastel/sys/file_logger.h"

namespace Pastel
{

	PASTELSYS File_LoggerPtr fileLogger(const std::string& fileName)
	{
		return File_LoggerPtr(new File_Logger(fileName));
	}

	File_Logger::File_Logger(const std::string& fileName)
		: file_(fileName.c_str())
	{
	}

	File_Logger::~File_Logger()
	{
	}

	File_Logger& File_Logger::operator<<(const std::string& value)
	{
		file_ << value;

		return *this;
	}

	void File_Logger::finalize()
	{
		file_.close();
	}

}
