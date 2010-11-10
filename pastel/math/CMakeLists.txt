# This is a CMake build file for PastelMath.

cmake_minimum_required(VERSION 2.8)

source_group(
"Curve interpolation"
FILES
crspline.h
crspline.hpp
hermite.h
hermite.hpp
interpolation.h
interpolation.txt
kbspline.h
kbspline.hpp
piecewiselinear.h
piecewiselinear.hpp
smoothstep.h
smoothstep.hpp
)

source_group(
"Transformations"
FILES
transformations.txt
)

source_group(
"Transformations\\Affine transformation"
FILES
affinetransformation.h
affinetransformation.hpp
affinetransformation.txt
affinetransformation_tools.h
affinetransformation_tools.hpp
)

source_group(
"Transformations\\Quaternion"
FILES
quaternion.h
quaternion.hpp
quaternion.txt
)

source_group(
"Transformations\\AffineBijection"
FILES
affinebijection.h
affinebijection.hpp
affinebijection.txt
)

source_group(
"Transformations\\Linear algebra"
FILES
linear_algebra.txt
)

source_group(
"Transformations\\Linear algebra\\Matrix"
FILES
matrix.h
matrix.hpp
matrix.txt
matrix_dynamic.h
matrix_static.h
matrixexpression.h
matrixview.h
)

source_group(
"Transformations\\Linear algebra\\Matrix decompositions"
FILES
matrix_decompositions.txt
)

source_group(
"Transformations\\Linear algebra\\Matrix decompositions\\Cholesky decomposition"
FILES
cholesky_decomposition.h
cholesky_decomposition.hpp
cholesky_decomposition.txt
cholesky_decomposition_tools.h
cholesky_decomposition_tools.hpp
)

source_group(
"Transformations\\Linear algebra\\Matrix decompositions\\Lu decomposition"
FILES
ludecomposition.h
ludecomposition.hpp
ludecomposition.txt
ludecomposition_tools.h
ludecomposition_tools.hpp
)

source_group(
"Transformations\\Linear algebra\\Matrix decompositions\\Qr decomposition"
FILES
qr_decomposition.h
qr_decomposition.hpp
qr_decomposition.txt
qr_decomposition_tools.h
qr_decomposition_tools.hpp
)

source_group(
"Transformations\\Linear algebra\\Matrix tools"
FILES
matrix_tools.h
matrix_tools.hpp
matrix_tools_more.h
matrix_tools_more.hpp
matrix_tools_more2.h
matrix_tools_more2.hpp
matrix_tools_more3.h
matrix_tools_more3.hpp
matrix_tools_more4.h
matrix_tools_more4.hpp
)

source_group(
"Transformations\\ConformalAffine2D"
FILES
conformalaffine2d.h
conformalaffine2d.hpp
conformalaffine2d.txt
conformalaffine2d_tools.h
conformalaffine2d_tools.hpp
)

source_group(
"Common"
FILES
common.txt
mathlibrary.cpp
mathlibrary.h
orthonormal.h
orthonormal.hpp
statistics.h
statistics.hpp
)

source_group(
"Uniform distortion"
FILES
uniform_distortion.h
uniform_distortion.hpp
uniform_distortion.txt
)

source_group(
"Numbers"
FILES
integer_tools.h
integer_tools.hpp
minmax.h
minmax.hpp
numbers.txt
)

source_group(
"Numbers\\BigInteger"
FILES
biginteger.cpp
biginteger.h
biginteger.txt
biginteger_more.h
biginteger_more.hpp
biginteger_tools.cpp
biginteger_tools.h
)

source_group(
"Numbers\\Rational"
FILES
rational.h
rational.hpp
rational.txt
rational_more.h
rational_more.hpp
rational_tools.h
rational_tools.hpp
)

source_group(
"Numbers\\Interval"
FILES
interval.h
interval.hpp
interval.txt
interval_tools.h
interval_tools.hpp
)

