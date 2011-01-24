// Description: Constant range
// Documentation: range.txt

#ifndef PASTEL_CONSTANT_RANGE_H
#define PASTEL_CONSTANT_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/range_concept.h"

namespace Pastel
{

	template <typename Type>
	class Constant_Range
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		typedef Type value_type;
		typedef const Type& reference;
		static const bool RandomAccess = true;
		static const bool Bidirectional = true;

		Constant_Range()
			: data_()
			, size_(0)
		{
		}

		explicit Constant_Range(
			const Type& data,
			integer size = -1)
			: data_(data)
			, size_(size)
		{
		}

		void swap(Constant_Range& that)
		{
			using std::swap;
			swap(data_, that.data_);
			swap(size_, that.size_);
		}

		bool empty() const
		{
			return size_ == 0;
		}

		void pop_front()
		{
			PENSURE(!empty());
			--size_;
		}

		reference front() const
		{
			PENSURE(!empty());
			return data_;
		}

		integer size() const
		{
			return size_;
		}

		void pop_back()
		{
			PENSURE(!empty());
			--size_;
		}

		reference back() const
		{
			PENSURE(!empty());
			return data_;
		}

		reference operator[](integer index) const
		{
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, size_);

			return data_;
		}

	private:
		Type data_;
		integer size_;
	};

	template <typename Type>
	Constant_Range<Type> constantRange2(
		const Type& data, integer size = -1)
	{
		return Constant_Range<Type>(data, size);
	}

}

#endif
