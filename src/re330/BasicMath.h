#ifndef __BasicMath_h__
#define __BasicMath_h__

#include <math.h>

namespace RE330
{
	/** Basic math utility class.
	@remarks
		Implements some basic math utilities and defines useful constants,
		such as PI.
	*/
	class BasicMath
	{
	public:
		static const float PI;

		static float radian(float a) { return a*PI/180.f; };
	};

}

#endif

