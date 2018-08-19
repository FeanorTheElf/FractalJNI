#pragma once
#include "jni.h"
#include "FractalImageMap.h"
#include <future>
#include <vector>

class ParallelFractalRenderer{
private:
	std::vector<std::future<void>> futures;

public:
	ParallelFractalRenderer(jint threadCount);
	~ParallelFractalRenderer() = default;

	template<typename Fractal, typename Coloring>
	void render(const Fractal& fractal, const Coloring& coloring, jint * out, jint maxIterations, const FractalImageMap& part);
};

template<typename Fractal, typename Coloring>
inline void renderSync(const Fractal& fractal, const Coloring& coloring, jint * const out, const jint totalWidth, const jint maxIterations, const FractalImageMap & part)
{
	for (FractalImageMap::iterator it = part.begin(); !(it == part.end()); ++it) {
		out[it.getY() * totalWidth + it.getX()] = coloring.getColor(fractal.getIterationCount(*it, maxIterations));
	}
}

inline ParallelFractalRenderer::ParallelFractalRenderer(jint threadCount) : futures(threadCount)
{
}

inline ParallelFractalRenderer::~ParallelFractalRenderer()
{
}

template<typename Fractal, typename Coloring>
inline void ParallelFractalRenderer::render(const Fractal& fractal, const Coloring& coloring, jint * out, jint maxIterations, const FractalImageMap& part)
{
	std::vector<FractalImageMap> divisions = part.divide(futures.size());
	for (jint i = 0; i < futures.size(); ++i) {
		futures[i] = std::async(std::launch::async, [&, i]() {
			renderSync(fractal, coloring, out, part.getWidth(), maxIterations, divisions[i]);
		});
	}
	for (std::future<void>& f : futures) {
		f.wait();
	}
}

