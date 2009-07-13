#ifndef PASTEL_PARALLELARRAY_HPP
#define PASTEL_PARALLELARRAY_HPP

#include "pastel/sys/parallelarray.h"

namespace Pastel
{

	inline Detail::UnknownArray::~UnknownArray()
	{
	}

	template <typename Key>
	ParallelArray<Key>::ParallelArray()
		: arrays_()
		, size_(0)
	{
	}

	template <typename Key>
	ParallelArray<Key>::ParallelArray(
		const ParallelArray& that)
		: arrays_()
		, size_(that.size_)
	{
		ConstIterator iter(that.arrays_.begin());
		ConstIterator iterEnd(that.arrays_.end());

		while (iter != iterEnd)
		{
			UnknownArray* unknownArray = iter->second;
			std::auto_ptr<UnknownArray> copy(
				unknownArray->clone());

			arrays_.insert(std::make_pair(
				iter->first, copy.get()));

			copy.release();

			++iter;
		}
	}

	template <typename Key>
	ParallelArray<Key>::~ParallelArray()
	{
		freeMem();
	}

	template <typename Key>
	ParallelArray<Key>& ParallelArray<Key>::operator=(
		const ParallelArray<Key>& that)
	{
		ParallelArray<Key> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Key>
	void ParallelArray<Key>::freeMem()
	{
		Iterator iter(arrays_.begin());
		Iterator iterEnd(arrays_.end());

		while (iter != iterEnd)
		{
			UnknownArray* unknownArray = iter->second;
			delete unknownArray;
			++iter;
		}

		arrays_.clear();
		size_ = 0;
	}

	template <typename Key>
	void ParallelArray<Key>::swap(ParallelArray<Key>& that)
	{
		arrays_.swap(that.arrays_);
		std::swap(size_, that.size_);
	}

	template <typename Key>
	void ParallelArray<Key>::clear()
	{
		resize(0);
	}

	template <typename Key>
	bool ParallelArray<Key>::reserve(integer size)
	{
		ENSURE_OP(size, >=, 0);

		if (size_ >= size)
		{
			return false;
		}

		return resize(size);
	}

	template <typename Key>
	bool ParallelArray<Key>::resize(integer size)
	{
		ENSURE_OP(size, >=, 0);

		// Resize each of the arrays.

		Iterator iter(arrays_.begin());
		Iterator iterEnd(arrays_.end());

		bool reallocated = false;
		while (iter != iterEnd)
		{
			UnknownArray* unknownArray = iter->second;

			// Propagate the reallocation info.
			reallocated |= unknownArray->resize(size);
			++iter;
		}

		// Update size.

		size_ = size;

		return reallocated;
	}

	template <typename Key>
	integer ParallelArray<Key>::properties() const
	{
		return arrays_.size();
	}

	template <typename Key>
	integer ParallelArray<Key>::size() const
	{
		return size_;
	}

	template <typename Key>
	bool ParallelArray<Key>::empty() const
	{
		return (size_ == 0);
	}

	template <typename Key>
	template <typename Type>
	bool ParallelArray<Key>::add(
		const PropertyId<Key, Type>& key,
		const Type& defaultValue)
	{
		std::pair<Iterator, bool> result(
			arrays_.insert(std::make_pair(
			key.id(), (UnknownArray*)0)));

		bool alreadyExisted = !result.second;

		if (!alreadyExisted)
		{
			try
			{
				result.first->second =
					new KnownArray<Type>(size_, defaultValue);
			}
			catch(...)
			{
				arrays_.erase(result.first);
				throw;
			}
		}

		return !alreadyExisted;
	}

	template <typename Key>
	template <typename Type>
	bool ParallelArray<Key>::remove(
		const PropertyId<Key, Type>& key)
	{
		Iterator iter(
			arrays_.find(key.id()));
		if (iter == arrays_.end())
		{
			return false;
		}

		UnknownArray* unknownArray = iter->second;

		arrays_.erase(iter);

		delete unknownArray;

		return true;
	}

	template <typename Key>
	template <typename Type>
	bool ParallelArray<Key>::get(
		const PropertyId<Key, Type>& key,
		MemoryView<Type>& view)
	{
		Iterator iter(
			arrays_.find(key.id()));
		if (iter == arrays_.end())
		{
			return false;
		}

		UnknownArray* unknownArray = iter->second;
		KnownArray<Type>* knownArray =
			dynamic_cast<KnownArray<Type>*>(unknownArray);

		if (!knownArray)
		{
			return false;
		}

		if (knownArray->data_.empty())
		{
			view.clear();
			return true;
		}

		view.set(&knownArray->data_.front(), size_);

		return true;
	}

	template <typename Key>
	template <typename Type>
	bool ParallelArray<Key>::get(
		const PropertyId<Key, Type>& key,
		ConstMemoryView<Type>& view)
	{
		ConstIterator iter(arrays_.find(key.id()));
		if (iter == arrays_.end())
		{
			return false;
		}

		const UnknownArray* unknownArray = iter->second;
		const KnownArray<Type>* knownArray =
			dynamic_cast<const KnownArray<Type>*>(unknownArray);

		if (!knownArray)
		{
			return false;
		}

		if (knownArray->data_.empty())
		{
			view.clear();
			return true;
		}

		view.set(&(knownArray->data_[0]), size_);

		return true;
	}

	template <typename Key>
	template <typename Type>
	bool ParallelArray<Key>::get(
		const PropertyId<Key, Type>& key,
		ConstMemoryView<Type>& view) const
	{
		ConstIterator iter(arrays_.find(key.id()));
		if (iter == arrays_.end())
		{
			return false;
		}

		const UnknownArray* unknownArray = iter->second;
		const KnownArray<Type>* knownArray =
			dynamic_cast<const KnownArray<Type>*>(unknownArray);

		if (!knownArray)
		{
			return false;
		}

		if (knownArray->data_.empty())
		{
			view.clear();
			return true;
		}

		view.set(&(knownArray->data_[0]), size_);

		return true;
	}

	// ParallelArray<Key>::KnownArray<Type>

	template <typename Key>
	template <typename Type>
	ParallelArray<Key>::KnownArray<Type>::KnownArray()
		: data_()
		, defaultValue_()
	{
	}

	template <typename Key>
	template <typename Type>
	ParallelArray<Key>::KnownArray<Type>::KnownArray(
		integer size,
		const Type& defaultValue)
		: data_(size, defaultValue)
		, defaultValue_(defaultValue)
	{
	}

	template <typename Key>
	template <typename Type>
	ParallelArray<Key>::KnownArray<Type>::KnownArray(
		const KnownArray<Type>& that)
		: data_(that.data_)
		, defaultValue_(that.defaultValue_)
	{
	}

	template <typename Key>
	template <typename Type>
	ParallelArray<Key>::KnownArray<Type>::~KnownArray()
	{
	}

	template <typename Key>
	template <typename Type>
	bool ParallelArray<Key>::KnownArray<Type>::resize(integer width)
	{
		// The possible reallocation is detected
		// by checking the memory address
		// of the (array contained in) std::vector
		// before and after the resizing.

		Type* before = 0;
		if (!data_.empty())
		{
			before = &data_.front();
		}

		data_.resize(width, defaultValue_);

		Type* after = 0;
		if (!data_.empty())
		{
			after = &data_.front();
		}

		return (before != after);
	}

	template <typename Key>
	template <typename Type>
	std::auto_ptr<
		typename ParallelArray<Key>::UnknownArray>
		ParallelArray<Key>::KnownArray<Type>::clone() const
	{
		return std::auto_ptr<UnknownArray>(
			new KnownArray<Type>(*this));
	}

}

#endif
