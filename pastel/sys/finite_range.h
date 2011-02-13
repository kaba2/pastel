// Description: Finite range

#ifndef PASTEL_FINITE_RANGE_H
#define PASTEL_FINITE_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/range_concept.h"

namespace Pastel
{

	template <typename Range>
	class Finite_Range
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		// Only random-access iterators can be efficiently
		// made bidirectional sub-ranges. Therefore, 
		// random-access ranges are made random-access ranges,
		// and other ranges are made forward ranges.

		typedef typename Range::value_type value_type;
		typedef typename Range::reference reference;
		static const bool RandomAccess = Range::RandomAccess;
		static const bool Bidirectional = Range::RandomAccess;

		Finite_Range()
			: data_()
			, size_(0)
		{
		}

		explicit Finite_Range(
			const Range& range,
			integer size)
			: range_(range)
			, size_(size)
		{
			PENSURE_OP(size, >=, 0);
		}

		void swap(Finite_Range& that)
		{
			using std::swap;
			range_.swap(that.range_);
			swap(size_, that.size_);
		}

		bool empty() const
		{
			return size_ == 0;
		}

		void pop_front()
		{
			PENSURE(!empty());
			range_.pop_front();
			--size_;
		}

		reference front() const
		{
			PENSURE(!empty());
			return range_.front();
		}

		integer size() const
		{
			return size_;
		}

		void pop_back()
		{
			PENSURE(!empty());
			range_.pop_back();
			--size_;
		}

		reference back() const
		{
			PENSURE(!empty());
			return range_.back();
		}

		PASTEL_ENABLE_IF_C(RandomAccess, reference) 
			operator[](integer index) const
		{
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, size());

			return range_[index];
		}

	private:
		Range range_;
		integer size_;
	};

	template <typename Range>
	Finite_Range<Range> finiteRange(
		const Range& range, integer size)
	{
		return Finite_Range<Range>(range, size);
	}

}

#endif
