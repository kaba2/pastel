// Description: Array range
// Documentation: range.txt

#ifndef PASTEL_ARRAY_RANGE_H
#define PASTEL_ARRAY_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/range_concept.h"

namespace Pastel
{

	template <typename Type>
	class Array_Range
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		typedef Type value_type;
		typedef Type& reference;
		static const bool RandomAccess = true;
		static const bool Bidirectional = true;

		Array_Range()
			: begin_(0)
			, end_(0)
		{
		}

		Array_Range(Type* begin, Type* end)
			: begin_(begin)
			, end_(end)
		{
		}

		void swap(Array_Range& that)
		{
			using std::swap;
			swap(begin_, that.begin_);
			swap(end_, that.end_);
		}

		bool empty() const
		{
			return begin_ >= end_;
		}

		void pop_front()
		{
			PENSURE(!empty());
			++begin_;
		}

		reference front() const
		{
			PENSURE(!empty());
			return *begin_;
		}

		integer size() const
		{
			return end_ - begin_;
		}

		void pop_back()
		{
			PENSURE(!empty());
			--end_;
		}

		reference back() const
		{
			PENSURE(!empty());
			return *(end_ - 1);
		}

		reference operator[](integer index) const
		{
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, size());

			return *(begin_ + index);
		}

	private:
		Type* begin_;
		Type* end_;
	};

	template <typename Type, int N>
	Array_Range<Type> arrayRange(
		Type (&data)[N])
	{
		return Array_Range<Type>(data, data + N);
	}

	template <typename Type, int N>
	Array_Range<Type> arrayRange(
		Type* begin, Type* end)
	{
		return Array_Range<Type>(begin, end);
	}

	template <typename Type, int N>
	Array_Range<Type> arrayRange(
		Type* begin, integer size)
	{
		return Array_Range<Type>(begin, begin + size);
	}

}

#endif
