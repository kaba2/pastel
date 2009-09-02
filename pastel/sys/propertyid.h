// Documentation: data_structures.txt

#ifndef PASTEL_PROPERTYID_H
#define PASTEL_PROPERTYID_H

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Key, typename Type>
	class PropertyId
		: boost::less_than_comparable<
		PropertyId<Key, Type>
		, boost::equality_comparable<
		PropertyId<Key, Type>
		> >
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		PropertyId()
			: id_()
		{
		}

		explicit PropertyId(const Key& id)
			: id_(id)
		{
		}

		void swap(PropertyId& that)
		{
			std::swap(id_, that.id_);
		}

		/*
		void setId(const Key& id)
		{
			id_ = id;
		}
		*/

		const Key& id() const
		{
			return id_;
		}

		bool operator<(const PropertyId& that) const
		{
			return id_ < that.id_;
		}

		bool operator==(const PropertyId& that) const
		{
			return id_ == that.id_;
		}

	private:
		Key id_;
	};

}

#endif
