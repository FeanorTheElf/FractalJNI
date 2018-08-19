#pragma once
#include "jni.h"
#include <complex>
#include <immintrin.h>

class MandelbrotSet {
public:
	jint getIterationCount(const std::complex<jdouble>, const jint) const noexcept;
	__m256i getIterationCount(__m256d cReal, __m256d cIm, unsigned maxIterations) const noexcept;
};

inline jint MandelbrotSet::getIterationCount(const std::complex<jdouble> c, const jint maxIterations) const noexcept
{
	jdouble currentReal = 0;
	jdouble currentIm = 0;
	jdouble realSquare;
	jdouble imSquare;
	for (int i = 0; i < maxIterations; ++i) {
		realSquare = currentReal * currentReal;
		imSquare = currentIm * currentIm;
		currentIm = 2 * currentReal * currentIm + c.imag();
		currentReal = realSquare - imSquare + c.real();
		if (realSquare + imSquare >= 4) {
			return i;
		}
	}
	return -1;
}

inline __m256i MandelbrotSet::getIterationCount(__m256d cReal, __m256d cIm, unsigned maxIterations) const noexcept {
	__m256i result = _mm256_set_epi64x(0, 0, 0, 0);
	__m256d currentReal = _mm256_set_pd(0, 0, 0, 0);
	__m256d currentIm = _mm256_set_pd(0, 0, 0, 0);
	__m256d realSquare;
	__m256d imSquare;
	for (unsigned i = 0; i <= maxIterations; ++i)
	{
		realSquare = _mm256_mul_pd(currentReal, currentReal);
		imSquare = _mm256_mul_pd(currentIm, currentIm);

		currentIm = _mm256_mul_pd(currentIm, _mm256_set_pd(2, 2, 2, 2));
		currentIm = _mm256_fmadd_pd(currentIm, currentReal, cIm);

		currentReal = _mm256_sub_pd(realSquare, imSquare);
		currentReal = _mm256_add_pd(currentReal, cReal);

		__m256i isSmaller = _mm256_castpd_si256(_mm256_cmp_pd(_mm256_add_pd(realSquare, imSquare), _mm256_set_pd(4, 4, 4, 4), _CMP_LE_OS));
		result = _mm256_sub_epi64(result, isSmaller);

		if (i % 10 == 0 && !isSmaller.m256i_i64[0] && !isSmaller.m256i_i64[1] && !isSmaller.m256i_i64[2] && !isSmaller.m256i_i64[3]) return result;
	}
	return result;
}