#pragma once

#include "util.h"

namespace Jil
{

	struct TestParams
	{
		int width;
		int height;
		float midSize;
		int alpha;
		int count;
		float midWeight;
		float midBlur;
		bool flag;

		static TestParams getRandom()
		{
			TestParams params;

			float imgAR = randBool(0.5f) ? randInt(6, 20) / 10.0f : 1;
			params.width = (int)pow(2.0f, randInt(6, 12));
			params.height = (int)(params.width / imgAR);

			params.midSize = params.width / randInt(6, 18);
			params.alpha = randBool(0.75f) ? 0x10 * randInt(0x4, 0x10) : 0xff;
			params.count = 2 * params.width * 0xff
				/ ((params.alpha / 2 + 0x80) * sqrt(params.midSize) * randFloat(0.5f, 3.5f));
			params.midWeight = randInt(-8, 9);
			params.midBlur = randBool(0.7f) ? 1.0f : randInt(2, 9);

			params.flag = randBool(0.5f);

			return params;
		}

		int randSizeX() const { return randInt(-midSize, width + midSize); }
		int randSizeY() const { return randInt(-midSize, height + midSize); }
		Vec2 randSizeVec() const { return Vec2(randSizeX(), randSizeY()); }
		int randWeightX() const { return randInt(-midWeight, width + midWeight); }
		int randWeightY() const { return randInt(-midWeight, height + midWeight); }
		Vec2 randWeightVec() const { return Vec2(randWeightX(), randWeightY()); }

		int randSize() const { return randInt(midSize / 2, 2 * midSize); }
		int randWeight() const { return randInt(midWeight / 2, 2 * midWeight); }
		int randBlur() const { return midBlur == 1.0f ? 1.0f :
			randInt(midBlur / 2, 2 * midBlur); }
	};
}