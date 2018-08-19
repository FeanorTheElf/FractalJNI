#pragma once
#include "jni.h"
#include "FractalImageMap.h"
#include <immintrin.h>
#include <future>
#include <vector>

class AVXFractalRenderer{
private:
	std::vector<std::future<void>> futures;

public:
	AVXFractalRenderer(jint threadCount);
	~AVXFractalRenderer();

	template<typename Fractal, typename Coloring>
	void render(const Fractal& fractal, const Coloring& coloring, jint * out, jint maxIterations, const FractalImageMap& part);
};

struct complex4 {
	double real[4];
	double im[4];
};

template<typename Fractal, typename Coloring>
inline void renderSyncAVX(const Fractal& fractal, const Coloring& coloring, jint * const out, const int64_t totalWidth, const jint maxIterations, const FractalImageMap & part)
{
	MandelbrotSet mandelbrot;
	auto it = part.begin();
	while (it != part.end()) {
		auto startIt(it);
		complex4 values;
		for (unsigned i = 0; i < 4; ++i) {
			values.real[i] = it.getReal();
			values.im[i] = it.getIm();
			++it;
		}
		__m256i result = fractal.getIterationCount(_mm256_setr_pd(values.real[0], values.real[1], values.real[2], values.real[3]),
			_mm256_setr_pd(values.im[0], values.im[1], values.im[2], values.im[3]), maxIterations);

		it = startIt;
		for (unsigned i = 0; i < 4 && !(it == part.end()); ++i) {
			out[it.getY() * totalWidth + it.getX()] = coloring.getColor(result.m256i_i64[i] == maxIterations + 1 ? -1 : result.m256i_i64[i]);
			++it;
		}
	}
}

inline AVXFractalRenderer::AVXFractalRenderer(jint threadCount) : futures(threadCount)
{
}

inline AVXFractalRenderer::~AVXFractalRenderer()
{
}

template<typename Fractal, typename Coloring>
inline void AVXFractalRenderer::render(const Fractal& fractal, const Coloring& coloring, jint * out, jint maxIterations, const FractalImageMap& part)
{
	std::vector<FractalImageMap> divisions = part.divide(futures.size());
	for (jint i = 0; i < futures.size(); ++i) {
		futures[i] = std::async(std::launch::async, [&, i]() {
			renderSyncAVX(fractal, coloring, out, part.getWidth(), maxIterations, divisions[i]);
		});
	}
	for (std::future<void>& f : futures) {
		f.wait();
	}
}

