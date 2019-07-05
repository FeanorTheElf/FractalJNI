#pragma once
#include "jni.h"

class DefaultColoring {
public:
	jint getColor(jint) const;
};

inline jint DefaultColoring::getColor(jint i) const
{
	if (i == -1) {
		return 0xFF000000;
	}
	else {
		return (i << ((i >> 5) & 0xFF)) | 0xFF000000;
	}
}