// Source file for mex building

#define FORCE_LINKING(name) \
	void force_linking_##name(); \
	void call_force_linking_##name() \
	{ \
		force_linking_##name(); \
	}

FORCE_LINKING(maximum_bipartite_matching);
