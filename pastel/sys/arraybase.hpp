#ifndef PASTELSYS_ARRAYBASE_HPP
#define PASTELSYS_ARRAYBASE_HPP

#include "pastel/sys/arraybase.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/destruct.h"
#include "pastel/sys/vector_tools.h"

#include <algorithm>

namespace Pastel
{

	namespace Detail_Array
	{

		template <int N, typename Type, typename Derived>
		ArrayBase<N, Type, Derived>::ArrayBase()
			: extent_(0)
			, factor_(0)
			, size_(0)
			, data_(0)
		{
		}

		template <int N, typename Type, typename Derived>
		ArrayBase<N, Type, Derived>::ArrayBase(
			const Vector<N, integer>& extent,
			const Type& defaultData)
			: extent_(0)
			, factor_(0)
			, size_(0)
			, data_(0)
		{
			allocate(extent);

			try
			{
				std::uninitialized_fill_n(data_,
					size_, defaultData);
			}
			catch(...)
			{
				deallocate();
				throw;
			};
		}

		template <int N, typename Type, typename Derived>
		ArrayBase<N, Type, Derived>::ArrayBase(
			const ArrayBase<N, Type, Derived>& that)
			: extent_(0)
			, factor_(0)
			, size_(0)
			, data_(0)
		{
			allocate(that.extent_);

			try
			{
				std::uninitialized_copy(
					that.data_, that.data_ + size_, data_);
			}
			catch(...)
			{
				deallocate();
				throw;
			};
		}

		template <int N, typename Type, typename Derived>
		ArrayBase<N, Type, Derived>::ArrayBase(
			const ArrayBase<N, Type, Derived>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData)
			: extent_(0)
			, factor_(0)
			, size_(0)
			, data_(0)
		{
			allocate(extent);

			try
			{
				if (that.size() == 0)
				{
					// Nothing to copy, pure
					// default initialization.
					std::uninitialized_fill_n(data_,
						size_, defaultData);
				}
				else if (extent_ == that.extent_)
				{
					// Nothing to default initialize,
					// pure copy construction.
					std::uninitialized_copy(
						that.data_, that.data_ + size_, data_);
				}
				else
				{
					// Both copy construction and
					// default initialization needed.
					copyConstruct(that, defaultData);
				}
			}
			catch(...)
			{
				deallocate();
				throw;
			};
		}

		template <int N, typename Type, typename Derived>
		ArrayBase<N, Type, Derived>::~ArrayBase()
		{
			clear();
		}

		template <int N, typename Type, typename Derived>
		void ArrayBase<N, Type, Derived>::clear()
		{
			if (data_)
			{
				destruct(data_, data_ + size_);
				deallocate();
			}

			extent_.set(0);
			factor_.set(0);
			size_ = 0;
		}

		template <int N, typename Type, typename Derived>
		void ArrayBase<N, Type, Derived>::swap(ArrayBase<N, Type, Derived>& that)
		{
			extent_.swap(that.extent_);
			factor_.swap(that.factor_);
			std::swap(size_, that.size_);
			std::swap(data_, that.data_);
		}

		template <int N, typename Type, typename Derived>
		void ArrayBase<N, Type, Derived>::setExtent(
			const Vector<N, integer>& extent,
			const Type& defaultData)
		{
			for (integer i = 0;i < N;++i)
			{
				ENSURE1(extent[i] >= 0, extent[i]);
			}

			const integer units = product(extent);
			if (units == 0)
			{
				clear();
			}
			else
			{
				if (extent != extent_)
				{
					ArrayBase<N, Type, Derived> copy(*this, extent,
						defaultData);
					swap(copy);
				}
			}
		}

		template <int N, typename Type, typename Derived>
		const Vector<N, integer>& ArrayBase<N, Type, Derived>::extent() const
		{
			return extent_;
		}

		template <int N, typename Type, typename Derived>
		bool ArrayBase<N, Type, Derived>::empty() const
		{
			return allEqual(extent(), 0);
		}

		template <int N, typename Type, typename Derived>
		integer ArrayBase<N, Type, Derived>::size() const
		{
			return size_;
		}

		template <int N, typename Type, typename Derived>
		ArrayBase<N, Type, Derived>& ArrayBase<N, Type, Derived>::operator=(
			const ArrayBase<N, Type, Derived>& that)
		{
			ArrayBase<N, Type, Derived> copy(that);
			swap(copy);

			return *this;
		}

