// Description: SFINAE machinery
// Documentation: environment.txt

#ifndef PASTELSYS_SFINAE_MACROS_H
#define PASTELSYS_SFINAE_MACROS_H

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	template <
		typename Condition,
		typename Return>
	using EnableIf = 
		typename boost::enable_if<Condition, Return>::type;

	template <
		bool Condition,
		typename Return>
	using EnableIfC = 
		typename boost::enable_if_c<Condition, Return>::type;

	template <
		typename Condition,
		typename Return>
	using DisableIf = 
		typename boost::disable_if<Condition, Return>::type;

	template <
		bool Condition,
		typename Return>
	using DisableIfC = 
		typename boost::disable_if_c<Condition, Return>::type;

}


#endif
