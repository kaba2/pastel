// Description: Rectangle iterator
// Documentation: iterator.txt

#ifndef PASTELSYS_RECTANGLE_ITERATOR_H
#define PASTELSYS_RECTANGLE_ITERATOR_H

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <int N_, bool RowMajor = true>
	class RectangleIterator
	{
	public:
		static PASTEL_CONSTEXPR int N = N_;

		explicit RectangleIterator(
			const Vector<integer, N>& max)
			: position_(ofDimension(max.n()), 0)
			, min_(position_)
			, max_(max)
			, done_(anyLessEqual(max, 0))
		{
		}

		RectangleIterator(
			const Vector<integer, N>& min,
			const Vector<integer, N>& max)
			: position_(min)
			, min_(min)
			, max_(max)
			, done_(anyLessEqual(max, min))
		{
		}

		bool done() const
		{
			return done_;
		}

		RectangleIterator& operator++()
		{
			integer d = n();
			static PASTEL_CONSTEXPR integer Step = RowMajor ? 1 : -1;

			integer i = RowMajor ? 0 : d - 1;

			while((!RowMajor && i >= 0) || 
				(RowMajor && i < d))
			{
				++position_[i];
				if (position_[i] >= max_[i])
				{
					ASSERT_OP(position_[i], ==, max_[i]);
					position_[i] = min_[i];
				}
				else
				{
					break;
				}
				
				i += Step;
			}
			
			done_ = 
				(!RowMajor && i == -1) ||
				(RowMajor && i == d);

			return *this;
		}

		RectangleIterator& operator--()
		{
			integer d = n();
			static PASTEL_CONSTEXPR integer Step = RowMajor ? 1 : -1;

			integer i = RowMajor ? 0 : d - 1;

			while((!RowMajor && i >= 0) || 
				(RowMajor && i < d))
			{
				--position_[i];
				if (position_[i] < min_[i])
				{
					ASSERT_OP(position_[i], ==, min_[i] - 1);
					position_[i] = max_[i] - 1;
				}
				else
				{
					break;
				}

				i += Step;
			}

			done_ = 
				(!RowMajor && i == -1) ||
				(RowMajor && i == d);

			return *this;
		}

		const Vector<integer, N>& position() const
		{
			return position_;
		}

		const Vector<integer, N>& min() const
		{
			return min_;
		}

		const Vector<integer, N>& max() const
		{
			return max_;
		}

		integer n() const
		{
			return position_.n();
		}

	private:
		Vector<integer, N> position_;
		Vector<integer, N> min_;
		Vector<integer, N> max_;
		bool done_;
	};

}

#endif
