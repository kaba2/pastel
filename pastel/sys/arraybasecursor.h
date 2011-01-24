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
			: boost::equality_comparable<ConstCursorBase<Type, N> >
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

			bool operator==(const ConstCursorBase& that) const
			{
				return data_ == that.data_;
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
			template <typename, int>
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
			template <typename, int>
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
				Base::increment(0);
			}

			void xDecrement()
			{
				Base::decrement(0);
			}

			void xMove(integer amount)
			{
				Base::move(0, amount);
			}

			void move(integer xAmount)
			{
				Base::move(Vector1i(xAmount));
			}

		protected:
			ConstCursor(Type const* data,
				const Vector1i& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class ConstCursor<Type, 2>
			: public ConstCursorBase<Type, 2>
		{
		private:
			template <typename, int>
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
				Base::increment(0);
			}

			void xDecrement()
			{
				Base::decrement(0);
			}

			void yIncrement()
			{
				Base::increment(1);
			}

			void yDecrement()
			{
				Base::decrement(1);
			}

			void xMove(integer amount)
			{
				Base::move(0, amount);
			}

			void yMove(integer amount)
			{
				Base::move(1, amount);
			}

			void move(integer xAmount, integer yAmount)
			{
				Base::move(Vector2i(xAmount, yAmount));
			}

		protected:
			ConstCursor(Type const* data,
				const Vector2i& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class ConstCursor<Type, 3>
			: public ConstCursorBase<Type, 3>
		{
		private:
			template <typename, int>
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
				Base::increment(0);
			}

			void xDecrement()
			{
				Base::decrement(0);
			}

			void yIncrement()
			{
				Base::increment(1);
			}

			void yDecrement()
			{
				Base::decrement(1);
			}

			void zIncrement()
			{
				Base::increment(2);
			}

			void zDecrement()
			{
				Base::decrement(2);
			}

			void xMove(integer amount)
			{
				Base::move(0, amount);
			}

			void yMove(integer amount)
			{
				Base::move(1, amount);
			}

			void zMove(integer amount)
			{
				Base::move(2, amount);
			}

			void move(integer xAmount, integer yAmount, integer zAmount)
			{
				Base::move(Vector3i(
					xAmount, yAmount, zAmount));
			}

		protected:
			ConstCursor(Type const* data,
				const Vector3i& factor)
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

			bool operator==(const CursorBase& that) const
			{
				return (const ConstCursorBase&)*this == 
					(const ConstCursorBase&)that;
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
			template <typename, int>
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
			template <typename, int>
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
				Base::increment(0);
			}

			void xDecrement()
			{
				Base::decrement(0);
			}

			void xMove(integer amount)
			{
				Base::move(0, amount);
			}

			void move(integer xAmount)
			{
				Base::move(Vector1i(xAmount));
			}

		private:
			Cursor(Type* data,
				const Vector1i& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class Cursor<Type, 2>
			: public CursorBase<Type, 2>
		{
		private:
			template <typename, int>
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
				Base::increment(0);
			}

			void xDecrement()
			{
				Base::decrement(0);
			}

			void yIncrement()
			{
				Base::increment(1);
			}

			void yDecrement()
			{
				Base::decrement(1);
			}

			void xMove(integer amount)
			{
				Base::move(0, amount);
			}

			void yMove(integer amount)
			{
				Base::move(1, amount);
			}

			void move(integer xAmount, integer yAmount)
			{
				Base::move(Vector2i(
					xAmount, yAmount));
			}

		private:
			Cursor(Type* data,
				const Vector2i& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class Cursor<Type, 3>
			: public CursorBase<Type, 3>
		{
		private:
			template <typename, int>
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
				Base::increment(0);
			}

			void xDecrement()
			{
				Base::decrement(0);
			}

			void yIncrement()
			{
				Base::increment(1);
			}

			void yDecrement()
			{
				Base::decrement(1);
			}

			void zIncrement()
			{
				Base::increment(2);
			}

			void zDecrement()
			{
				Base::decrement(2);
			}

			void xMove(integer amount)
			{
				Base::move(0, amount);
			}

			void yMove(integer amount)
			{
				Base::move(1, amount);
			}

			void zMove(integer amount)
			{
				Base::move(2, amount);
			}

			void move(integer xAmount, integer yAmount, integer zAmount)
			{
				Base::move(Vector3i(
					xAmount, yAmount, zAmount));
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
