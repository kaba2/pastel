#ifndef PASTEL_VECTOR_CONCEPT_HPP
#define PASTEL_VECTOR_CONCEPT_HPP

#include "pastel/sys/vector_concept.h"

#include <boost/type_traits/is_arithmetic.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/apply.hpp>

namespace Pastel
{

	namespace Detail_Vector
	{

		template <typename Type>
		struct Scalar_Class
		{
			typedef typename Type::Scalar type;
		};

		template <typename Type>
		struct Scalar_Builtin
		{
			typedef Type type;
		};

	}

	template <typename Type>
	struct Vector_Scalar
		: boost::mpl::eval_if<
		boost::is_arithmetic<Type>,
		Detail_Vector::Scalar_Builtin<Type>,
		Detail_Vector::Scalar_Class<Type> >
	{
	};

}

#endif
