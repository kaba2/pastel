#ifndef PASTEL_READ_PROTECTED_H
#define PASTEL_READ_PROTECTED_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <utility>

namespace Pastel
{

	template <typename Type, typename Base = EmptyClass>
	class ReadProtected;

	template <typename Type>
	class ReadPtr
	{
	public:
		ReadPtr()
			: data_(0)
			, count_(0)
		{
		}

		ReadPtr(ReadPtr<Type>&& that)
			: data_(0)
			, count_(0)
		{
			swap(that);
		}

		ReadPtr(const ReadPtr<Type>& that)
			: data_(that.data_)
			, count_(that.count_)
		{
			increaseCount();
		}

		~ReadPtr()
		{
			clear();
		}

		ReadPtr& operator=(ReadPtr that)
		{
			swap(that);
			return *this;
		}

		const Type& operator*() const
		{
			return data_;
		}

		const Type* operator->() const
		{
			return data_;
		}

		void swap(ReadPtr& that)
		{
			using std::swap;
			swap(data_, that.data_);
			swap(count_, that.count_);
		}

		void clear()
		{
			decreaseCount();
			data_ = 0;
			count_ = 0;
		}

		integer count() const
		{
			ASSERT(count_);
			return *count_;
		}

	private:
		template <typename Type, typename Base>
		friend class ReadProtected;

		explicit ReadPtr(const Type* data, integer* count)
			: data_(data)
			, count_(count)
		{
			increaseCount();
		}

		void increaseCount()
		{
			if (count_)
			{
				++(*count_);
			}
		}

		void decreaseCount()
		{
			if (count_)
			{
				--(*count_);
			}
		}

		const Type* data_;
		integer* count_;
	};

	template <typename Type, typename Base>
	class ReadProtected
		: public Base
	{
	public:
		// Note that a reference counter counts
		// the references to the _memory address_.
		// Thus assignment, copy construction and
		// swap must not influence the reference count.
		// In a way we should then prohibit them,
		// but in this special case it is easier
		// to define these functions but not
		// copy the reference counts. This
		// way the derived classes can use default
		// compiler implementations of copy constructors etc.

		ReadProtected()
			: readCount_(0)
		{
		}

		ReadProtected(
			const ReadProtected& that)
			: readCount_(0)
		{
		}

		ReadProtected& operator=(const ReadProtected& that)
		{
			return *this;
		}

		~ReadProtected()
		{
			readProtect();
		}

		operator ReadPtr<Type>() const
		{
			return ReadPtr<Type>((const Type*)this, &readCount_);
		}

	protected:
		void readProtect()
		{
			ENSURE_OP(readCount_, ==, 0);
		}

		mutable integer readCount_;
	};

}

#endif
