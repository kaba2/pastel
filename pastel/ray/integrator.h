#ifndef PASTEL_INTEGRATOR_H
#define PASTEL_INTEGRATOR_H

#include "pastel/ray/spectrum.h"

#include "pastel/sys/countedptr.h"

namespace Pastel
{

	class Integrator
		: public ReferenceCounted
	{
	public:
		virtual ~Integrator()
		{
		}

		virtual Spectrum work(
			const ScenePtr& scene)
		{
		}

	};

	typedef CountedPtr<Integrator> IntegratorPtr;
	typedef CountedPtr<const Integrator> ConstIntegratorPtr;

}

#endif
