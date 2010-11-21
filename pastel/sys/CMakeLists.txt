# This is a CMake build file for PastelSys.

cmake_minimum_required(VERSION 2.8)

source_group(
"Data structures"
FILES
data_structures.txt
)

source_group(
"Data structures\\Parallel array"
FILES
parallelarray.h
parallelarray.hpp
propertyid.h
)

source_group(
"Data structures\\DisjointGroups"
FILES
disjointgroups.h
disjointgroups.hpp
disjointgroups.txt
disjointgroups_design.txt
)

source_group(
"Data structures\\Array"
FILES
array.h
array.hpp
array.txt
arraybase.h
arraybase.hpp
arraybasecursor.h
subarray.h
subarray_iterator.h
subarray_tools.h
subarray_tools.hpp
)

source_group(
"Data structures\\FastList"
FILES
fastlist.h
fastlist.hpp
fastlist.txt
fastlist_tools.h
fastlist_tools.hpp
fastlistprivate.hpp
)

source_group(
"Data structures\\Unordered containers"
FILES
hash_tables.txt
)

source_group(
"Data structures\\Unordered containers\\UnorderedMap"
FILES
functional.h
functional.hpp
unorderedbase.h
unorderedbase.hpp
unorderedmap.h
unorderedmap.hpp
)

source_group(
"Data structures\\Unordered containers\\UnorderedSet"
FILES
unorderedset.h
unorderedset.hpp
)

source_group(
"Data structures\\Small containers"
FILES
small_containers.txt
)

source_group(
"Data structures\\Small containers\\SmallFixedSet"
FILES
smallfixedset.h
smallfixedset.hpp
)

source_group(
"Data structures\\Small containers\\SmallSet"
FILES
smallset.h
smallset.hpp
)

source_group(
"Data structures\\Small containers\\SmallMap"
FILES
smallmap.h
smallmap.hpp
)

source_group(
"Data structures\\View"
FILES
adaptedview.h
adaptedviewcursor.h
arrayview.h
cursorview.h
mirrorview.h
mirrorviewcursor.h
permutedview.h
permutedviewcursor.h
rowview.h
rowviewcursor.h
sliceview.h
sliceviewcursor.h
sparseview.h
sparseviewcursor.h
subview.h
transformedview.h
transformedviewcursor.h
view.h
view.txt
view_all.h
view_tools.h
view_tools.hpp
view_visit.h
view_visit.hpp
view_visit_more.h
view_visit_more.hpp
view_visit_more2.h
view_visit_more2.hpp
)

source_group(
"Data structures\\View\\Extended ConstView"
FILES
borderedconstview.h
clampedconstview.h
extendedconstview.h
extendedconstview_all.h
mirroredconstview.h
repeatedconstview.h
)

source_group(
"Data structures\\AnyStorage"
FILES
anystorage.h
)

source_group(
"Memory"
FILES
addressof.h
addressof.hpp
memory.txt
memory_overlaps.h
)

source_group(
"Memory\\Allocators"
FILES
allocators.txt
)

source_group(
"Memory\\Allocators\\Arena allocator"
FILES
arenaallocator.cpp
arenaallocator.h
arenaallocator.hpp
arenaallocator.txt
)

source_group(
"Memory\\Allocators\\Native allocator"
FILES
nativeallocator.cpp
nativeallocator.h
nativeallocator.hpp
nativeallocator.txt
)

source_group(
"Memory\\Allocators\\Pool allocator"
FILES
poolallocator.cpp
poolallocator.h
poolallocator.hpp
poolallocator.txt
poolallocator_notes.txt
)

source_group(
"Memory\\Reference counted pointers"
FILES
countedptr.cpp
countedptr.h
countedptr.hpp
countedptr.txt
referencecounted.h
referencecounted.hpp
weakptr.h
weakptr.hpp
)

source_group(
"Testing"
FILES
testing.txt
)

source_group(
"Testing\\Invariant checking"
FILES
ensure.cpp
ensure.h
ensure.hpp
ensure.txt
)

source_group(
"Testing\\Unit testing"
FILES
testreport.cpp
testreport.h
testrunner.cpp
testrunner.h
testsuite.cpp
testsuite.h
unit_testing.txt
)

