// Description: ArrayBaseCursor class
// Detail: Cursor for the ArrayBase class

#ifndef PASTEL_ARRAYBASECURSOR_H
#define PASTEL_ARRAYBASECURSOR_H

#include "pastel/sys/arraybase.h"

namespace Pastel
{

	namespace Detail_Array
	{

		template <typename Type, int N>
		class ArrayBase;

		template <typename Type, int N>
		class ConstCursor;

		template <typename Type, int N>
		class ConstCursorBase
		{
		public:
			typedef Type Element;
			typedef Type const& ConstReference;

			ConstCursorBase()
				: data_(0)
				, factor_(0)
			{
			}

			void swap(ConstCursor<Type, N>& that)
			{
				std::swap(data_, that.data_);
				std::swap(factor_, that.factor_);
			}

			template <int Index>
			void increment()
			{
				data_ += factor_[Index];
			}

			template <>
			void increment<0>()
			{
				++data_;
			}

			void increment(integer index)
			{
				data_ += factor_[index];
			}

			template <int Index>
			void decrement()
			{
				data_ -= factor_[Index];
			}

			template <>
			void decrement<0>()
			{
				--data_;
			}

			void decrement(integer index)
			{
				data_ -= factor_[index];
			}

			template <int Index>
			void move(integer amount)
			{
				data_ += amount * factor_[Index];
			}

			template <>
			void move<0>(integer amount)
			{
				data_ += amount;
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

		protected:
			ConstCursorBase(Type const* data,
				const Vector<integer, N>& factor)
				: data_(data)
				, factor_(factor)
			{
			}

			Type const* data_;
			Vector<integer, N> factor_;
		};

