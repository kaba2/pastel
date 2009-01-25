#ifndef PASTELSYS_FASTLIST_HPP
#define PASTELSYS_FASTLIST_HPP

#include "pastel/sys/fastlist.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/functional.h"

#include <boost/mpl/if.hpp>
#include <boost/operators.hpp>
#include <boost/type_traits.hpp>

#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <iterator>
#include <functional>

namespace Pastel
{

	template <
		typename Type,
		typename UniformAllocator>
	class FastList<Type, UniformAllocator>::iterator
		: boost::less_than_comparable<
		typename FastList<Type, UniformAllocator>::iterator
		, boost::equality_comparable<
		typename FastList<Type, UniformAllocator>::iterator
		> >
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef Type value_type;
		typedef ptrdiff_t difference_type;
		typedef Type* pointer;
		typedef Type& reference;

		friend class FastList;
		friend class FastList::const_iterator;
		friend struct hash<typename FastList<Type, UniformAllocator>::iterator>;

		// Using default copy constructor
		// Using default destructor
		// Using default assignment

		iterator()
			: node_(0)
		{
		}

		void swap(iterator& that)
		{
			std::swap(node_, that.node_);
		}

		pointer operator->() const
		{
			ASSERT(node_);
			DataNode* dataNode = (DataNode*)node_;
			return &dataNode->data_;
		}

		reference operator*() const
		{
			ASSERT(node_);
			DataNode* dataNode = (DataNode*)node_;
			return dataNode->data_;
		}

		iterator& operator++()
		{
			ASSERT(node_);
			node_ = node_->next_;
			return *this;
		}

		iterator& operator--()
		{
			ASSERT(node_);
			node_ = node_->previous_;
			return *this;
		}

		iterator operator++(int)
		{
			iterator result(*this);
			++(*this);
			return result;
		}

		iterator operator--(int)
		{
			iterator result(*this);
			--(*this);
			return result;
		}

		bool operator==(const iterator& that) const
		{
			return node_ == that.node_;
		}

		bool operator<(const iterator& that) const
		{
			return node_ < that.node_;
		}

	private:
		explicit iterator(Node* node)
			: node_(node)
		{
		}

