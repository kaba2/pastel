#ifndef PASTEL_CONSTANT_RANGE_H
#define PASTEL_CONSTANT_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

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
		{
		}

		explicit Constant_Range(
			const Type& data)
			: data_(data)
		{
		}

		void swap(Constant_Range& that)
		{
			using std::swap;
			swap(data_, that.data_);
		}

		bool empty() const
		{
			return false;
		}

		void pop_front()
		{
		}

		reference front() const
		{
			return data_;
		}

		integer size() const
		{
			return -1;
		}

		void pop_back()
		{
		}

		reference back() const
		{
			return data_;
		}

		reference operator[](integer index)
		{
			PENSURE_OP(index, >=, 0);
			return data_;
		}

	private:
		Type data_;
	};

}

#endif
