#ifndef PASTELSYS_BINARYFILE_HPP
#define PASTELSYS_BINARYFILE_HPP

#include "pastel/sys/binaryfile.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/endian.h"

#include <fstream>

namespace Pastel
{

	inline BinaryFile::BinaryFile()
		: stream_()
		, littleEndian_(true)
	{
	}

	inline BinaryFile::BinaryFile(const std::string& fileName, bool read, bool write)
		: stream_()
		, littleEndian_(true)
	{
		open(fileName, read, write);
	}

	inline BinaryFile::~BinaryFile()
	{
		close();
	}

	inline void BinaryFile::open(const std::string& fileName, bool read, bool write)
	{
		if (fileName.empty())
		{
			return;
		}

		std::ios_base::openmode mode = std::ios_base::binary;

		if (write)
		{
			if (read)
			{
				mode |=
					std::ios_base::in |
					std::ios_base::out;
			}
			else
			{
				mode |=
					std::ios_base::out |
					std::ios_base::trunc;
			}
		}
		else
		{
			if (read)
			{
				mode |=
					std::ios_base::in;
			}
			else
			{
			}
		}

		stream_.open(fileName.c_str(), mode);
	}

	inline void BinaryFile::close()
	{
		if (stream_.is_open())
		{
			stream_.close();
		}
	}

	inline void BinaryFile::setEndian(bool littleEndian)
	{
		littleEndian_ = littleEndian;
	}

	inline bool BinaryFile::isOpen()
	{
		return stream_.is_open();
	}

	inline bool BinaryFile::isInEnd() const
	{
		return stream_.eof();
	}

	inline BinaryFile& BinaryFile::operator>>(int8& value)
	{
		read((char*)&value, 1);

		return *this;
	}

	inline BinaryFile& BinaryFile::operator>>(uint8& value)
	{
		read((char*)&value, 1);

		return *this;
	}

	inline BinaryFile& BinaryFile::operator>>(int16& value)
	{
		int8 data[2];
		read(&data[0], 2);

		if (littleEndian_)
		{
			value = littleEndian((int16&)data[0]);
		}
		else
		{
			value = bigEndian((int16&)data[0]);
		}

		return *this;
	}

	inline BinaryFile& BinaryFile::operator>>(uint16& value)
	{
		int8 data[2];
		read(&data[0], 2);

		if (littleEndian_)
		{
			value = littleEndian((uint16&)data[0]);
		}
		else
		{
			value = bigEndian((uint16&)data[0]);
		}

		return *this;
	}

	inline BinaryFile& BinaryFile::operator>>(int32& value)
	{
		int8 data[4];
		read(&data[0], 4);

		if (littleEndian_)
		{
			value = littleEndian((int32&)data[0]);
		}
		else
		{
			value = bigEndian((int32&)data[0]);
		}

		return *this;
	}

	inline BinaryFile& BinaryFile::operator>>(uint32& value)
	{
		int8 data[4];
		read(&data[0], 4);

		if (littleEndian_)
		{
			value = littleEndian((uint32&)data[0]);
		}
		else
		{
			value = bigEndian((uint32&)data[0]);
		}

		return *this;
	}

	inline BinaryFile& BinaryFile::operator>>(real32& value)
	{
		int8 data[4];
		read(&data[0], 4);

		if (littleEndian_)
		{
			value = littleEndian((real32&)data[0]);
		}
		else
		{
			value = bigEndian((real32&)data[0]);
		}

		return *this;
	}

	inline BinaryFile& BinaryFile::operator>>(real64& value)
	{
		int8 data[8];
		read(&data[0], 8);

		if (littleEndian_)
		{
			value = littleEndian((real64&)data[0]);
		}
		else
		{
			value = bigEndian((real64&)data[0]);
		}

		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(int8 value)
	{
		write((char*)&value, 1);
		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(uint8 value)
	{
		write((int8*)&value, 1);
		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(int16 value)
	{
		if (littleEndian_)
		{
			value = littleEndian(value);
		}
		else
		{
			value = bigEndian(value);
		}

		write((int8*)&value, 2);
		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(uint16 value)
	{
		if (littleEndian_)
		{
			value = littleEndian(value);
		}
		else
		{
			value = bigEndian(value);
		}

		write((int8*)&value, 2);
		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(int32 value)
	{
		if (littleEndian_)
		{
			value = littleEndian(value);
		}
		else
		{
			value = bigEndian(value);
		}

		write((int8*)&value, 4);
		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(uint32 value)
	{

		if (littleEndian_)
		{
			value = littleEndian(value);
		}
		else
		{
			value = bigEndian(value);
		}

		write((int8*)&value, 4);
		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(real32 value)
	{
		if (littleEndian_)
		{
			value = littleEndian(value);
		}
		else
		{
			value = bigEndian(value);
		}

		write((int8*)&value, 4);
		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(real64 value)
	{
		if (littleEndian_)
		{
			value = littleEndian(value);
		}
		else
		{
			value = bigEndian(value);
		}

		write((int8*)&value, 8);
		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(const std::string& text)
	{
		write(&text[0], text.size());

		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(const char* nullString)
	{
		char symbol = 0;
		integer index = 0;
		do
		{
			symbol = nullString[index];
			(*this) << symbol;
		}
		while (symbol != 0);

		return *this;
	}

	inline BinaryFile& BinaryFile::operator<<(void (*callBack)(BinaryFile&))
	{
		ENSURE(callBack);

		callBack(*this);

		return *this;
	}

	inline void BinaryFile::ignore(integer count)
	{
		ENSURE_OP(count, >=, 0);

		stream_.ignore(count);
	}

	inline void BinaryFile::flush()
	{
		stream_.flush();
	}

	inline void BinaryFile::write(const char* str, integer count)
	{
		ENSURE_OP(count, >=, 0);

		if (!str)
		{
			return;
		}

		stream_.write(str, count);
	}

	inline void BinaryFile::read(char* str, integer count)
	{
		ENSURE_OP(count, >=, 0);
		if (!str)
		{
			return;
		}

		stream_.read(str, count);
	}

	inline void BinaryFile::setOffset(integer offset)
	{
		if (offset < 0)
		{
			stream_.seekg(offset, std::ios::end);
		}
		else
		{
			stream_.seekg(offset, std::ios::beg);
		}
	}

	inline integer BinaryFile::getOffset()
	{
		return stream_.tellg();
	}

	// Helpers

	inline void binaryLittleEndian(BinaryFile& file)
	{
		file.setEndian(true);
	}

	inline void binaryBigEndian(BinaryFile& file)
	{
		file.setEndian(false);
	}

}

#endif
