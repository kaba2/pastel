#ifndef PASTEL_REFINABLE_PARTITION_ELEMENT_H
#define PASTEL_REFINABLE_PARTITION_ELEMENT_H

#include "pastel/sys/refinable_partition.h"
#include "pastel/sys/possiblyemptymember.h"

namespace Pastel
{

	template <typename Type>
	class RefinablePartition_Fwd<Type>::Element
		: public PossiblyEmptyMember<Type>
	{
	public:
		typedef PossiblyEmptyMember<Type> Data;

		Element(const Element& that)
			: block_(that.block_)
			, position_(that.position_)
		{
			if (Data::data())
			{
				new(Data::data()) Type(that.data());
			}
		}

		Element(Element&& that)
			: block_()
			, position_()
		{
			if (Data::data())
			{
				new(Data::data()) Type;
			}

			swap(that);
		}

		~Element()
		{
			if (Data::data())
			{
				Data::data()->~Type();
			}
		}

		Element& operator=(Element that)
		{
			swap(that);
			return *this;
		}

		Type& data()
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		const Type& data() const
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		Block_ConstIterator block() const
		{
			return block_;
		}

		Partition_ConstIterator position() const
		{
			return position_;
		}

	private:
		template <typename Type>
		friend class RefinablePartition;

		Element()
			: block_()
			, position_()
		{
			if (Data::data())
			{
				new(Data::data()) Type;
			}
		}

		Element(
			Block_Iterator block,
			Partition_Iterator position,
			Type data)
			: block_(block)
			, position_(position)
		{
			if (Data::data())
			{
				new(Data::data()) Type(std::move(data));
			}
		}

		void swap(Element& that)
		{
			using std::swap;
			swap(block_, that.block_);
			swap(position_, that.position_);
			if (Data::data())
			{
				swap(data(), that.data());
			}
		}

		Block_Iterator block_;
		Partition_Iterator position_;
	};

}

#endif
