// Source file for mex building

#define FORCE_LINKING(name) \
	void force_linking_##name(); \
	void call_force_linking_##name() \
	{ \
		force_linking_##name(); \
	}

FORCE_LINKING(ls_affine);
FORCE_LINKING(match_points_vw);
FORCE_LINKING(match_points_kr);
FORCE_LINKING(pointkdtree);

