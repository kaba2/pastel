// Description: Testing for norms
// DocumentationOf: norm.h

#include "test/test_init.h"
#include "pastel/math/norm.h"

TEST_CASE("Norm (Euclidean)")
{
	using Product = Product_Norm<dreal, Maximum_Norm<dreal>, Euclidean_Norm<dreal>>;
	PASTEL_CONCEPT_CHECK(Product, Norm_Concept);
	PASTEL_CONCEPT_CHECK(Minkowski_Norm<dreal>, Norm_Concept);
	PASTEL_CONCEPT_CHECK(Maximum_Norm<dreal>, Norm_Concept);
	PASTEL_CONCEPT_CHECK(Manhattan_Norm<dreal>, Norm_Concept);
	PASTEL_CONCEPT_CHECK(Euclidean_Norm<dreal>, Norm_Concept);
	PASTEL_CONCEPT_REJECT(Euclidean_Distance<dreal>, Norm_Concept);
}
