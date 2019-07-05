#include "simon_fractal_rendering_cpp_CppRenderHandle.h"
#include "simon_fractal_rendering_cpp_CppRenderHandle.h"
#include "AVXFractalRenderer.h"
#include "Mandelbrot.h"
#include "DefaultColoring.h"

typedef AVXFractalRenderer FractalRenderer;

JNIEXPORT jlong JNICALL Java_simon_fractal_rendering_cpp_CppRenderHandle_create
(JNIEnv *, jclass, jint threadCount)
{
	return reinterpret_cast<jlong>(new AVXFractalRenderer(threadCount));
}

JNIEXPORT void JNICALL Java_simon_fractal_rendering_cpp_CppRenderHandle_renderNative
	(JNIEnv * env, jclass, jlong ptr, jintArray out, jstring fractalId, jstring coloringId, jint maxIterations, 
	jdouble complexUnitsXPerPixel, jdouble complexUnitsYPerPixel, jdouble topLeftReal, jdouble topLeftIm, 
	jint width, jint height, jdoubleArray fractalParamsReal, jdoubleArray fractalParamsIm, jintArray colorParams)
{
	FractalRenderer* renderer = reinterpret_cast<FractalRenderer*>(ptr);
	MandelbrotSet f;
	DefaultColoring c;
	jint* outArray = env->GetIntArrayElements(out, false);
	renderer->render<MandelbrotSet, DefaultColoring>(f, c, outArray, maxIterations, FractalImageMap(complexUnitsXPerPixel, complexUnitsYPerPixel, jcomplex(topLeftReal, topLeftIm), width, height));
	env->ReleaseIntArrayElements(out, outArray, 0);
}

JNIEXPORT void JNICALL Java_simon_fractal_rendering_cpp_CppRenderHandle_cancelNative
(JNIEnv *, jclass, jlong)
{

}

JNIEXPORT void JNICALL Java_simon_fractal_rendering_cpp_CppRenderHandle_closeNative
(JNIEnv *, jclass, jlong ptr)
{
	FractalRenderer* renderer = reinterpret_cast<FractalRenderer*>(ptr);
	delete renderer;
}