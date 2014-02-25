// Description: SFINAE machinery
// Documentation: environment.txt

#ifndef PASTELSYS_SFINAE_MACROS_H
#define PASTELSYS_SFINAE_MACROS_H

#include <boost/utility/enable_if.hpp>

#define PASTEL_ENABLE_IF(Condition, ReturnType) \
	typename boost::enable_if< \
	PASTEL_REMOVE_BRACKETS(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type

#define PASTEL_ENABLE_IF_P(Condition) \
	PASTEL_ENABLE_IF(Condition, MyTypes_::Enabler) = MyTypes_::Enabler()

#define PASTEL_ENABLE_IF_C(Condition, ReturnType) \
	typename boost::enable_if_c< \
	(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_ENABLE_IF_C_P(Condition) \
	PASTEL_ENABLE_IF_C(Condition, MyTypes_::Enabler) = MyTypes_::Enabler()

#define PASTEL_DISABLE_IF(Condition, ReturnType) \
	typename boost::disable_if< \
	PASTEL_REMOVE_BRACKETS(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_DISABLE_IF_P(Condition) \
	PASTEL_DISABLE_IF(Condition, MyTypes_::Enabler) = MyTypes_::Enabler()

#define PASTEL_DISABLE_IF_C(Condition, ReturnType) \
	typename boost::disable_if_c< \
	(Condition), \
	PASTEL_REMOVE_BRACKETS(ReturnType)>::type 

#define PASTEL_DISABLE_IF_C_P(Condition) \
	PASTEL_DISABLE_IF_C(Condition, MyTypes_::Enabler) = MyTypes_::Enabler()

#endif
