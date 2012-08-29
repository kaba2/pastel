// Description: PossiblyEmptyMember class
// Detail: Allows to take advantage of the empty-member optimization.
// Documentation: miscellaneous.txt

#ifndef PASTELSYS_POSSIBLYEMPTYMEMBER_H
#define PASTELSYS_POSSIBLYEMPTYMEMBER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

namespace Pastel
{

	template <typename Type>
	class PossiblyEmptyMember
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		PossiblyEmptyMember()
			: data_()
		{
		}

		Type* data()
		{
			return (Type*)data_;
		}

		const Type* data() const
		{
			return (Type*)data_;
		}

	private:
		char data_[sizeof(Type)];
	};

	template <>
	class PASTELSYS PossiblyEmptyMember<EmptyClass>
	{
	public:
		EmptyClass* data()
		{
			return 0;
		}

		const EmptyClass* data() const
		{
			return 0;
		}
	};

}

#endif
