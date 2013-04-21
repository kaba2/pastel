// Description: Read protection

#ifndef PASTELSYS_READ_PROTECTED_H
#define PASTELSYS_READ_PROTECTED_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <utility>
#include <memory>

namespace Pastel
{

	template <typename Type>
	class ReadProtected;

	template <typename Type>
	class ReadPtr
	{
	public:
		ReadPtr()
			: read_()
		{
		}

		ReadPtr(const ReadPtr& that)
			: read_(that.read_)
		{
		}

		ReadPtr(ReadPtr&& that)
			: read_(std::move(that.read_))
		{
		}

		template <typename That>
		bool operator==(const ReadPtr<That>& that) const
		{
			return get() == that.get();
		}

		template <typename That>
		bool operator!=(const ReadPtr<That>& that) const
		{
			return !(*this == that);
		}

		void swap(ReadPtr& that)
		{
			read_.swap(that.read_);
		}

		void clear()
		{
			read_.reset();
		}

		integer readCount() const
		{
			return read_.use_count();
		}

		const Type* get() const
		{
			return *read_;
		}

		const Type* operator->() const
		{
			return *read_;
		}

		const Type& operator*() const
		{
			return **read_;
		}

	private:
		friend class ReadProtected<Type>;

		explicit ReadPtr(const Type* that)
			: read_(new const Type*)
		{
			*read_ = that;
		}

		void swapPtr(ReadPtr& that)
		{
			ASSERT(read_ && that.read_);
			std::swap(*read_, *that.read_);
		}

		std::shared_ptr<const Type*> read_;
	};

	template <typename Type>
	class ReadProtected
	{
	public:
		explicit ReadProtected(const Type* that)
			: read_(that)
		{
		}

		~ReadProtected()
		{
			readProtect();
		}

		void readProtect()
		{
			ENSURE_OP(readCount(), ==, 1);
		}

		integer readCount() const
		{
			return read_.readCount();
		}

		void swap(ReadProtected& that)
		{
			read_.swap(that.read_);
			read_.swapPtr(that.read_);
		}

		ReadPtr<Type> read() const
		{
			return read_;
		}

	private:
		ReadProtected();
		ReadProtected(ReadProtected&& that);
		ReadProtected(const ReadProtected& that);
		ReadProtected& operator=(ReadProtected that);

		ReadPtr<Type> read_;
	};

}

#endif
