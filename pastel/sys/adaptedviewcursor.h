// Description: AdaptedViewCursor class
// Documentation: view.txt

#ifndef PASTEL_ADAPTEDVIEWCURSOR_H
#define PASTEL_ADAPTEDVIEWCURSOR_H

#include "pastel/sys/adaptedview.h"

namespace Pastel
{

	namespace Detail_AdaptedView
	{

		template <typename Physical, typename Logical,
			typename Adapter, typename ConstCursor>
		class ConstProxy
		{
		public:
			explicit ConstProxy(
				const ConstCursor& cursor)
				: cursor_(cursor)
			{
			}

			operator Logical() const
			{
				Adapter adapter;

				return adapter.convert(*cursor_);
			}

		protected:
			ConstCursor cursor_;
		};

		template <typename Physical, typename Logical,
			typename Adapter, typename Cursor>
		class Proxy
			: public ConstProxy<Physical, Logical, Adapter, Cursor>
		{
		private:
			typedef ConstProxy<Physical, Logical, Adapter, Cursor> Base;

			using Base::cursor_;

		public:
			explicit Proxy(const Cursor& cursor)
				: Base(cursor)
			{
			}

			operator Logical() const
			{
				Adapter adapter;

				return adapter.convert(*cursor_);
			}

			template <typename ThatLogical>
			Proxy<Physical, Logical, Adapter, Cursor>& operator=(
				const ThatLogical& that)
			{
				Adapter adapter;

				*cursor_ = adapter.revert(that);
				return *this;
			}

			template <typename ThatLogical>
			Proxy<Physical, Logical, Adapter, Cursor>& operator*=(
				const ThatLogical& that)
			{
				Adapter adapter;

				*cursor_ = adapter.revert(
					adapter.convert(*cursor_) * that);
				return *this;
			}

			template <typename ThatLogical>
			Proxy<Physical, Logical, Adapter, Cursor>& operator/=(
				const ThatLogical& that)
			{
				Adapter adapter;

				*cursor_ = adapter.revert(
					adapter.convert(*cursor_) / that);
				return *this;
			}

			template <typename ThatLogical>
			Proxy<Physical, Logical, Adapter, Cursor>& operator+=(
				const ThatLogical& that)
			{
				Adapter adapter;

				*cursor_ = adapter.revert(
					adapter.convert(*cursor_) + that);
				return *this;
			}

			template <typename ThatLogical>
			Proxy<Physical, Logical, Adapter, Cursor>& operator-=(
				const ThatLogical& that)
			{
				Adapter adapter;

				*cursor_ = adapter.revert(
					adapter.convert(*cursor_) - that);
				return *this;
			}

			template <typename ThatLogical>
			Logical operator+(const ThatLogical& that) const
			{
				return adapter.convert(*cursor_) + that;
			}

			template <typename ThatLogical>
			Logical operator-(const ThatLogical& that) const
			{
				return adapter.convert(*cursor_) - that;
			}

			template <typename ThatLogical>
			Logical operator*(const ThatLogical& that) const
			{
				return adapter.convert(*cursor_) * that;
			}

			template <typename ThatLogical>
			Logical operator/(const ThatLogical& that) const
			{
				return adapter.convert(*cursor_) / that;
			}
		};

		template <typename Physical, typename Logical,
			typename Adapter, typename Cursor, typename ThatLogical>
		Logical operator+(const ThatLogical& that,
			const Proxy<Physical, Logical, Adapter, Cursor>& proxy)
		{
			return that + (Logical)proxy;
		}

		template <typename Physical, typename Logical,
			typename Adapter, typename Cursor, typename ThatLogical>
		Logical operator-(const ThatLogical& that,
			const Proxy<Physical, Logical, Adapter, Cursor>& proxy)
		{
			return that - (Logical)proxy;
		}

		template <typename Physical, typename Logical,
			typename Adapter, typename Cursor, typename ThatLogical>
		Logical operator*(const ThatLogical& that,
			const Proxy<Physical, Logical, Adapter, Cursor>& proxy)
		{
			return that * (Logical)proxy;
		}

		template <typename Physical, typename Logical,
			typename Adapter, typename Cursor, typename ThatLogical>
		Logical operator/(const ThatLogical& that,
			const Proxy<Physical, Logical, Adapter, Cursor>& proxy)
		{
			return that / (Logical)proxy;
		}

		template <int N, typename ConstCursor, typename Adapter>
		class ConstAdaptedViewCursor
		{
		protected:
			typedef typename ConstCursor::Element PhysicalElement;
			typedef typename Adapter::Logical LogicalElement;

		public:
			typedef LogicalElement Element;
			typedef ConstProxy<PhysicalElement, LogicalElement, Adapter, ConstCursor>
				ConstReference;

			ConstAdaptedViewCursor()
				: cursor_()
				, adapter_()
			{
			}

			explicit ConstAdaptedViewCursor(
				const ConstCursor& cursor,
				const Adapter& adapter)
				: cursor_(cursor)
				, adapter_(adapter)
			{
			}

			void swap(ConstAdaptedViewCursor& that)
			{
				cursor_.swap(that.cursor_);
				adapter_.swap(that.adapter_);
			}

			void increment(integer index)
			{
				cursor_.increment(index);
			}

			void decrement(integer index)
			{
				cursor_.decrement(index);
			}

			void move(integer index, integer amount)
			{
				cursor_.move(index, amount);
			}

			void move(const Vector<integer, N>& amount)
			{
				cursor_.move(amount);
			}

			const ConstReference* operator->() const
			{
				return &ConstReference(cursor_);
			}

			ConstReference operator*() const
			{
				return ConstReference(cursor_);
			}

		protected:
			ConstCursor cursor_;
			Adapter adapter_;
		};

		template <int N, typename Cursor, typename Adapter>
		class AdaptedViewCursor
			: public ConstAdaptedViewCursor<N, Cursor, Adapter>
		{
		private:
			typedef ConstAdaptedViewCursor<N, Cursor, Adapter> Base;

			using typename Base::LogicalElement;
			using typename Base::PhysicalElement;

			using Base::cursor_;
			using Base::adapter_;

		public:
			using Base::Element;
			using Base::ConstReference;

			typedef Proxy<PhysicalElement, LogicalElement, Adapter, Cursor>
				Reference;

			using Base::increment;
			using Base::decrement;
			using Base::move;

			friend class ConstAdaptedViewCursor<N, Cursor, Adapter>;

			AdaptedViewCursor()
				: Base()
			{
			}

			explicit AdaptedViewCursor(
				const Cursor& cursor,
				const Adapter& adapter)
				: Base(cursor, adapter)
			{
			}

			void swap(AdaptedViewCursor& that)
			{
				Base::swap(that);
			}

			const Reference* operator->() const
			{
				return &Reference(cursor_);
			}

			Reference operator*() const
			{
				return Reference(cursor_);
			}
		};

	}

}

#endif
