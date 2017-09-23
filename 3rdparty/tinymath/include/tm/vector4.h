/*
	Copyright (c) 2003-2005 Cengiz Terzibas

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Cengiz Terzibas         cengiz@terzibas.de
*/

//Source modified by Cory Parks 09/2017

#ifndef VECTOR4_H
#define VECTOR4_H

namespace tmath{

template<typename T>
class quaternion;

template<typename T, int NUM>
class vectorn;

/**
	@author 		Cengiz Terzibas
*/

template<typename T>
class vectorn<T,4> {
public:
	typedef T value_type;
	T x, y, z, w;
	// constructors
	vectorn<T,4>() :	x(static_cast<T>(0.0)),
									y(static_cast<T>(0.0)),
									z(static_cast<T>(0.0)),
									w(static_cast<T>(0.0)) {
	}
	vectorn<T,4>(T vx, T vy, T vz, T vw) : x(vx), y(vy), z(vz), w(vw) {
	}
	vectorn<T,4>(const T* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {
	}
	vectorn<T,4>(const vectorn<T,4>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {
	}
	template<typename U>
	vectorn<T, 4>(const vectorn<U, 4>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z), w(static_cast<T>(v.w))) {
	}
	void init() {
		x = y = z = w = static_cast<T>(0);
	}
	// assignment operations
	inline const vectorn<T,4> operator+=(const vectorn<T,4>& v) {
		x += v.x; y += v.y; z += v.z; w += v.w; return *this;
	}
	inline const vectorn<T,4> operator-=(const vectorn<T,4>& v) {
		x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;
	}
	inline const vectorn<T,4> operator*=(const T& num) {
		x *= num; y *= num; z *= num; w *= num; return *this;
	}
	inline const vectorn<T,4> operator/=(const T& num) {
		const T r = (T)1.0/num; x *= r; y *= r; z *=r; w *= r; return *this;
	}
	// stream operations
	inline friend std::ostream& operator<<(std::ostream& out, const vectorn<T,4>& v) {
		out << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]"; return out;
	}
	// comparison operations
	inline const bool operator == ( const vectorn<T,4>& v ) const {
		return (v.x==x && v.y==y && v.z==z && v.w==w);
	}
	inline const bool operator != ( const vectorn<T,4>& v ) const {
		return !(v == *this);
	}
	// unary operations
	inline const vectorn<T,4> operator - () const {
		return vectorn<T,4>( -x, -y, -z, -w);
	}
	// binary operations
	inline friend const vectorn<T,4> operator+(const vectorn<T,4>& v1, const vectorn<T,4>& v2)  {
		return vectorn(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z,v1.w + v2.w);
	}
	inline friend const vectorn<T,4> operator-(const vectorn<T,4>& v1, const vectorn<T,4>& v2) {
		return vectorn(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
	}
	//inline friend const T	operator*(const vectorn<T,4>& v1,const vectorn<T,4>& v2) {
	//	return T(v1.x * v2.x + v1.y *v2.y + v1.z * v2.z + v1.w * v2.w);
	//}
	inline const vectorn<T,4> operator*(const T& num) const {
		return vectorn(x * num, y * num, z * num, w * num);
	}
	friend inline const vectorn<T,4> operator * ( const T& s, const vectorn<T,4>& v ) {
		return v * s;
	}
	inline const vectorn<T,4> operator/(const T& num) const {
		const T r = (T)1.0/num; return vectorn(x * r, y * r, z * r, w * r);
	}
	size_t size() const {
		return 4;
	}
	// cast operations
	operator T*() {
		return &x;
	}
	operator const T*() const {
		return &x;
	}

	// dot product
	inline T dot(const vectorn<T, 4>& v) const {
		return T(x * v.x + y * v.y + z * v.z + w * v.w);
	}

	inline const vectorn<T, 4>	multiplyComponents(const vectorn<T,4>& v) const {
		return vectorn(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	// Calculate the normal
	inline T norm() {
		return static_cast<T>(x*x + y*y + z*z + w*w);
	}
	// Calculate the length
	inline T len() {
		return static_cast<T>(sqrt(x*x + y*y + z*z + w*w));
	}

	// Normalize the vector (in place)
	inline vectorn<T, 4>& normalize() {
		T length = len();
		if (length > 0) {
			T value = static_cast<T>(1.0) / length;
			v.x *= value; v.y *= value; v.z *= value; v.w *= value;
		}
		return *this;
	}

	// Return a Normalized copy of the vector
	inline const vectorn<T, 4> normalized() const {
		vectorn<T, 4> v(this);
		return normalize(v);
	}

	inline T magnitude2() {
		return norm();
	}

	inline T magnitude() {
		return len();
	}

	inline const vectorn<T, 4> unitVector() const {
		return normalized();
	}
};

}

#endif

