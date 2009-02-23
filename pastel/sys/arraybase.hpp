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
			construct(extent, defaultData);
		}

		template <int N, typename Type, typename Derived>
		ArrayBase<N, Type, Derived>::ArrayBase(
			const ArrayBase<N, Type, Derived>& that)
			: extent_(0)
			, factor_(0)
			, size_(0)
			, data_(0)
		{
			construct(that.extent_, Type());
			copy(that, extent_, Type());
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
			construct(extent, defaultData);
			copy(that, extent_, defaultData);
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
				destruct(data_, data_ + product(extent_));
				delete[] ((int8*)data_);
				data_ = 0;
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
		void ArrayBase<N, Type, Derived>::construct(
			const Vector<N, integer>& extent,
			const Type& defaultData)
		{
			// Allocate raw memory.

			Vector<N, integer> newFactor;

			newFactor[0] = 1;

			for (integer i = 1;i < N;++i)
			{
				newFactor[i] = newFactor[i - 1] * extent[i - 1];
			}

			const integer units = product(extent);
			int8* newData = new int8[units * sizeof(Type)];

			try
			{
				// Construct the objects in the array.
				std::uninitialized_fill_n((Type*)newData,
					units, defaultData);
			}
			catch(...)
			{
				delete[] newData;
				newData = 0;
				throw;
			}

			// Commit

			factor_ = newFactor;
			extent_ = extent;
			size_ = product(extent);
			data_ = (Type*)newData;
		}

		template <int N, typename Type, typename Derived>
		void ArrayBase<N, Type, Derived>::copy(
			const ArrayBase<N, Type, Derived>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData)
		{
			for (integer i = 0;i < N;++i)
			{
				ASSERT2(0 <= extent[i] && extent[i] <= extent_[i], extent[i], extent_[i]);
			}

			const integer units = product(extent);

			if (units == 0)
			{
				return;
			}

			const Vector<N, integer> minExtent = min(extent, that.extent_);

			integer outOfCopyZone = 0;
			integer traversed = 0;
			Point<N, integer> position(0);

			while(traversed < units)
			{
				if (outOfCopyZone == 0)
				{
					for (position[0] = 0;position[0] < minExtent[0];++position[0])
					{
						(*this)(position) = that(position);
					}
					for (position[0] = minExtent[0];position[0] < extent[0];++position[0])
					{
						(*this)(position) = defaultData;
					}
				}
				else
				{
					for (position[0] = 0;position[0] < extent[0];++position[0])
					{
						(*this)(position) = defaultData;
					}
				}

				for (integer i = 1;i < N;++i)
				{
					++position[i];
					if (position[i] == minExtent[i])
					{
						++outOfCopyZone;
					}
					if (position[i] == extent[i])
					{
						position[i] = 0;
						--outOfCopyZone;
					}
					else
					{
						break;
					}
				}

				traversed += extent[0];
			}
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
