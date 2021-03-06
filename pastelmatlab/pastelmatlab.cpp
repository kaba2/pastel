#include <tbb/scalable_allocator.h>

// Source file for mex building

#define FORCE_LINKING(name) \
	void force_linking_##name(); \
	void call_force_linking_##name() \
	{ \
		force_linking_##name(); \
	}

void call_force_linking_scalable_free() {
	void* ptr = 0;
	scalable_free(ptr);
}

FORCE_LINKING(maximum_bipartite_matching);
FORCE_LINKING(random_generalized_normal);

FORCE_LINKING(random_orthogonal);

FORCE_LINKING(coherent_point_drift);
FORCE_LINKING(ls_affine);
FORCE_LINKING(match_points_vw);
FORCE_LINKING(match_points_kr);
FORCE_LINKING(pointkdtree);
