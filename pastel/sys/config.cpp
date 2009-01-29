#include "pastel/sys/config.h"

namespace Pastel
{

	Config& Config::operator=(const Config& that)
	{
		Config copy(that);
		swap(copy);
		return *this;
	}

	Config& Config::operator+=(const Config& that)
	{
		ConstIterator iter = that.begin();
		const ConstIterator iterEnd = that.end();

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

	void Config::swap(Config& that)
	{
		data_.swap(that.data_);
	}

	void Config::clear()
	{
		data_.clear();
	}

	Config::ConstIterator Config::begin() const
	{
		return data_.begin();
	}

	Config::ConstIterator Config::end() const
	{
		return data_.end();
	}

	bool Config::created(const std::string& key) const
	{
		return data_.find(key) != data_.end();
	}

	integer Config::properties() const
	{
		return data_.size();
	}

	void Config::erase(const std::string& key)
	{
		data_.erase(key);
	}

}
