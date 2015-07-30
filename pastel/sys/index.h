#ifndef PASTELSYS_INDEX_H
#define PASTELSYS_INDEX_H

namespace Pastel
{

	template <typename Key>
	class Index
	{
	public:
		using KeyToIndex = 
			std::unordered_set<Key, integer>;
		using Key_ConstIterator = 
			typename KeyToIndex::iterator;
		using IndexToKey = 
			std::vector<Key_ConstIterator>;

		template <
			typename Iterator>
		void assign(
			const Iterator& begin, 
			const Iterator& end)
		{
			for (Iterator i = begin;i != end;++i)
			{
				insert(*i);
			}
		}

		void clear()
		{
			keyToIndex_.clear();
			indexToKey_.clear();
		}

		integer n() const
		{
			return indexToKey_.size();
		}

		void reserve(integer n)
		{
			ENSURE_OP(n, >=, 0);
			indexToKey_.reserve(n);
			keyToIndex_.reserve(n);
		}

		template <typename Type>
		Key_ConstIterator insert(Type&& key)
		{
			Key_ConstIterator i = 
				keyToIndex_.emplace(
					std::forward(key), 
					(integer)indexToKey_.size());
			try
			{
				indexToKey_.push_back(i);
			}
			catch(...)
			{
				keyToIndex_.erase(i);
				throw;
			};

			return i;
		}

	private:
		KeyToIndex keyToIndex_;
		IndexToKey indexToKey_;
	};
	
}

#endif
