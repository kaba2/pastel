// Description: Rectangle iterator
// Documentation: iterators.txt

#ifndef PASTEL_RECTANGLEITERATOR_H
#define PASTEL_RECTANGLEITERATOR_H

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N_>
	class RectangleIterator
	{
	public:
		enum
		{
			N = N_
		};

		explicit RectangleIterator(
			const Vector<integer, N>& max)
			: position_(ofDimension(max.dimension()), 0)
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
			const integer n = dimension();

			integer i = 0;
			while(i < n)
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
				++i;
			}
			done_ = (i == n);

			return *this;
		}

		RectangleIterator& operator--()
		{
			const integer n = dimension();

			integer i = 0;
			while(i < n)
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
				++i;
			}
			done_ = (i == n);

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

		integer dimension() const
		{
			return position_.dimension();
		}

	private:
		Vector<integer, N> position_;
		Vector<integer, N> min_;
		Vector<integer, N> max_;
		bool done_;
	};

}

#endif