source_group(
"Polynomials"
FILES
polynomial.h
polynomial.hpp
polynomial.txt
polynomial_tools.h
polynomial_tools.hpp
)

source_group(
"Real conversions"
FILES
coordinates.h
coordinates.hpp
coordinates.txt
coordinates_derivatives.h
coordinates_derivatives.hpp
coordinates_derivatives.txt
)

source_group(
"Norm bijections"
FILES
euclidean_normbijection.h
manhattan_normbijection.h
maximum_normbijection.h
minkowski_normbijection.h
normbijection.h
normbijection.txt
normbijection_concept.h
)

source_group(
"Low discrepancy sequences"
FILES
lowdiscrepancy.h
lowdiscrepancy.hpp
lowdiscrepancy.txt
)

source_group(
"Uniform sampling"
FILES
uniform_sampling.h
uniform_sampling.hpp
uniform_sampling.txt
)

set(PastelMath_Files
affinebijection.h
affinebijection.hpp
affinebijection.txt
affinetransformation.h
affinetransformation.hpp
affinetransformation.txt
affinetransformation_tools.h
affinetransformation_tools.hpp
biginteger.cpp
biginteger.h
biginteger.txt
biginteger_more.h
biginteger_more.hpp
biginteger_tools.cpp
biginteger_tools.h
cholesky_decomposition.h
cholesky_decomposition.hpp
cholesky_decomposition.txt
cholesky_decomposition_tools.h
cholesky_decomposition_tools.hpp
common.txt
conformalaffine2d.h
conformalaffine2d.hpp
conformalaffine2d.txt
conformalaffine2d_tools.h
conformalaffine2d_tools.hpp
coordinates.h
coordinates.hpp
coordinates.txt
coordinates_derivatives.h
coordinates_derivatives.hpp
coordinates_derivatives.txt
crspline.h
crspline.hpp
euclidean_normbijection.h
hermite.h
hermite.hpp
integer_tools.h
integer_tools.hpp
interpolation.h
interpolation.txt
interval.h
interval.hpp
interval.txt
interval_tools.h
interval_tools.hpp
kbspline.h
kbspline.hpp
linear_algebra.txt
lowdiscrepancy.h
lowdiscrepancy.hpp
lowdiscrepancy.txt
ludecomposition.h
ludecomposition.hpp
ludecomposition.txt
ludecomposition_tools.h
ludecomposition_tools.hpp
manhattan_normbijection.h
mathlibrary.cpp
mathlibrary.h
matrix.h
matrix.hpp
matrix.txt
matrix_decompositions.txt
matrix_dynamic.h
matrix_static.h
matrix_tools.h
matrix_tools.hpp
matrix_tools_more.h
matrix_tools_more.hpp
matrix_tools_more2.h
matrix_tools_more2.hpp
matrix_tools_more3.h
matrix_tools_more3.hpp
matrix_tools_more4.h
matrix_tools_more4.hpp
matrixexpression.h
matrixview.h
maximum_normbijection.h
minkowski_normbijection.h
minmax.h
minmax.hpp
normbijection.h
normbijection.txt
normbijection_concept.h
numbers.txt
orthonormal.h
orthonormal.hpp
pastelmath.txt
piecewiselinear.h
piecewiselinear.hpp
polynomial.h
polynomial.hpp
polynomial.txt
polynomial_tools.h
polynomial_tools.hpp
qr_decomposition.h
qr_decomposition.hpp
qr_decomposition.txt
qr_decomposition_tools.h
qr_decomposition_tools.hpp
quaternion.h
quaternion.hpp
quaternion.txt
rational.h
rational.hpp
rational.txt
rational_more.h
rational_more.hpp
rational_tools.h
rational_tools.hpp
smoothstep.h
smoothstep.hpp
statistics.h
statistics.hpp
transformations.txt
uniform_distortion.h
uniform_distortion.hpp
uniform_distortion.txt
uniform_sampling.h
uniform_sampling.hpp
uniform_sampling.txt
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_library(PastelMath ${PastelMath_Files})
