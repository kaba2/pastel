// Description: Testing for norms
// DocumentationOf: norm.h

#include "test/test_init.h"
#include "pastel/math/norm.h"

TEST_CASE("Norm (Euclidean)")
{
	using Product = Product_Norm<real, Maximum_Norm<real>, Euclidean_Norm<real>>;
	PASTEL_CONCEPT_CHECK(Product, Norm_Concept);
	PASTEL_CONCEPT_CHECK(Minkowski_Norm<real>, Norm_Concept);
	PASTEL_CONCEPT_CHECK(Maximum_Norm<real>, Norm_Concept);
	PASTEL_CONCEPT_CHECK(Manhattan_Norm<real>, Norm_Concept);
	PASTEL_CONCEPT_CHECK(Euclidean_Norm<real>, Norm_Concept);
	PASTEL_CONCEPT_REJECT(Euclidean_Distance<real>, Norm_Concept);
}
