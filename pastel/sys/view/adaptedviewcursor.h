// Description: AdaptedViewCursor class
// Documentation: concrete_views.txt

#ifndef PASTELSYS_ADAPTEDVIEWCURSOR_H
#define PASTELSYS_ADAPTEDVIEWCURSOR_H

#include "pastel/sys/view/adaptedview.h"

namespace Pastel
{

	namespace AdaptedView_
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
			using Base = ConstProxy<Physical, Logical, Adapter, Cursor>;

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
				Adapter adapter;
				return adapter.convert(*cursor_) + that;
			}

			template <typename ThatLogical>
			Logical operator-(const ThatLogical& that) const
			{
				Adapter adapter;
				return adapter.convert(*cursor_) - that;
			}

			template <typename ThatLogical>
			Logical operator*(const ThatLogical& that) const
			{
				Adapter adapter;
				return adapter.convert(*cursor_) * that;
			}

			template <typename ThatLogical>
			Logical operator/(const ThatLogical& that) const
			{
				Adapter adapter;
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

		template <integer N, typename ConstCursor, typename Adapter>
		class ConstAdaptedViewCursor
		{
		protected:
			using PhysicalElement = typename ConstCursor::Element;
			using LogicalElement = typename Adapter::Logical;

		public:
			using Element = LogicalElement;
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

		template <integer N, typename Cursor, typename Adapter>
		class AdaptedViewCursor
			: public ConstAdaptedViewCursor<N, Cursor, Adapter>
		{
		private:
			using Base = ConstAdaptedViewCursor<N, Cursor, Adapter>;

			using LogicalElement = typename Base::LogicalElement;
			using PhysicalElement = typename Base::PhysicalElement;

			using Base::cursor_;
			using Base::adapter_;

		public:
			using Element = typename Base::Element; 
			using ConstReference = typename Base::ConstReference;

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
