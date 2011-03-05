// Description: Stream range

#ifndef PASTEL_STREAM_RANGE_H
#define PASTEL_STREAM_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/range_concepts.h"

#include <istream>
#include <ostream>

namespace Pastel
{

	template <typename Type>
	class OutputStream_Range
	{
	public:
		typedef Type value_type;
		typedef Type& reference;
		static const bool RandomAccess = false;
		static const bool Bidirectional = false;

		explicit OutputStream_Range(
			std::ostream& stream,
			const std::string& separator)
			: stream_(stream)
			, current_()
			, separator_(separator)
		{
		}

		void swap(OutputStream_Range& that)
		{
			using std::swap;
			stream_.swap(that.stream_);
			swap(current_, that.current_);
			separator_.swap(that.separator_);
		}

		bool empty() const
		{
			return false;
		}

		void pop_front()
		{
			stream_ << current_ << separator_;
		}

		reference front() const
		{
			return current_;
		}

		integer size() const
		{
			return -1;
		}

	private:
		std::ostream& stream_;
		mutable Type current_;
		std::string separator_;
	};

	template <typename Type>
	OutputStream_Range<Type> streamRange(
		std::ostream& stream, 
		const std::string& separator = "\n")
	{
		return OutputStream_Range<Type>(
			stream, separator);
	}

	template <typename Type>
	class InputStream_Range
	{
	public:
		typedef Type value_type;
		typedef Type& reference;
		static const bool RandomAccess = false;
		static const bool Bidirectional = false;

		explicit InputStream_Range(std::istream& stream)
			: stream_(stream)
			, current_()
			, cached_(false)
		{
		}

		void swap(InputStream_Range& that)
		{
			using std::swap;
			stream_.swap(that.stream_);
			swap(current_, that.current_);
		}

		bool empty() const
		{
			return false;
		}

		void pop_front()
		{
			if (!cached_)
			{
				stream >> current_;
			}

			cached_ = false;
		}

		reference front() const
		{
			if (!cached_)
			{
				stream >> current_;
				cached_ = true;
			}

			return current_;
		}

		integer size() const
		{
			return -1;
		}

	private:
		std::istream& stream_;
		Type current_;
		bool cached_;
	};

	template <typename Type>
	InputStream_Range<Type> streamRange(
		std::istream& stream)
	{
		return InputStream_Range<Type>(stream);
	}

}

#endif
