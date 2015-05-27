// Description: Point concept

#ifndef PASTELSYS_POINT_CONCEPT_H
#define PASTELSYS_POINT_CONCEPT_H

#include "pastel/sys/locator/location.h"
#include "pastel/sys/type_traits/or.h"
#include "pastel/sys/type_traits/is_template_instance.h"

namespace Pastel
{

	// The default locator can be specified for a new
	// point-type Point by specializing Default_Locator 
	// for const Point&. The specialization must be done
	// in the Pastel namespace. This is similar to how
	// extending std::hash works. The Constraint parameter
	// can be used for SFINAE constraints.
	template <
		typename Point,
		typename Constraint = void>
	class Default_Locator;

	template <
		typename Point,
		typename Locator>
	class Location;

}

namespace Pastel
{

	template <typename Type>
	struct HasDefaultLocator
	{
	private:
		template <
			typename T,
			typename = typename Default_Locator<const T&>::Locator>
		struct Test 
		{
			// The class needs to be defined here ({};); otherwise 
			// it triggers a bug in Visual Studio 2015 RC.
		};

	public:
		static constexpr bool value =
			Compiles<Test, Type>::value;
	};

}

namespace Pastel
{

	struct Point_Concept
	{
		// A Point is either an arbitrary type with a 
		// default locator, or a Location, which pairs a 
		// point with a locator.
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::holds<
					Or<
						HasDefaultLocator<Type>,
						IsTemplateInstance<Type, Location>					
					>
				>()
			)

			// The following does not work, although I
			// cannot see why. Some possibilities: 
			// 1) This could invoke the most vexing parse,
			// where the argument is interpreted as
			// a function type.
			// 2) Visual Studio 2015 CTP6 has a bug.

			/*
			conceptCheck(
				RequiresSome<
					HasDefaultLocator<Type>,
					IsTemplateInstance<Type, Location>
				>()
			)
			*/
		);
	};

}

#include "pastel/sys/point/point_locator.h"
#include "pastel/sys/point/point_axis.h"
#include "pastel/sys/point/point_dimension.h"
#include "pastel/sys/point/point_point.h"
#include "pastel/sys/point/point_real.h"

#endif