source_group(
"Basic math"
FILES
constants.h
constants.hpp
eps.h
eps.hpp
histogram.h
histogram.hpp
linear.h
linear.hpp
basic_math.txt
math_functions.cpp
math_functions.h
math_functions.hpp
math_functions_more.h
math_functions_more.hpp
primes.cpp
primes.h
tristate.h
tristate.hpp
why_no_points.txt
)

source_group(
"Basic math\\Vector"
FILES
vector.h
vector.hpp
vector.txt
vector_compare.h
vector_compare.hpp
vector_design.txt
vector_tools.h
vector_tools.hpp
vector_tools_more.h
vector_tools_more.hpp
vectorbase.h
vectorexpression.h
)

source_group(
"Basic math\\Rectangle"
FILES
rectangle.h
rectangle.hpp
rectangle_tools.h
rectangle_tools.hpp
rectanglebase.h
rectanglebase.hpp
)

source_group(
"Basic math\\Tuple"
FILES
tuple.h
tuple.hpp
tuple_tools.h
tuple_tools.hpp
tuplebase.h
)

source_group(
"Basic math\\Numbers"
FILES
statreal.cpp
statreal.h
statreal_tools.cpp
statreal_tools.h
)

source_group(
"Miscellaneous"
FILES
callfunction.h
callfunction.hpp
endian.h
endian.hpp
keyvalue.h
miscellaneous.txt
pastelsys.cpp
possiblyemptymember.h
prototypefactory.h
prototypefactory.hpp
range.cpp
range.h
subset.cpp
subset.h
syslibrary.cpp
syslibrary.h
)

source_group(
"Miscellaneous\\Guid"
FILES
guid.cpp
guid.h
guid.hpp
guid.txt
)

source_group(
"Miscellaneous\\MeasureTable"
FILES
measuretable.cpp
measuretable.h
measuretable.txt
measuretable_tools.cpp
measuretable_tools.h
)

source_group(
"Miscellaneous\\BinaryFile"
FILES
binaryfile.cpp
binaryfile.h
binaryfile.txt
)

source_group(
"Miscellaneous\\AbstractArray"
FILES
abstractarray.cpp
abstractarray.h
adaptedarray.h
)

source_group(
"Miscellaneous\\IndexAllocator"
FILES
indexallocator.cpp
indexallocator.h
indexallocator.hpp
indexallocator.txt
)

source_group(
"Miscellaneous\\CommaFiller"
FILES
commafiller.h
commafiller.txt
)

source_group(
"Configuration files"
FILES
config.cpp
config.h
config.hpp
config.txt
config_tools.cpp
config_tools.h
configfile.cpp
configfile.h
)

source_group(
"Logging"
FILES
log.cpp
log.h
log.txt
log_all.h
)

source_group(
"Logging\\Log observers"
FILES
filelogger.cpp
filelogger.h
logger.cpp
logger.h
streamlogger.cpp
streamlogger.h
)

source_group(
"Random variables"
FILES
mt19937.cpp
mt19937.h
random.h
random.txt
random_beta.h
random_beta.hpp
random_beta.txt
random_cauchy.h
random_cauchy.hpp
random_cauchy.txt
random_chisquared.h
random_chisquared.hpp
random_chisquared.txt
random_dirichlet.h
random_dirichlet.hpp
random_dirichlet.txt
random_exponential.h
random_exponential.hpp
random_exponential.txt
random_gamma.h
random_gamma.hpp
random_gamma.txt
random_gaussian.h
random_gaussian.hpp
random_gaussian.txt
random_generalizedgaussian.h
random_generalizedgaussian.hpp
random_generalizedgaussian.txt
random_laplace.h
random_laplace.hpp
random_laplace.txt
random_lognormal.h
random_lognormal.hpp
random_lognormal.txt
random_triangle.h
random_triangle.hpp
random_triangle.txt
random_uniform.h
random_uniform.hpp
random_uniform.txt
randomdistribution.h
)

source_group(
"Iterators"
FILES
constantiterator.h
countingiterator.h
iterators.txt
nulliterator.h
sparseiterator.h
)

source_group(
"Iterators\\Iterator ranges"
FILES
bidirectionalrange.h
forwardrange.h
iteratorrange.h
iteratorrange.txt
randomaccessrange.h
)

