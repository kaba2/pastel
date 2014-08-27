#ifndef PASTELSYS_CONFIG_HPP
#define PASTELSYS_CONFIG_HPP

#include "pastel/sys/config.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	inline Config& Config::operator=(const Config& that)
	{
		Config copy(that);
		swap(copy);
		return *this;
	}

	inline Config& Config::operator+=(const Config& that)
	{
		ConstIterator iter = that.begin();
		ConstIterator iterEnd = that.end();

		while(iter != iterEnd)
		{
			if (created(iter->first))
			{
				erase(iter->first);
			}

			data_.insert(*iter);

			++iter;
		}

		return *this;
	}

	inline void Config::swap(Config& that)
	{
		data_.swap(that.data_);
	}

	inline void Config::clear()
	{
		data_.clear();
	}

	inline Config::ConstIterator Config::begin() const
	{
		return data_.begin();
	}

	inline Config::ConstIterator Config::end() const
	{
		return data_.end();
	}

	inline bool Config::created(const std::string& key) const
	{
		return data_.find(key) != data_.end();
	}

	inline integer Config::properties() const
	{
		return data_.size();
	}

	inline void Config::erase(const std::string& key)
	{
		data_.erase(key);
	}

	template <typename Type>
	bool Config::ofType(const ConstIterator& iter) const
	{
		return boost::any_cast<std::vector<Type> >(&iter->second) != 0;
	}

	template <typename Type>
	void Config::assign(
		const std::string& key,
		const PASTEL_NO_DEDUCTION(Type)& value)
	{
		ConstIterator iter = data_.find(key);

		if (iter == data_.end())
		{
			iter = data_.insert(std::make_pair(key, boost::any(std::vector<Type>()))).first;
		}

		std::vector<Type>& valueList = writePropertyList<Type>(iter);
		if (!valueList.empty())
		{
			valueList.clear();
		}

		valueList.push_back(value);
	}

	template <typename Type>
	void Config::insert(
		const std::string& key,
		const PASTEL_NO_DEDUCTION(Type)& value)
	{
		ConstIterator iter = data_.find(key);

		if (iter == data_.end())
		{
			iter = data_.insert(std::make_pair(key, 
				boost::any(std::vector<Type>()))).first;
		}

		std::vector<Type>& valueList = writePropertyList<Type>(iter);
		valueList.push_back(value);
	}

	template <typename Type>
	const Type& Config::property(
		const std::string& key,
		const PASTEL_NO_DEDUCTION(Type)& defaultValue) const
	{
		return property<Type>(
			data_.find(key), defaultValue);
	}

	template <typename Type>
	const Type& Config::property(
		const ConstIterator& iter,
		const PASTEL_NO_DEDUCTION(Type)& defaultValue) const
	{
		if (iter == data_.end())
		{
			return defaultValue;
		}

		return propertyList<Type>(iter).front();
	}

	template <typename Type>
	const std::vector<Type>& Config::propertyList(
		const std::string& key) const
	{
		ConstIterator iter = data_.find(key);
		bool propertyExists = (iter != data_.end());

		ENSURE(propertyExists);

		return propertyList<Type>(iter);
	}

	template <typename Type>

	const std::vector<Type>& Config::propertyList(
		const ConstIterator& iter) const
	{
		const std::vector<Type>* correctType =
			boost::any_cast<std::vector<Type> >(&iter->second);
		ENSURE(correctType);

		return *correctType;
	}

	// Private

	template <typename Type>
	std::vector<Type>& Config::writePropertyList(
		const std::string& key)
	{
		return (std::vector<Type>&)((const Config*)this)->propertyList<Type>(key);
	}

	template <typename Type>
	std::vector<Type>& Config::writePropertyList(
		const ConstIterator& iter)
	{
		return (std::vector<Type>&)((const Config*)this)->propertyList<Type>(iter);
	}

}

#endif
