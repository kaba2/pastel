#include "pastel/sys/subset.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	Subset::Subset()
		: data_()
		, elements_()
	{
	}

	Subset::Subset(integer size, integer elements)
		: data_()
		, elements_(elements)
	{
		ENSURE_OP(size, >=, 0);
		ENSURE_OP(elements, >=, 0);
		ENSURE_OP(size, <=, elements);

		setSize(size);
	}

	void Subset::swap(Subset& that)
	{
		data_.swap(that.data_);
		std::swap(elements_, that.elements_);
	}

	void Subset::first()
	{
		integer size = data_.size();
		for (integer i = 0;i < size;++i)
		{
			data_[i] = i;
		}
	}

	bool Subset::next()
	{
		const integer size = data_.size();

		if (size == 0)
		{
			return false;
		}

		if (data_[size - 1] + 1 < elements_)
		{
			++data_[size - 1];
		}
		else
		{
			integer j = size - 2;
			while(j >= 0)
			{
				if (data_[j] + 1 < data_[j + 1])
				{
					break;
				}

				--j;
			}

			if (j == -1)
			{
				// This is the last data_.
				return false;
			}

			integer value = data_[j] + 1;
			while(j < size)
			{
				data_[j] = value;
				++value;
				++j;
			}
		}

		return true;
	}

	void Subset::setSize(integer size)
	{
		ENSURE_OP(size, <=, elements_);

		data_.clear();
		data_.reserve(size);

		for (integer i = 0;i < size;++i)
		{
			data_.push_back(i);
		}
	}

	void Subset::setSize(integer size, integer elements)
	{
		ENSURE_OP(size, >=, 0);
		ENSURE_OP(elements, >=, 0);
		ENSURE_OP(size, <=, elements);

		elements_ = elements;
		setSize(size);
	}

	integer Subset::size() const
	{
		return data_.size();
	}

	integer Subset::elements() const
	{
		return elements_;
	}

	integer Subset::operator[](integer index) const
	{

		PENSURE2(index >= 0 && index < size(), index, size());
		return data_[index];
	}


}
