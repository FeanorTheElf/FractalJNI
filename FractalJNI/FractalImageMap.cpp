#include "stdafx.h"
#include "FractalImageMap.h"

FractalImageMap::FractalImageMap(jdouble complexUnitXPerPixel, jdouble complexUnitYPerPixel, jcomplex top_left, jint width, jint height) :
	complexUnitXPerPixel(complexUnitXPerPixel), complexUnitYPerPixel(complexUnitYPerPixel), top_left(top_left), width(width), height(height)
{
}

std::vector<FractalImageMap> FractalImageMap::divide(size_t count) const
{
	std::vector<FractalImageMap> result(count);
	jint untilNow = 0;
	jdouble widthDelta = (jdouble)this->width / count;
	jcomplex divisionDelta(widthDelta * complexUnitXPerPixel, 0);
	for (jint i = 0; i < count; ++i) {
		jint partWidth = (int)((i + 1) * widthDelta) - untilNow;
		result[i] = FractalImageMap(complexUnitXPerPixel, complexUnitYPerPixel, top_left + divisionDelta * (jdouble)i, partWidth, this->height);
		result[i].offsetX = untilNow;
		untilNow += partWidth;
	}
	return result;
}