// Description: Default cloning behaviour for ClonedPtr

#ifndef PASTELSYS_CLONE_PTR_PRIVATE_H
#define PASTELSYS_CLONE_PTR_PRIVATE_H

#include "pastel/sys/clone_ptr.h"

#include <type_traits>

#include <boost/mpl/if.hpp>

namespace Pastel
{

	namespace ClonePtr_
	{

		template<class Type>
		struct is_cloneable
		{
		private:
			typedef char (& Yes)[1];
			typedef char (& No)[2];

			template<class U, U* (U::*)() const = &U::clone> 
			struct cloneable {};

			template<class U> static Yes test( cloneable<U>* );
			template<class U> static No test(...);

		public:
			static bool const value = 
				(sizeof(test<Type>(0)) == sizeof(Yes));
		};

		class Copy_Cloner
		{
		public:
			template<class Type>
			Type* operator()(const Type& that) const 
			{ 
				return new Type(that); 
			}
		};

		class Clone_Cloner
		{
		public:
			template<class Type>
			Type* operator()(const Type& that) const 
			{ 
				return that.clone(); 
			}
		};

		template <typename Type>
		class Default_Cloner
		{
		public:
			typedef typename boost::mpl::if_c<
				std::is_polymorphic<Type>::value &&
				is_cloneable<Type>::value,
				Clone_Cloner, Copy_Cloner>::type type;
		};

	}

}

#endif
