#ifndef PASTELSYS_KEYVALUE_H
#define PASTELSYS_KEYVALUE_H

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Key, typename Value>
	class KeyValue
		: boost::less_than_comparable<
		KeyValue<Key, Value>
		, boost::equality_comparable<
		KeyValue<Key, Value>
		> >
	{
	public:
		KeyValue()
			: key_()
			, value_()
		{
		}

		KeyValue(
			const Key& key,
			const Value& value)
			: key_(key)
			, value_(value)
		{
		}

		bool operator<(const KeyValue& that) const
		{
			if (key_ < that.key_)
			{
				return true;
			}
			if (key_ > that.key_)
			{
				return false;
			}
			return value_ < that.value_;
		}

		bool operator==(const KeyValue& that) const
		{
			return key_ == that.key_ && value_ == that.value_;
		}

		const Key& key() const
		{
			return key_;
		}

		Value& value()
		{
			return value_;
		}

		const Value& value() const
		{
			return value_;
		}

	private:
		Key key_;
		Value value_;
	};

	template <typename Key, typename Value>
	KeyValue<Key, Value> keyValue(const Key& key, const Value& value)
	{
		return KeyValue<Key, Value>(key, value);
	}

}

#endif
