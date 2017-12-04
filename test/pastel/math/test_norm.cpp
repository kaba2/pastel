// Description: Testing for norms
// DocumentationOf: norm.h

#include "test/test_init.h"
#include "pastel/math/norm.h"

TEST_CASE("Norm (Euclidean)")
{
	PASTEL_CONCEPT_CHECK(Euclidean_Norm<real>, Norm_Concept);
}
