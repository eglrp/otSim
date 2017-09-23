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

#ifndef VECTOR3_H
#define VECTOR3_H

namespace tmath{

template<typename T, int NUM>
class vectorn;

/**
	@author 		Cengiz Terzibas
*/

template<typename T>
class vectorn<T, 3> {
public:
	typedef T value_type;
	T x, y, z;
	// constructors
	vectorn<T,3>() :	x(static_cast<T>(0.0)),
										y(static_cast<T>(0.0)),
										z(static_cast<T>(0.0)) {
	}
	vectorn<T,3>(T vx, T vy, T vz) : x(vx), y(vy), z(vz) {
	}
	vectorn<T,3>(const T* v) : x(v[0]), y(v[1]), z(v[2]) {
	}
	vectorn<T,3>(const vectorn<T,3>& v) : x(v.x), y(v.y), z(v.z) {
	}
	template<typename U>
	vectorn<T, 3>(const vectorn<U, 3>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {
	}
	void init() {
		x = y = z = static_cast<T>(0);
	}
	// assignment operations
	inline const vectorn<T,3> operator+=(const vectorn<T,3>& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}
	inline const vectorn<T,3> operator-=(const vectorn<T,3>& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	inline const vectorn<T,3> operator*=(const T& num) {
		x *= num; y *= num; z *= num; return *this;
	}
	inline const vectorn<T,3> operator/=(const T& num) {
		const T r = static_cast<T>(1.0)/num; x *= r; y *= r; z *=r; return *this;
	}
	// stream operations
	inline friend std::ostream& operator<<(std::ostream& out, const vectorn<T,3>& v)	{
		out << "[" << v.x << "," << v.y << "," << v.z << "]"; return out;
	}
	// comparison operations
	inline const bool operator == ( const vectorn<T,3>& v ) const {
		return (v.x==x && v.y==y && v.z==z);
	}
	inline const bool operator != ( const vectorn<T,3>& v ) const {
		return !(v == *this);
	}
	// unary operations
	inline const vectorn<T,3> operator - () const {
		return vectorn<T,3>( -x, -y, -z);
	}
	// binary operations
	inline friend const vectorn<T,3> operator+(const vectorn<T,3>& v1, const vectorn<T,3>& v2) {
		return vectorn<T,3>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}
	inline friend const vectorn<T,3> operator-(const vectorn<T,3>& v1, const vectorn<T,3>& v2) {
		return vectorn<T,3>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}
	//inline friend const T	operator*(const vectorn<T,3>& v1,const vectorn<T,3>& v2) {
	//	return T(v1.x * v2.x + v1.y *v2.y + v1.z * v2.z);
	//}
	inline const vectorn<T,3> operator*(const T& num) const {
		return vectorn<T,3>(x * num, y * num, z * num);
	}
	friend inline const vectorn<T,3> operator * ( const T& s, const vectorn<T,3>& v ) {
		return v * s;
	}
	inline const vectorn<T,3> operator/(const T& num) const {
		const T r = static_cast<T>(1.0)/num; return vectorn<T,3>(x * r, y * r, z * r);
	}
	size_t size() const {
		return 3;
	}
	// cast operations
	operator T*() {
		return &x;
	}
	operator const T*() const {
		return &x;
	}

	/// cross product
	inline const tmath::vectorn<T, 3> operator*(const vectorn<T, 3>& v) const {
		return tmath::vectorn<T, 3>((y * v.z) - (z * v.y),
									(z * v.x) - (x * v.z),
									(x * v.y) - (y * v.x));
	}

	/// dot product
	inline T dot(const vectorn<T, 3>& v) const {
		return T(x * v.x + y * v.y + z * v.z);
	}

	inline const vectorn<T, 3>	multiplyComponents(const vectorn<T, 3>& v) const {
		return vectorn(x * v.x, y * v.y, z * v.z);
	}

	/// Calculate the normal
	inline T norm() {
		return static_cast<T>(x*x + y*y + z*z);
	}
	/// Calculate the length
	inline T len() {
		return static_cast<T>(sqrt(x*x + y*y + z*z));
	}

	/// Normalize the vector (in place)
	inline vectorn<T, 3>& normalize() {
		T length = len();
		if (length > 0) {
			T value = static_cast<T>(1.0) / length;
			v.x *= value; v.y *= value; v.z *= value;
		}
		return *this;
	}

	/// Return a Normalized copy of the vector
	inline const vectorn<T, 3> normalized() const {
		vectorn<T, 3> v(this);
		return normalize(v);
	}
	inline T distance(const vectorn<T, 3>& v) const {
		return ((*this - v).len());
	}

	inline T distance2(const vectorn<T, 3>& v) const {
		return ((*this - v).norm());
	}

	inline T magnitude2() {
		return norm();
	}

	inline T magnitude() {
		return len();
	}

	inline const vectorn<T, 3> unitVector() const {
		return normalized();
	}

	inline T angleBetween(const vectorn<T, 3>& v) const {
		return acos(unitVector().dot(v.unitVector()));
	}

	/// Returns a reflected vector, based on the given normal vector
	const vectorn<T, 3> reflect(const vectorn<T, 3>& normalVec) const {
		vectorn<T, 3> unitNormal = normalVec.unitVector();
		return (unitNormal * -2 * this->dot(unitNormal) + (*this));
	}

};

}

#endif

