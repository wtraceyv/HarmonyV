#include <math.h>

namespace util
{
	float wrapPhase(float phase)
	{
		return (phase >= 0.0f) ? 
			fmod(phase + M_PI, 2.0f * M_PI) - M_PI
			: fmod(phase + M_PI, -2.0f * M_PI) + M_PI;
	}
}