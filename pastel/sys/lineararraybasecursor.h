#ifndef PASTELSYS_LINEARARRAYBASECURSOR_H
#define PASTELSYS_LINEARARRAYBASECURSOR_H

#include "pastel/sys/lineararraybase.h"

namespace Pastel
{

	namespace Detail_LinearArray
	{

		template <int N, typename Type, typename Derived>
		class LinearArrayBase;

		template <int N, typename Type, typename Derived>
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

			void swap(Derived& that)
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

			void move(const Vector<N, integer>& amount)
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
				const Vector<N, integer>& factor)
				: data_(data)
				, factor_(factor)
			{
			}

			Type const* data_;
			Vector<N, integer> factor_;
		};

		template <int N, typename Type>
		class ConstCursor
			: public ConstCursorBase<N, Type, ConstCursor<N, Type> >
		{
		private:
			template <int N, typename Type, typename Derived>
			friend class LinearArrayBase;

			typedef ConstCursorBase<N, Type, ConstCursor<N, Type> > Base;

		public:
			ConstCursor()
				: Base()
			{
			}

		protected:
			ConstCursor(Type const* data, 
				const Vector<N, integer>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class ConstCursor<1, Type>
			: public ConstCursorBase<1, Type, ConstCursor<1, Type> >
		{
		private:
			template <int N, typename Type, typename Derived>
			friend class LinearArrayBase;

			typedef ConstCursorBase<1, Type, ConstCursor<1, Type> > Base;

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
				Base::move(Vector<1, integer>(xAmount));
			}

		protected:
			ConstCursor(Type const* data, 
				const Vector<1, integer>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class ConstCursor<2, Type>
			: public ConstCursorBase<2, Type, ConstCursor<2, Type> >
		{
		private:
			template <int N, typename Type, typename Derived>
			friend class LinearArrayBase;

			typedef ConstCursorBase<2, Type, ConstCursor<2, Type> > Base;

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
				Base::move(Vector<2, integer>(xAmount, yAmount));
			}

		protected:
			ConstCursor(Type const* data, 
				const Vector<2, integer>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class ConstCursor<3, Type>
			: public ConstCursorBase<3, Type, ConstCursor<3, Type> >
		{
		private:
			template <int N, typename Type, typename Derived>
			friend class LinearArrayBase;

			typedef ConstCursorBase<3, Type, ConstCursor<3, Type> > Base;

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
				Base::move(Vector<3, integer>(xAmount, yAmount, zAmount));
			}

		protected:
			ConstCursor(Type const* data, 
				const Vector<3, integer>& factor)
				: Base(data, factor)
			{
			}
		};

		template <int N, typename Type, typename Derived>
		class CursorBase
			: public ConstCursor<N, Type>
		{
		private:
			typedef ConstCursor<N, Type> Base;

			using Base::data_;

		public:
			typedef Type Element;
			typedef Type& Reference;

			CursorBase()
				: Base()
			{
			}

			void swap(Derived& that)
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
				const Vector<N, integer>& factor)
				: Base(data, factor)
			{
			}
		};

		template <int N, typename Type>
		class Cursor
			: public CursorBase<N, Type, Cursor<N, Type> >
		{
		private:
			template <int N, typename Type, typename Derived>
			friend class LinearArrayBase;

			typedef CursorBase<N, Type, Cursor<N, Type> > Base;

		public:
			Cursor()
				: Base()
			{
			}

		private:
			Cursor(Type* data, 
				const Vector<N, integer>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class Cursor<1, Type>
			: public CursorBase<1, Type, Cursor<1, Type> >
		{
		private:
			template <int N, typename Type, typename Derived>
			friend class LinearArrayBase;

			typedef CursorBase<1, Type, Cursor<1, Type> > Base;

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
				Base::move(Vector<1, integer>(xAmount));
			}

		private:
			Cursor(Type* data, 
				const Vector<1, integer>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class Cursor<2, Type>
			: public CursorBase<2, Type, Cursor<2, Type> >
		{
		private:
			template <int N, typename Type, typename Derived>
			friend class LinearArrayBase;

			typedef CursorBase<2, Type, Cursor<2, Type> > Base;

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
				Base::move(Vector<2, integer>(xAmount, yAmount));
			}

		private:
			Cursor(Type* data, 
				const Vector<2, integer>& factor)
				: Base(data, factor)
			{
			}
		};

		template <typename Type>
		class Cursor<3, Type>
			: public CursorBase<3, Type, Cursor<3, Type> >
		{
		private:
			template <int N, typename Type, typename Derived>
			friend class LinearArrayBase;

			typedef CursorBase<3, Type, Cursor<3, Type> > Base;

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
				Base::move(Vector<3, integer>(xAmount, yAmount, zAmount));
			}

		private:
			Cursor(Type* data, 
				const Vector<3, integer>& factor)
				: Base(data, factor)
			{
			}
		};

	}

}

#endif
