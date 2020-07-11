// Description: Concept-checking

#ifndef PASTELSYS_CONCEPT_H
#define PASTELSYS_CONCEPT_H

#include <type_traits>
#include <concepts>

#define PASTEL_CONCEPT_CHECK(Type, Concept) \
	static_assert(Concept##_<Type>, #Type " is not a model of " #Concept ".")

#define PASTEL_CONCEPT_CHECK1(Type, Concept, A) \
	static_assert(Concept##_<Type, A>, #Type " is not a model of " #Concept ".")

#define PASTEL_CONCEPT_CHECK2(Type, Concept, A, B) \
	static_assert(Concept##_<Type, A, B>, #Type " is not a model of " #Concept ".")

#define PASTEL_CONCEPT_REJECT(Type, Concept) \
	static_assert(!Concept##_<Type>, #Type " is a model of " #Concept ", although it should not be.")

#define PASTEL_CONCEPT_REJECT1(Type, Concept, A) \
	static_assert(!Concept##_<Type, A>, #Type " is a model of " #Concept ", although it should not be.")

#define PASTEL_CONCEPT_REJECT2(Type, Concept, A, B) \
	static_assert(!Concept##_<Type, A, B>, #Type " is a model of " #Concept ", although it should not be.")

namespace Pastel
{

	template <typename T>
	concept Trivial_Concept_ = std::same_as<T, T>;

}

#endif
