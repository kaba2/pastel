// Description: Composite thing
// Documentation: things.txt

#ifndef PASTELRAY_COMPOSITE_THING_H
#define PASTELRAY_COMPOSITE_THING_H

#include "pastel/ray/thing.h"

namespace Pastel
{

	class PASTELRAY Composite_Thing
		: public Thing
	{
	public:
		virtual ~Composite_Thing();
	};

}

#endif
