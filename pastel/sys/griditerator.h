#ifndef PASTEL_GRIDITERATOR_H
#define PASTEL_GRIDITERATOR_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N_>
	class GridIterator
	{
	public:
		enum
		{
			N = N_
		};

		explicit GridIterator(
			const Vector<integer, N>& extent)
			: position_(ofDimension(extent.dimension()), 0)
			, extent_(extent)
			, done_(anyEqual(extent, 0))
		{
		}

		bool done() const
		{
			return done_;
		}

		GridIterator& operator++()
		{
			const integer n = dimension();

			integer i = 0;
			while(i < n)
			{
				++position_[i];
				if (position_[i] >= extent_[i])
				{
					ASSERT_OP(position_[i], ==, extent_[i]);
					position_[i] = 0;
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

		GridIterator& operator--()
		{
			const integer n = dimension();

			integer i = 0;
			while(i < n)
			{
				--position_[i];
				if (position_[i] < 0)
				{
					ASSERT_OP(position_[i], ==, -1);
					position_[i] = extent_[i] - 1;
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

		const Vector<integer, N>& extent() const
		{
			return extent_;
		}

		integer dimension() const
		{
			return position_.dimension();
		}

	private:
		Vector<integer, N> position_;
		Vector<integer, N> extent_;
		bool done_;
	};

}

#endif
