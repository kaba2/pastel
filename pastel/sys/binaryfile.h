// Description: Binary file

#ifndef PASTELSYS_BINARYFILE_H
#define PASTELSYS_BINARYFILE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

#include <string>
#include <fstream>

namespace Pastel
{

	class FileNotFoundException
	{
	public:
		FileNotFoundException(
			const std::string& that)
			: message_(that)
		{
		}

		const std::string& what() const
		{
			return message_;
		}

	private:
		std::string message_;
	};

	class BinaryFile
	{
	public:
		BinaryFile();
		BinaryFile(const std::string& fileName,
			bool read, bool write);
		~BinaryFile();

		void open(const std::string& fileName,
			bool read, bool write);
		void close();

		void setEndian(bool little);

		bool isOpen();
		bool isInEnd() const;

		BinaryFile& operator>>(int8& val);
		BinaryFile& operator>>(uint8& val);
		BinaryFile& operator>>(int16& val);
		BinaryFile& operator>>(uint16& val);
		BinaryFile& operator>>(int32& val);
		BinaryFile& operator>>(uint32& val);
		BinaryFile& operator>>(real32& val);
		BinaryFile& operator>>(real64& val);

		BinaryFile& operator<<(int8 val);
		BinaryFile& operator<<(uint8 val);
		BinaryFile& operator<<(int16 val);
		BinaryFile& operator<<(uint16 val);
		BinaryFile& operator<<(int32 val);
		BinaryFile& operator<<(uint32 val);
		BinaryFile& operator<<(real32 val);
		BinaryFile& operator<<(real64 val);

		BinaryFile& operator<<(const std::string& text);
		BinaryFile& operator<<(const char* nullString);
		BinaryFile& operator<<(void (*callBack)(BinaryFile&));

		void ignore(integer count);

		void flush();
		void write(const char* str, integer count);
		void read(char* str, integer count);

		void setOffset(integer offset);
		integer getOffset();

	private:
		BinaryFile(const BinaryFile&);
		BinaryFile& operator=(const BinaryFile&);

		std::fstream stream_;
		bool littleEndian_;
	};

	void binaryLittleEndian(BinaryFile& file);
	void binaryBigEndian(BinaryFile& file);

}

#include "pastel/sys/binaryfile.hpp"

#endif