source_group(
"Documentation"
FILES
../../bug_fights.txt
../../external.txt
../../installation.txt
../../internal.txt
../../more_news.txt
../../pastel.txt
pastelsys.txt
../../previous_releases.txt
../../style_conventions.txt
../../techniques.txt
../../techniques_deduction.txt
../../techniques_miscellaneous.txt
../../techniques_specialization.txt
)

source_group(
"Stl extensions"
FILES
stdext.txt
stdext_algorithm.h
stdext_copy_n.h
stdext_copy_n.hpp
stdext_destruct.h
stdext_destruct.hpp
stdext_isnan.h
stdext_subset.h
stdext_subset.hpp
)

source_group(
"Not sure"
FILES
notsure.txt
tablemodify.h
)

source_group(
"Not sure\\ListArray"
FILES
listarray.h
listarray.hpp
)

source_group(
"Not sure\\Memory view"
FILES
memoryview.h
memoryview.hpp
)

source_group(
"Not sure\\IndexExtender"
FILES
arrayextender.h
clampextender.h
indexextender.h
indexextender.txt
indexextender_all.h
mirrorextender.h
repeatextender.h
)

source_group(
"Basic algorithms"
FILES
basic_algorithms.txt
bitquery.h
bitquery.hpp
insertionsort.h
insertionsort.hpp
string_tools.cpp
string_tools.h
string_tools.hpp
syscommon.cpp
syscommon.h
syscommon.hpp
)

source_group(
"Build switches"
FILES
../CMakeLists.txt
../../example/CMakeLists.txt
../../CMakeLists.txt
CMakeLists.txt
../../test/CMakeLists.txt
environment.h
environment.hpp
environment.txt
mytypes.h
mytypes.hpp
pastelomp.h
)

