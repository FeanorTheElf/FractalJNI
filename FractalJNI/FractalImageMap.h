#pragma once
#include "jni.h"
#include <vector>
#include <complex>

typedef std::complex<jdouble> jcomplex;

class FractalImageMap {
private:
	jdouble complexUnitXPerPixel;
	jdouble complexUnitYPerPixel;
	jcomplex top_left;
	jint width;
	jint height;
	jint offsetX = 0;
	jint offsetY = 0;

public:
	class iterator {
	private:
		const FractalImageMap* parent;
		jint x;
		jint y;

	public:
		iterator(jint x, jint y, const FractalImageMap* parent);
		iterator(const iterator& that) = default;
		iterator(iterator&& that) = default;
		~iterator() = default;

		iterator& operator=(const iterator& that) = default;

		jdouble getReal() const;
		jdouble getIm() const;
		jcomplex operator*() const;
		iterator& operator++();
		bool operator==(const iterator& that) const;
		bool operator!=(const iterator& that) const;
		jint getX() const;
		jint getY() const;
	};

	FractalImageMap(jdouble complexUnitXPerPixel, jdouble complexUnitYPerPixel, jcomplex top_left, jint width, jint height);
	FractalImageMap() = default;
	FractalImageMap(const FractalImageMap& toCopy) = default;
	FractalImageMap(FractalImageMap&& toMove) = default;
	~FractalImageMap() = default;
	FractalImageMap& operator=(const FractalImageMap& value) = default;
	iterator begin() const;
	iterator end() const;
	std::vector<FractalImageMap> divide(size_t count) const;
	jint getWidth() const;

	friend std::ostream& operator<<(std::ostream& os, const FractalImageMap& map);
};

inline FractalImageMap::iterator FractalImageMap::begin() const
{
	return iterator(0, 0, this);
}

inline FractalImageMap::iterator FractalImageMap::end() const
{
	return iterator(0, height, this);
}

inline FractalImageMap::iterator::iterator(jint x, jint y, const FractalImageMap* parent) : x(x), y(y), parent(parent)
{
}

inline jdouble FractalImageMap::iterator::getReal() const
{
	return x * parent->complexUnitXPerPixel + parent->top_left.real();
}

inline jdouble FractalImageMap::iterator::getIm() const
{
	return -y * parent->complexUnitYPerPixel + parent->top_left.imag();
}

inline jcomplex FractalImageMap::iterator::operator*() const
{
	return jcomplex(getReal(), getIm());
}

inline FractalImageMap::iterator& FractalImageMap::iterator::operator++()
{
	++x;
	if (x >= parent->width) {
		x = 0;
		++y;
	}
	if (y >= parent->height) {
		x = 0;
		y = parent->height;
	}
	return *this;
}

inline bool FractalImageMap::iterator::operator==(const iterator & that) const
{
	return this->x == that.x && this->y == that.y;
}

inline bool FractalImageMap::iterator::operator!=(const iterator & that) const
{
	return !(*this == that);
}

inline jint FractalImageMap::iterator::getX() const
{
	return x + this->parent->offsetX;
}

inline jint FractalImageMap::iterator::getY() const
{
	return y + this->parent->offsetY;
}

inline jint FractalImageMap::getWidth() const
{
	return width;
}

inline std::ostream& operator<<(std::ostream& os, const FractalImageMap& map) {
	os << "(" << map.top_left << "; " << map.width << " x " << map.height << ")";
	return os;
}