		Node* node_;
	};

	template <
		typename Type,
		typename UniformAllocator>
	class FastList<Type, UniformAllocator>::const_iterator
		: boost::less_than_comparable<
		typename FastList<Type, UniformAllocator>::const_iterator
		, boost::equality_comparable<
		typename FastList<Type, UniformAllocator>::const_iterator
		, boost::equality_comparable2<
		typename FastList<Type, UniformAllocator>::const_iterator,
		typename FastList<Type, UniformAllocator>::iterator
		, boost::less_than_comparable2<
		typename FastList<Type, UniformAllocator>::const_iterator,
		typename FastList<Type, UniformAllocator>::iterator
		> > > >
	{
	private:
		typedef typename FastList<Type, UniformAllocator>::iterator iterator;
	public:
		friend class FastList<Type, UniformAllocator>;
		friend struct hash<typename FastList<Type, UniformAllocator>::const_iterator>;

		typedef std::bidirectional_iterator_tag iterator_category;
		typedef const Type value_type;
		typedef ptrdiff_t difference_type;
		typedef const Type* pointer;
		typedef const Type& reference;

		// Using default copy constructor
		// Using default destructor
		// Using default assignment

		const_iterator()
			: node_(0)
		{
		}

		const_iterator(
			const iterator& that)
			: node_(that.node_)
		{
		}

		const_iterator& operator=(
			const iterator& that)
		{
			node_ = that.node_;
			return *this;
		}

		void swap(const_iterator& that)
		{
			std::swap(node_, that.node_);
		}

		const_pointer operator->() const
		{
			ASSERT(node_);
			const DataNode* dataNode = (const DataNode*)node_;
			return &dataNode->data_;
		}

		const_reference operator*() const
		{
			ASSERT(node_);
			const DataNode* dataNode = (const DataNode*)node_;
			return dataNode->data_;
		}

		const_iterator& operator++()
		{
			ASSERT(node_);
			node_ = node_->next_;
			return *this;
		}

		const_iterator& operator--()
		{
			ASSERT(node_);
			node_ = node_->previous_;
			return *this;
		}

		const_iterator operator++(int)
		{
			const_iterator result(*this);
			++(*this);
			return result;
		}

		const_iterator operator--(int)
		{
			const_iterator result(*this);
			--(*this);
			return result;
		}

		bool operator==(const const_iterator& that) const
		{
			return node_ == that.node_;
		}

		bool operator==(const iterator& that) const
		{
			return node_ == that.node_;
		}

		bool operator<(const const_iterator& that) const
		{
			return node_ < that.node_;
		}

		bool operator<(const iterator& that) const
		{
			return node_ < that.node_;
		}

	private:
		explicit const_iterator(const Node* node)
			: node_(node)
		{
		}

		const Node* node_;
	};

	// Construction

	template <
		typename Type,
		typename UniformAllocator>
		FastList<Type, UniformAllocator>::FastList(
		const allocator_ref& allocatorRef)
		: head_(0)
		, size_(0)
		, allocator_(
		allocatorRef.get() == 0 ?
		allocator_ref(new UniformAllocator(sizeof(DataNode))) : allocatorRef)
	{
		ENSURE2(allocator_->unitSize() >= sizeof(DataNode),
			allocator_->unitSize(), sizeof(DataNode));

		initialize();
	}

	template <
		typename Type,
		typename UniformAllocator>
		FastList<Type, UniformAllocator>::FastList(
		size_type count, const value_type& value,
		const allocator_ref& allocatorRef)
		: head_(0)
		, size_(0)
		, allocator_(
		allocatorRef.get() == 0 ?
		allocator_ref(new UniformAllocator(sizeof(DataNode))) : allocatorRef)
	{
		ENSURE1(count >= 0, count);
		ENSURE2(allocator_->unitSize() >= sizeof(DataNode),
			allocator_->unitSize(), sizeof(DataNode));

		initialize();

		insert(end(), count, value);
	}

	template <
		typename Type,
		typename UniformAllocator>
		template <typename InputIterator>
	FastList<Type, UniformAllocator>::FastList(
		InputIterator first,
		InputIterator last,
		const allocator_ref& allocatorRef)
		: head_(0)
		, size_(0)
		, allocator_(
		allocatorRef.get() == 0 ?
		allocator_ref(new UniformAllocator(sizeof(DataNode))) : allocatorRef)
	{
		ENSURE2(allocator_->unitSize() >= sizeof(DataNode),
			allocator_->unitSize(), sizeof(DataNode));

		initialize();

		insert(end(), first, last);
	}

	template <
		typename Type,
		typename UniformAllocator>
		FastList<Type, UniformAllocator>::FastList(
		const FastList& that,
		const allocator_ref& allocatorRef)
		: head_(0)
		, size_(0)
		, allocator_(
		allocatorRef.get() == 0 ?
		that.allocator_ : allocatorRef)
	{
		ENSURE2(allocator_->unitSize() >= sizeof(DataNode),
			allocator_->unitSize(), sizeof(DataNode));

		initialize();

		insert(end(), that.begin(), that.end());
	}

	template <
		typename Type,
		typename UniformAllocator>
		FastList<Type, UniformAllocator>::~FastList()
	{
		clear();
		//allocator_->deallocate(head_);
		delete head_;
	}

	template <
		typename Type,
		typename UniformAllocator>
		FastList<Type, UniformAllocator>&
		FastList<Type, UniformAllocator>::operator=(
		const FastList<Type, UniformAllocator> &that)
	{
		assign(that.begin(), that.end());

		return *this;
	}

	template <
		typename Type,
		typename UniformAllocator>
		bool FastList<Type, UniformAllocator>::operator==(
		const FastList& that) const
	{
		return (size() == that.size() &&
			std::equal(begin(), end(), that.begin()));
	}

	template <
		typename Type,
		typename UniformAllocator>
		bool FastList<Type, UniformAllocator>::operator<(
		const FastList& that) const
	{
		return std::lexicographical_compare(
			begin(), end(),
			that.begin(), that.end());
	}

	template <
		typename Type,
		typename UniformAllocator>
		template <typename InputIterator>
	void FastList<Type, UniformAllocator>::assign(
		InputIterator from,
		InputIterator to)
	{
		typedef typename boost::mpl::if_<
			boost::is_integral<InputIterator>,
			ValueTag,
			RangeTag>::type Tag;

		iterator first(
			insert(end(), from, to, Tag()));
		erase(begin(), first);
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::assign(
		size_type count, const value_type& value)
	{
		iterator first(
			insert(end(), count, value, ValueTag()));
		erase(begin(), first);
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::
		iterator
		FastList<Type, UniformAllocator>::begin()
	{
		return iterator(nodeBegin());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::
		const_iterator
		FastList<Type, UniformAllocator>::begin() const
	{
		return const_iterator(nodeBegin());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::
		iterator
		FastList<Type, UniformAllocator>::end()
	{
		return iterator(nodeEnd());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::
		const_iterator
		FastList<Type, UniformAllocator>::end() const
	{
		return const_iterator(nodeEnd());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::
		reverse_iterator
		FastList<Type, UniformAllocator>::rbegin()
	{
		return reverse_iterator(end());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::
		const_reverse_iterator
		FastList<Type, UniformAllocator>::rbegin() const
	{
		return const_reverse_iterator(end());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::
		reverse_iterator
		FastList<Type, UniformAllocator>::rend()
	{
		return reverse_iterator(begin());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::
		const_reverse_iterator
		FastList<Type, UniformAllocator>::rend() const
	{
		return const_reverse_iterator(begin());
	}

	template <
		typename Type,
		typename UniformAllocator>
		bool FastList<Type, UniformAllocator>::empty() const
	{
		return (begin() == end());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::size_type
		FastList<Type, UniformAllocator>::size() const
	{
		if (size_ == UnknownSize)
		{
			size_ = std::distance(begin(), end());
		}

		return size_;
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::size_type
		FastList<Type, UniformAllocator>::max_size() const
	{
		return std::numeric_limits<size_type>::max();
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::
		set_allocator(const allocator_ref& allocatorRef)
	{
		FastList copy(*this, allocatorRef);
		swap(copy);
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::allocator_ref
		FastList<Type, UniformAllocator>::get_allocator() const
	{
		return allocator_;
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::resize(
		size_type newSize, Type value)
	{
		ENSURE1(newSize >= 0, newSize);

		if (newSize < size_)
		{
			while (newSize < size_)
			{
				pop_back();
			}
		}
		else if (newSize > size_)
		{
			size_type originalSize = size_;
			try
			{
				while (newSize > size_)
				{
					push_back(value);
				}
			}
			catch(...)
			{
				while (size_ > originalSize)
				{
					pop_back();
				}
				throw;
			}
		}
	}

	template <typename Type, typename UniformAllocator>
	typename FastList<Type, UniformAllocator>::reference
		FastList<Type, UniformAllocator>::front()
	{
		ENSURE(!empty());
		DataNode* dataNode = (DataNode*)head_->next_;
		return dataNode->data_;
	}

	template <typename Type, typename UniformAllocator>
	typename FastList<Type, UniformAllocator>::const_reference
		FastList<Type, UniformAllocator>::front() const
	{
		ENSURE(!empty());
		const DataNode* dataNode = (const DataNode*)head_->next_;
		return dataNode->data_;
	}

	template <typename Type, typename UniformAllocator>
	typename FastList<Type, UniformAllocator>::reference
		FastList<Type, UniformAllocator>::back()
	{
		ENSURE(!empty());
		DataNode* dataNode = (DataNode*)head_->previous_;
		return dataNode->data_;
	}

	template <typename Type, typename UniformAllocator>
	typename FastList<Type, UniformAllocator>::const_reference
		FastList<Type, UniformAllocator>::back() const
	{
		ENSURE(!empty());
		const DataNode* dataNode = (const DataNode*)head_->previous_;
		return dataNode->data_;
	}

	template <typename Type, typename UniformAllocator>
	void FastList<Type, UniformAllocator>::push_front(
		const value_type& that)
	{
		insert(begin(), that);
	}

	template <typename Type, typename UniformAllocator>
	void FastList<Type, UniformAllocator>::pop_front()
	{
		erase(begin());
	}

	template <typename Type, typename UniformAllocator>
	void FastList<Type, UniformAllocator>::push_back(
		const value_type& that)
	{
		insert(end(), that);
	}

	template <typename Type, typename UniformAllocator>
	void FastList<Type, UniformAllocator>::pop_back()
	{
		erase(head_->previous_);
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::iterator
		FastList<Type, UniformAllocator>::insert(
		iterator there, const value_type& that)
	{
		return iterator(insert(there.node_, that));
	}

	template <typename Type, typename UniformAllocator>
	void FastList<Type, UniformAllocator>::insert(
		iterator there,
		size_type count,
		const value_type& that)
	{
		insert(there, count, that, ValueTag());
	}

	template <typename Type, typename UniformAllocator>
	template <typename InputIterator>
	void FastList<Type, UniformAllocator>::insert(
		iterator there,
		InputIterator from,
		InputIterator to)
	{
		typedef typename boost::mpl::if_<
			boost::is_integral<InputIterator>,
			ValueTag,
			RangeTag>::type Tag;

		insert(there, from, to, Tag());
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::iterator
		FastList<Type, UniformAllocator>::erase(
		iterator that)
	{
		ENSURE(that != end());

		return iterator(erase(that.node_));
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::iterator
		FastList<Type, UniformAllocator>::erase(
		iterator first,
		iterator last)
	{
		iterator current(first);
		iterator rangeEnd(last);

		while (current != rangeEnd)
		{
			current = erase(current);
		}

		return last;
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::swap(
		FastList<Type, UniformAllocator>& that)
	{
		std::swap(head_, that.head_);
		std::swap(size_, that.size_);
		std::swap(allocator_, that.allocator_);
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::clear()
	{
		iterator current(begin());
		iterator thisEnd(end());

		while (current != thisEnd)
		{
			current = erase(current);
		}

		size_ = 0;
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::remove(
		const value_type& that)
	{
		remove_if(std::bind2nd(
			equal_to<Type>(), that));
	}

	template <
		typename Type,
		typename UniformAllocator>
		template <typename Predicate>
	void FastList<Type, UniformAllocator>::remove_if (
		Predicate predicate)
	{
		iterator current(begin());
		iterator thisEnd(end());

		while (current != thisEnd)
		{
			if (predicate(*current))
			{
				current = erase(current);
			}
			else
			{
				++current;
			}
		}
	}

	template <
		typename Type,
		typename UniformAllocator>
	void FastList<Type, UniformAllocator>::unique()
	{
		unique(equal_to<Type>());
	}

	template <
		typename Type,
		typename UniformAllocator>
		template <typename Compare>
	void FastList<Type, UniformAllocator>::unique(
		Compare compare)
	{
		if (empty())
		{
			return;
		}

		iterator thisEnd(end());
		iterator current(begin());
		iterator next(current);
		++next;

		while (next != thisEnd)
		{
			bool equal = compare(*next, *current);
			if (equal)
			{
				// Next and current are equal,
				// remove next.

				next = erase(next);
			}
			else
			{
				current = next;
				++next;
			}
		}
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::splice(
		iterator position, FastList& that)
	{
		splice(position.node_, that, that.nodeBegin(), that.nodeEnd());
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::splice(
		iterator position, FastList& that,
		iterator i)
	{
		splice(position.node_, that, i.node_, i.node_->next_);
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::splice(
		iterator position, FastList& that,
		iterator first, iterator last)
	{
		splice(position.node_, that, first.node_, last.node_);
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::merge(
		FastList& that)
	{
		merge(that, std::less<Type>());
	}

	template <
		typename Type,
		typename UniformAllocator>
		template <typename Compare>
	void FastList<Type, UniformAllocator>::merge(
		FastList& that,
		Compare compare)
	{
		ENSURE(*allocator_ == *(that.allocator_));

		iterator thatCurrent(that.begin());
		iterator thatEnd(that.end());
		iterator thisCurrent(begin());
		iterator thisEnd(end());

		while (thatCurrent != thatEnd &&
			thisCurrent != thisEnd)
		{
			bool thatLessThis = compare(
				*thatCurrent,
				*thisCurrent);

			if (thatLessThis)
			{
				iterator thatNext(thatCurrent);
				++thatNext;
				splice(thisCurrent, that, thatCurrent, thatNext);
				thatCurrent = thatNext;
			}
			else
			{
				// That is,
				// this <= that

				// In case of equality,
				// the element in this list
				// comes first.

				++thisCurrent;
			}
		}

		// Insert rest of that.
		splice(end(), that, that.begin(), that.end());
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::sort()
	{
		sort(std::less<Type>());
	}

	template <
		typename Type,
		typename UniformAllocator>
		template <typename Compare>
	void FastList<Type, UniformAllocator>::sort(
		Compare compare)
	{
		// Shamelessly copied from Microsoft Stl implementation:/

		size_type mySize = size();

		if (mySize >= 2)
		{
			// worth sorting, do it
			const size_type MaxBins = 25;
			FastList tempList(get_allocator());
			std::vector<FastList> binList(MaxBins + 1);
			for (integer i = 0;i < MaxBins + 1;++i)
			{
				binList[i].set_allocator(get_allocator());
			}

			size_type maxBin = 0;

			while (!empty())
			{
				// sort another element, using bins
				// don't invalidate iterators
				tempList.splice(tempList.begin(), *this, begin(),
					++begin());

				size_type bin = 0;
				for (bin = 0; bin < maxBin && !binList[bin].empty();++bin)
				{
					// merge into ever larger bins
					binList[bin].merge(tempList, compare);
					binList[bin].swap(tempList);
				}

				// i e [0, maxBin[ => binList[i].empty()
				// =>
				// tempList contains all of the elements
				// thus far in an ordered list.

				if (bin == MaxBins)
				{
					binList[bin - 1].merge(tempList, compare);
				}
				else
				{   // spill to new bin, while they last
					binList[bin].swap(tempList);
					if (bin == maxBin)
					{
						++maxBin;
					}
				}
			}

			for (size_type bin = 1; bin < maxBin; ++bin)
			{
				// Merge up
				binList[bin].merge(binList[bin - 1], compare);
			}

			// Result in last bin
			splice(begin(), binList[maxBin - 1]);
		}
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::reverse()
	{
		Node* current(nodeBegin()->next_);
		while (current != nodeEnd());
		{
			Node* next = current->next_;
			linkNodes(current, nodeBegin());
			linkNodes(nodeEnd(), current);
			current = next;
		}
	}

	template <
		typename Type,
		typename UniformAllocator>
		void swap(
		const FastList<Type, UniformAllocator>& left,
		const FastList<Type, UniformAllocator>& right)
	{
		left.swap(right);
	}

}

#include "pastel/sys/fastlistprivate.hpp"

#endif