set(PastelSys_Files
../../CMakeLists.txt
../../example/CMakeLists.txt
../../external.txt
../../installation.txt
../../internal.txt
../../more_news.txt
../../pastel.txt
../../previous_releases.txt
../../style_conventions.txt
../../techniques.txt
../../techniques_deduction.txt
../../techniques_miscellaneous.txt
../../techniques_specialization.txt
../../test/CMakeLists.txt
../CMakeLists.txt
CMakeLists.txt
abstractarray.cpp
abstractarray.h
adaptedarray.h
adaptedview.h
adaptedviewcursor.h
addressof.h
addressof.hpp
allocators.txt
anystorage.h
arenaallocator.cpp
arenaallocator.h
arenaallocator.hpp
arenaallocator.txt
array.h
array.hpp
array.txt
arraybase.h
arraybase.hpp
arraybasecursor.h
arrayextender.h
arrayview.h
basic_algorithms.txt
bidirectionalrange.h
binaryfile.cpp
binaryfile.h
binaryfile.txt
bitquery.h
bitquery.hpp
borderedconstview.h
../../bug_fights.txt
callfunction.h
callfunction.hpp
clampedconstview.h
clampextender.h
commafiller.h
commafiller.txt
config.cpp
config.h
config.hpp
config.txt
config_tools.cpp
config_tools.h
configfile.cpp
configfile.h
constantiterator.h
constants.h
constants.hpp
countedptr.cpp
countedptr.h
countedptr.hpp
countedptr.txt
countingiterator.h
cursorview.h
data_structures.txt
disjointgroups.h
disjointgroups.hpp
disjointgroups.txt
disjointgroups_design.txt
endian.h
endian.hpp
ensure.cpp
ensure.h
ensure.hpp
ensure.txt
environment.h
environment.hpp
environment.txt
eps.h
eps.hpp
extendedconstview.h
extendedconstview_all.h
fastlist.h
fastlist.hpp
fastlist.txt
fastlist_tools.h
fastlist_tools.hpp
fastlistprivate.hpp
filelogger.cpp
filelogger.h
forwardrange.h
functional.h
functional.hpp
guid.cpp
guid.h
guid.hpp
guid.txt
histogram.h
histogram.hpp
indexallocator.cpp
indexallocator.h
indexallocator.hpp
indexallocator.txt
indexextender.h
indexextender.txt
indexextender_all.h
insertionsort.h
insertionsort.hpp
iteratorrange.h
iteratorrange.txt
iterators.txt
keyvalue.h
linear.h
linear.hpp
listarray.h
listarray.hpp
log.cpp
log.h
log.txt
log_all.h
logger.cpp
logger.h
basic_math.txt
math_functions.cpp
math_functions.h
math_functions.hpp
math_functions_more.h
math_functions_more.hpp
measuretable.cpp
measuretable.h
measuretable.txt
measuretable_tools.cpp
measuretable_tools.h
memory.txt
memory_overlaps.h
memoryview.h
memoryview.hpp
mirroredconstview.h
mirrorextender.h
mirrorview.h
mirrorviewcursor.h
miscellaneous.txt
mt19937.cpp
mt19937.h
mytypes.h
mytypes.hpp
nativeallocator.cpp
nativeallocator.h
nativeallocator.hpp
nativeallocator.txt
notsure.txt
nulliterator.h
parallelarray.h
parallelarray.hpp
pastelomp.h
pastelsys.cpp
pastelsys.txt
permutedview.h
permutedviewcursor.h
poolallocator.cpp
poolallocator.h
poolallocator.hpp
poolallocator.txt
poolallocator_notes.txt
possiblyemptymember.h
primes.cpp
primes.h
propertyid.h
prototypefactory.h
prototypefactory.hpp
random.h
random.txt
random_beta.h
random_beta.hpp
random_beta.txt
random_cauchy.h
random_cauchy.hpp
random_cauchy.txt
random_chisquared.h
random_chisquared.hpp
random_chisquared.txt
random_dirichlet.h
random_dirichlet.hpp
random_dirichlet.txt
random_exponential.h
random_exponential.hpp
random_exponential.txt
random_gamma.h
random_gamma.hpp
random_gamma.txt
random_gaussian.h
random_gaussian.hpp
random_gaussian.txt
random_generalizedgaussian.h
random_generalizedgaussian.hpp
random_generalizedgaussian.txt
random_laplace.h
random_laplace.hpp
random_laplace.txt
random_lognormal.h
random_lognormal.hpp
random_lognormal.txt
random_triangle.h
random_triangle.hpp
random_triangle.txt
random_uniform.h
random_uniform.hpp
random_uniform.txt
randomaccessrange.h
randomdistribution.h
range.cpp
range.h
rectangle.h
rectangle.hpp
rectangle_tools.h
rectangle_tools.hpp
rectanglebase.h
rectanglebase.hpp
referencecounted.h
referencecounted.hpp
repeatedconstview.h
repeatextender.h
rowview.h
rowviewcursor.h
sliceview.h
sliceviewcursor.h
small_containers.txt
smallfixedset.h
smallfixedset.hpp
smallmap.h
smallmap.hpp
smallset.h
smallset.hpp
sparseiterator.h
sparseview.h
sparseviewcursor.h
statreal.cpp
statreal.h
statreal_tools.cpp
statreal_tools.h
stdext.txt
stdext_algorithm.h
stdext_copy_n.h
stdext_copy_n.hpp
stdext_destruct.h
stdext_destruct.hpp
stdext_isnan.h
stdext_subset.h
stdext_subset.hpp
streamlogger.cpp
streamlogger.h
string_tools.cpp
string_tools.h
string_tools.hpp
subarray.h
subarray_iterator.h
subarray_tools.h
subarray_tools.hpp
subset.cpp
subset.h
subview.h
syscommon.cpp
syscommon.h
syscommon.hpp
syslibrary.cpp
syslibrary.h
tablemodify.h
testing.txt
testreport.cpp
testreport.h
testrunner.cpp
testrunner.h
testsuite.cpp
testsuite.h
transformedview.h
transformedviewcursor.h
tristate.h
tristate.hpp
tuple.h
tuple.hpp
tuple_tools.h
tuple_tools.hpp
tuplebase.h
unit_testing.txt
hash_tables.txt
unorderedbase.h
unorderedbase.hpp
unorderedmap.h
unorderedmap.hpp
unorderedset.h
unorderedset.hpp
vector.h
vector.hpp
vector.txt
vector_compare.h
vector_compare.hpp
vector_design.txt
vector_tools.h
vector_tools.hpp
vector_tools_more.h
vector_tools_more.hpp
vectorbase.h
vectorexpression.h
view.h
view.txt
view_all.h
view_tools.h
view_tools.hpp
view_visit.h
view_visit.hpp
view_visit_more.h
view_visit_more.hpp
view_visit_more2.h
view_visit_more2.hpp
weakptr.h
weakptr.hpp
why_no_points.txt
)

include_directories(${Pastel_SOURCE_DIR})
add_library(PastelSys ${PastelSys_Files})
