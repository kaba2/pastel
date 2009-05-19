#ifndef PASTEL_AGGREGATE_HPP
#define PASTEL_AGGREGATE_HPP

#include "pastel/sys/aggregate.h"
#include "pastel/sys/ensure.h"

#include <map>

namespace Pastel
{

	template <typename Key, typename Type>
	Aggregate<Key, Type>::Aggregate()
		: data_()
	{
	}

	template <typename Key, typename Type>
	Aggregate<Key, Type>::~Aggregate()
	{
		clear();
	}

	template <typename Key, typename Type>
	void Aggregate<Key, Type>::clear()
	{
		Iterator iter(data_.begin());
		Iterator iterEnd(data_.end());
		while (iter != iterEnd)
		{
			delete iter->second;
			++iter;
		}

		data_.clear();
	}

	template <typename Key, typename Type>
	void Aggregate<Key, Type>::swap(Aggregate<Key, Type>& that)
	{
		data_.swap(that.data_);
	}

	template <typename Key, typename Type>
	typename Aggregate<Key, Type>::ConstIterator
		Aggregate<Key, Type>::begin() const
	{
		return data_.begin();
	}

	template <typename Key, typename Type>
	typename Aggregate<Key, Type>::ConstIterator
		Aggregate<Key, Type>::end() const
	{
		return data_.end();
	}

	template <typename Key, typename Type>
	void Aggregate<Key, Type>::add(const Key& key,
		std::auto_ptr<Type>& that)
	{
		bool result = data_.insert(
			std::make_pair(key, that.release())).second;

		ENSURE(result);

		if (result)
		{
			that.release();
		}
	}

	template <typename Key, typename Type>
	std::auto_ptr<Type> Aggregate<Key, Type>::remove(const Key& key)
	{
		Iterator iter(data_.find(key));
		if (iter != data_.end())
		{
			std::auto_ptr<Type> result(iter->second);
			data_.erase(key);
			return result;
		}

		return std::auto_ptr<Type>();
	}

	template <typename Key, typename Type>
	Type* Aggregate<Key, Type>::get(const Key& key) const
	{
		ConstIterator iter(data_.find(key));
		if (iter != data_.end())
		{
			return iter->second;
		}

		return 0;
	}

}

#endif
