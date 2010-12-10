#ifndef PASTEL_FUNCTOR_RANGE_H
#define PASTEL_FUNCTOR_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Type, typename Functor>
	class Functor_Range
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		typedef Type value_type;
		typedef const Type& reference;

		Functor_Range()
			: f_()
			, current_()
		{
		}

		explicit Functor_Range(
			const Functor& f)
			: f_(f)
			, current_(f_())
		{
		}

		void swap(Functor_Range& that)
		{
			using std::swap;
			swap(f_, that.f_);
			swap(current_, that.current_);
		}

		bool empty() const
		{
			return false;
		}

		void pop_front()
		{
			PENSURE(!empty());
			current_ = f_();
		}

		reference front() const
		{
			PENSURE(!empty());
			return current_;
		}

		integer size() const
		{
			return -1;
		}

	private:
		Functor f_;
		Type current_;
	};

	template <typename Type, typename Functor>
	Functor_Range<Type, Functor> functorRange(
		const Functor& f)
	{
		return Functor_Range<Type, Functor>(f);
	}

}

#endif