		template <int N, typename Type, typename Derived>
		Type& ArrayBase<N, Type, Derived>::operator()(
			integer index)
		{
			return (Type&)((const ArrayBase&)*this)(index);
		}

		template <int N, typename Type, typename Derived>
		const Type& ArrayBase<N, Type, Derived>::operator()(
			integer index) const
		{
			PENSURE2(index >= 0 && index < size_, index, size_);

			return *((const Type*)data_ + index);
		}

		template <int N, typename Type, typename Derived>
		Type& ArrayBase<N, Type, Derived>::operator()(
			const Point<N, integer>& position)
		{
			return (Type&)((const ArrayBase&)*this)(position);
		}

		template <int N, typename Type, typename Derived>
		const Type& ArrayBase<N, Type, Derived>::operator()(
			const Point<N, integer>& position) const
		{
			for (integer i = 0;i < N;++i)
			{
				PENSURE3(0 <= position[i] && position[i] < extent_[i], position[i], extent_[i], i);
			}

			integer offset = position[0];

			for (integer i = 1;i < N;++i)
			{
				offset += position[i] * factor_[i];
			}

			return *((const Type*)data_ + offset);
		}

		// Private

		template <int N, typename Type, typename Derived>
		void ArrayBase<N, Type, Derived>::allocate(
			const Vector<N, integer>& extent)
		{
			ASSERT(data_ == 0);
			ASSERT(size_ == 0);

			const integer units = product(extent);
			if (units == 0)
			{
				return;
			}

			Type* newData = (Type*)allocateRaw(units * sizeof(Type));

			// Compute steps between consecutive
			// elements on each axis.

			Vector<N, integer> newFactor;

			newFactor[0] = 1;

			for (integer i = 1;i < N;++i)
			{
				newFactor[i] = newFactor[i - 1] * extent[i - 1];
			}

			factor_ = newFactor;
			extent_ = extent;
			size_ = units;
			data_ = newData;
		}

		template <int N, typename Type, typename Derived>
		void ArrayBase<N, Type, Derived>::deallocate()
		{
			deallocateRaw((void*)data_);
			data_ = 0;
			size_ = 0;
			factor_.set(0);
			extent_.set(0);
		}

		template <int N, typename Type, typename Derived>
		void ArrayBase<N, Type, Derived>::copyConstruct(
			const ArrayBase<N, Type, Derived>& that,
			const Type& defaultData)
		{
			if (size_ == 0)
			{
				return;
			}

			const Vector<N, integer> minExtent = min(extent_, that.extent_);

			integer outOfCopyZone = 0;
			integer traversed = 0;
			Point<N, integer> position(0);

			try
			{
				while(traversed < size_)
				{
					if (outOfCopyZone == 0)
					{
						std::uninitialized_copy(
							&that(position),
							&that(position) + minExtent[0],
							&(*this)(position));
						
						traversed += minExtent[0];

						if (extent_[0] > minExtent[0])
						{
							std::uninitialized_fill_n(
								&(*this)(position) + minExtent[0],
								extent_[0] - minExtent[0],
								defaultData);
							
							traversed -= minExtent[0];
							traversed += extent_[0];
						}
					}
					else
					{
						std::uninitialized_fill_n(
							&(*this)(position),
							extent_[0],
							defaultData);

						traversed += extent_[0];
					}

					for (integer i = 1;i < N;++i)
					{
						++position[i];
						if (position[i] == minExtent[i])
						{
							++outOfCopyZone;
						}
						if (position[i] == extent_[i])
						{
							position[i] = 0;
							--outOfCopyZone;
						}
						else
						{
							break;
						}
					}
				}
			}
			catch(...)
			{
				destruct(data_, data_ + traversed);
				throw;
			};
		}

		template <int N, typename Type, typename Derived>
		typename ArrayBase<N, Type, Derived>::Cursor
			ArrayBase<N, Type, Derived>::cursor(
			const Point<N, integer>& position)
		{
			return Cursor(&(*this)(position), factor_);
		}

		template <int N, typename Type, typename Derived>
		typename ArrayBase<N, Type, Derived>::ConstCursor
			ArrayBase<N, Type, Derived>::constCursor(
			const Point<N, integer>& position) const
		{
			return ConstCursor(&(*this)(position), factor_);
		}

	}

}

#endif
