#define FORCE_LINKING(name) \
	void force_linking_##name(); \
	force_linking_##name()

namespace
{

	void forceLinking()
	{
		FORCE_LINKING(point_pattern_match);
		FORCE_LINKING(point_pattern_match_gmo);
	}

}
