#include "pastel/sys/ensure.h"

namespace Pastel
{

	namespace
	{

		InvariantFailureAction currentInvariantFailureAction = 
			InvariantFailureAction::AssertAndAbort;

	}

	void setInvariantFailureAction(
		InvariantFailureAction action)
	{
		currentInvariantFailureAction = action;
	}

	InvariantFailureAction invariantFailureAction()
	{
		return currentInvariantFailureAction;
	}

}
