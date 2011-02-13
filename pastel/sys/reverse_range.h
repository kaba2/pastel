// Description: Reverse range

#ifndef PASTEL_REVERSE_RANGE_H
#define PASTEL_REVERSE_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/range_concept.h"

namespace Pastel
{

	template <typename Range>
	class Reverse_Range
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		typedef typename Range::value_type value_type;
		typedef typename Range::reference reference;
		static const bool RandomAccess = Range::RandomAccess;
		static const bool Bidirectional = Range::Bidirectional;

		PASTEL_STATIC_ASSERT(Bidirectional);

		Reverse_Range()
			: range_()
		{
			PENSURE_OP(range_.size(), >=, 0);
		}

		explicit Reverse_Range(
			const Range& range)
			: range_(range)
		{
			PENSURE_OP(range_.size(), >=, 0);
		}

		void swap(Reverse_Range& that)
		{
			range_.swap(that.range_);
		}

		bool empty() const
		{
			return range_.empty();
		}

		void pop_front()
		{
			range_.pop_back();
		}

		reference front() const
		{
			return range_.back();
		}

		integer size() const
		{
			return range_.size();
		}

		void pop_back()
		{
			range_.pop_front();
		}

		reference back() const
		{
			return range_.front();
		}

		const Range& reverse() const
		{
			return range_;
		}

		PASTEL_ENABLE_IF_C(RandomAccess, reference)
			operator[](integer index)
		{
			return range_[size() - 1 - index];
		}

	private:
		Range range_;
	};

	template <typename Range>
	Reverse_Range<Range> reverseRange(const Range& range)
	{
		return Reverse_Range<Range>(range);
	}

	template <typename Range>
	Range reverseRange(const Reverse_Range<Range>& range)
	{
		return range.reverse();
	}

}

#endif
