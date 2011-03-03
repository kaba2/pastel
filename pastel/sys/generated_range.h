// Description: Generated range

#ifndef PASTEL_GENERATED_RANGE_H
#define PASTEL_GENERATED_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/range_concepts.h"

namespace Pastel
{

	template <typename Type, typename Generator>
	class Generated_Range
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		typedef Type value_type;
		typedef const Type& reference;

		Generated_Range()
			: f_()
			, current_()
			, size_(0)
		{
		}

		explicit Generated_Range(
			Generator f,
			integer size = -1)
			: f_(f)
			, current_(f_())
			, size_(size)
		{
		}

		void swap(Generated_Range& that)
		{
			using std::swap;
			swap(f_, that.f_);
			swap(current_, that.current_);
			swap(size_, that.size_);
		}

		bool empty() const
		{
			return size_ == 0;
		}

		void pop_front()
		{
			PENSURE(!empty());
			current_ = f_();
			--size_;
		}

		reference front() const
		{
			PENSURE(!empty());
			return current_;
		}

		integer size() const
		{
			return size_;
		}

	private:
		Generator f_;
		Type current_;
		integer size_;
	};

	template <typename Type, typename Generator>
	Generated_Range<Type, Generator> generatedRange(
		Generator f, integer size = -1)
	{
		return Generated_Range<Type, Generator>(f, size);
	}

}

#endif
