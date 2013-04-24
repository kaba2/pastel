// Description: Cursor for Array

#ifndef PASTELSYS_ARRAY_CURSOR_H
#define PASTELSYS_ARRAY_CURSOR_H

#include "pastel/sys/array.h"

namespace Pastel
{

	template <typename Type, int N>
	class Array;

	namespace Array_
	{

		template <typename Type, int N>
		class ConstCursor
			: boost::equality_comparable<ConstCursor<Type, N> >
		{
		public:
			template <typename, int>
			friend class Array;

			typedef Type Element;
			typedef Type const& ConstReference;

			ConstCursor()
				: data_(0)
				, factor_(0)
			{
			}

			void swap(ConstCursor<Type, N>& that)
			{
				std::swap(data_, that.data_);
				std::swap(factor_, that.factor_);
			}

			void increment(integer index)
			{
				data_ += factor_[index];
			}

			void decrement(integer index)
			{
				data_ -= factor_[index];
			}

			void move(integer index, integer amount)
			{
				data_ += amount * factor_[index];
			}

			void move(const Vector<integer, N>& amount)
			{
				data_ += dot(amount, factor_);
			}

			Type const* operator->() const
			{
				PENSURE(data_);
				return data_;
			}

			Type const& operator*() const
			{
				PENSURE(data_);
				return *data_;
			}

			bool operator==(const ConstCursor& that) const
			{
				return data_ == that.data_;
			}

			// 1-dimensional special functions

			void xIncrement()
			{
				increment(0);
			}

			void xDecrement()
			{
				decrement(0);
			}

			void xMove(integer amount)
			{
				move(0, amount);
			}

			// 2-dimensional special functions

			void yIncrement()
			{
				increment(1);
			}

			void yDecrement()
			{
				decrement(1);
			}

			void yMove(integer amount)
			{
				move(1, amount);
			}

			// 3-dimensional special functions

			void zIncrement()
			{
				increment(2);
			}

			void zDecrement()
			{
				decrement(2);
			}

			void zMove(integer amount)
			{
				move(2, amount);
			}

		//protected:
			ConstCursor(Type const* data,
				const Vector<integer, N>& factor)
				: data_(data)
				, factor_(factor)
			{
			}
			
		protected:
			Type const* data_;
			Vector<integer, N> factor_;
		};

		template <typename Type, int N>
		class Cursor
			: public ConstCursor<Type, N>
		{
		private:
			template <typename, int>
			friend class Array;

			typedef ConstCursor<Type, N> Base;

			using Base::data_;

		public:
			typedef Type Element;
			typedef Type& Reference;

			Cursor()
				: Base()
			{
			}

			void swap(Cursor<Type, N>& that)
			{
				Base::swap(that);
			}

			Type* operator->() const
			{
				PENSURE(data_);
				return (Type*)data_;
			}

			Type& operator*() const
			{
				PENSURE(data_);
				return *((Type*)data_);
			}

			bool operator==(const Cursor& that) const
			{
				return (const ConstCursor<Type, N>&)*this == 
					(const ConstCursor<Type, N>&)that;
			}

		//protected:
			Cursor(Type* data,
				const Vector<integer, N>& factor)
				: Base(data, factor)
			{
			}
		};

	}

}

#endif