		template <typename Type, int N>
		class ConstCursor
			: public ConstCursorBase<Type, N>
		{
		private:
			template <typename Type, int N>
			friend class ArrayBase;

			typedef ConstCursorBase<Type, N> Base;

		public:
			ConstCursor()
				: Base()
			{
			}

		protected:
			ConstCursor(Type const* data,
				const Vector<integer, N>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class ConstCursor<Type, 1>
			: public ConstCursorBase<Type, 1>
		{
		private:
			template <typename Type, int N>
			friend class ArrayBase;

			typedef ConstCursorBase<Type, 1> Base;

		public:
			using Base::move;

			ConstCursor()
				: Base()
			{
			}

			void xIncrement()
			{
				Base::increment<0>();
			}

			void xDecrement()
			{
				Base::decrement<0>();
			}

			void xMove(integer amount)
			{
				Base::move<0>(amount);
			}

			void move(integer xAmount)
			{
				Base::move(Vector<integer, 1>(xAmount));
			}

		protected:
			ConstCursor(Type const* data,
				const Vector<integer, 1>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class ConstCursor<Type, 2>
			: public ConstCursorBase<Type, 2>
		{
		private:
			template <typename Type, int N>
			friend class ArrayBase;

			typedef ConstCursorBase<Type, 2> Base;

		public:
			using Base::move;

			ConstCursor()
				: Base()
			{
			}

			void xIncrement()
			{
				Base::increment<0>();
			}

			void xDecrement()
			{
				Base::decrement<0>();
			}

			void yIncrement()
			{
				Base::increment<1>();
			}

			void yDecrement()
			{
				Base::decrement<1>();
			}

			void xMove(integer amount)
			{
				Base::move<0>(amount);
			}

			void yMove(integer amount)
			{
				Base::move<1>(amount);
			}

			void move(integer xAmount, integer yAmount)
			{
				Base::move(Vector<integer, 2>(xAmount, yAmount));
			}

		protected:
			ConstCursor(Type const* data,
				const Vector<integer, 2>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class ConstCursor<Type, 3>
			: public ConstCursorBase<Type, 3>
		{
		private:
			template <typename Type, int N>
			friend class ArrayBase;

			typedef ConstCursorBase<Type, 3> Base;

		public:
			using Base::move;

			ConstCursor()
				: Base()
			{
			}

			void xIncrement()
			{
				Base::increment<0>();
			}

			void xDecrement()
			{
				Base::decrement<0>();
			}

			void yIncrement()
			{
				Base::increment<1>();
			}

			void yDecrement()
			{
				Base::decrement<1>();
			}

			void zIncrement()
			{
				Base::increment<2>();
			}

			void zDecrement()
			{
				Base::decrement<2>();
			}

			void xMove(integer amount)
			{
				Base::move<0>(amount);
			}

			void yMove(integer amount)
			{
				Base::move<1>(amount);
			}

			void zMove(integer amount)
			{
				Base::move<2>(amount);
			}

			void move(integer xAmount, integer yAmount, integer zAmount)
			{
				Base::move(Vector<integer, 3>(xAmount, yAmount, zAmount));
			}

		protected:
			ConstCursor(Type const* data,
				const Vector<integer, 3>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type, int N>
		class Cursor;

		template <typename Type, int N>
		class CursorBase
			: public ConstCursor<Type, N>
		{
		private:
			typedef ConstCursor<Type, N> Base;

			using Base::data_;

		public:
			typedef Type Element;
			typedef Type& Reference;

			CursorBase()
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

		protected:
			CursorBase(Type* data,
				const Vector<integer, N>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type, int N>
		class Cursor
			: public CursorBase<Type, N>
		{
		private:
			template <typename Type, int N>
			friend class ArrayBase;

			typedef CursorBase<Type, N> Base;

		public:
			Cursor()
				: Base()
			{
			}

		private:
			Cursor(Type* data,
				const Vector<integer, N>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class Cursor<Type, 1>
			: public CursorBase<Type, 1>
		{
		private:
			template <typename Type, int N>
			friend class ArrayBase;

			typedef CursorBase<Type, 1> Base;

		public:
			using Base::move;

			Cursor()
				: Base()
			{
			}

			void xIncrement()
			{
				Base::increment<0>();
			}

			void xDecrement()
			{
				Base::decrement<0>();
			}

			void xMove(integer amount)
			{
				Base::move<0>(amount);
			}

			void move(integer xAmount)
			{
				Base::move(Vector<integer, 1>(xAmount));
			}

		private:
			Cursor(Type* data,
				const Vector<integer, 1>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class Cursor<Type, 2>
			: public CursorBase<Type, 2>
		{
		private:
			template <typename Type, int N>
			friend class ArrayBase;

			typedef CursorBase<Type, 2> Base;

		public:
			using Base::move;

			Cursor()
				: Base()
			{
			}

			void xIncrement()
			{
				Base::increment<0>();
			}

			void xDecrement()
			{
				Base::decrement<0>();
			}

			void yIncrement()
			{
				Base::increment<1>();
			}

			void yDecrement()
			{
				Base::decrement<1>();
			}

			void xMove(integer amount)
			{
				Base::move<0>(amount);
			}

			void yMove(integer amount)
			{
				Base::move<1>(amount);
			}

			void move(integer xAmount, integer yAmount)
			{
				Base::move(Vector<integer, 2>(xAmount, yAmount));
			}

		private:
			Cursor(Type* data,
				const Vector<integer, 2>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class Cursor<Type, 3>
			: public CursorBase<Type, 3>
		{
		private:
			template <typename Type, int N>
			friend class ArrayBase;

			typedef CursorBase<Type, 3> Base;

		public:
			using Base::move;

			Cursor()
				: Base()
			{
			}

			void xIncrement()
			{
				Base::increment<0>();
			}

			void xDecrement()
			{
				Base::decrement<0>();
			}

			void yIncrement()
			{
				Base::increment<1>();
			}

			void yDecrement()
			{
				Base::decrement<1>();
			}

			void zIncrement()
			{
				Base::increment<2>();
			}

			void zDecrement()
			{
				Base::decrement<2>();
			}

			void xMove(integer amount)
			{
				Base::move<0>(amount);
			}

			void yMove(integer amount)
			{
				Base::move<1>(amount);
			}

			void zMove(integer amount)
			{
				Base::move<2>(amount);
			}

			void move(integer xAmount, integer yAmount, integer zAmount)
			{
				Base::move(Vector<integer, 3>(xAmount, yAmount, zAmount));
			}

		private:
			Cursor(Type* data,
				const Vector<integer, 3>& factor)
				: Base(data, factor)
			{
			}
		};

	}

}

#endif
