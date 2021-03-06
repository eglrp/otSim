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

#ifndef VECTOR2_H
#define VECTOR2_H

namespace tmath{

template<typename T, int NUM>
class vectorn;

/**
	@author 		Cengiz Terzibas
*/

template<typename T>
class vectorn<T,2> {
public:
	typedef T value_type;
	T x, y;
	// constructors
	vectorn<T,2>() :	x(static_cast<T>(0.0)),
										y(static_cast<T>(0.0)) {
	}
	vectorn<T,2>(T x_, T y_) : x(x_), y(y_) {
	}
	vectorn<T,2>(const T* v) : x(v[0]), y(v[1]) {
	}
	vectorn<T,2>(const vectorn<T,2>& v) : x(v.x), y(v.y) {
	}
	template<typename U>
	vectorn<T, 2>(const vectorn<U, 2>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {
	}
	void init() {
		x = y = static_cast<T>(0);
	}
	// assignment operations
	inline const vectorn<T,2> operator+=(const vectorn<T,2>& v) {
		x += v.x; y += v.y; return *this;
	}
	inline const vectorn<T,2> operator-=(const vectorn<T,2>& vVector) {
		x -= vVector.x; y -= vVector.y; return *this;
	}
	inline const vectorn<T,2> operator*=(const T& num) {
		x *= num; y *= num;  return *this;
	}
	inline const vectorn<T,2> operator/=(const T& num) {
		const T r = 1.0/num; x *= r; y *= r; return *this;
	}
	// stream operations
	inline friend std::ostream& operator<<(std::ostream& out, const vectorn<T,2>& v) {
		out << "[" << v.x << "," << v.y << "]";  return out;
	}
	// comparison operations
	inline const bool operator == ( const vectorn<T,2>& v ) const {
		return (v.x==x && v.y==y );
	}
	inline const bool operator != ( const vectorn<T,2>& v ) const {
		return !(v == *this);
	}
	// unary operations
	inline const vectorn<T,2> operator - () const {
		return vectorn<T,2>( -x, -y);
	}
	// binary operations
	inline friend const vectorn<T,2> operator+(const vectorn<T,2>& v1, const vectorn<T,2>& v2){
		return vectorn<T,2>(v1.x + v2.x, v1.y + v2.y);
	}
	inline friend const vectorn<T,2> operator-(const vectorn<T,2>& v1, const vectorn<T,2>& v2) {
		return vectorn<T,2>(v1.x - v2.x, v1.y - v2.y);
	}
	//inline friend const T	operator*(const vectorn<T,2>& v1,const vectorn<T,2>& v2) {
	//	return T(v1.x * v2.x + v1.y *v2.y );
	//}
	inline const vectorn<T,2> operator*(const T& num) const {
		return vectorn<T,2>(x * num, y * num);
	}
	friend inline const vectorn<T,2> operator * ( const T& s, const vectorn<T,2>& v ) {
		return v * s;
	}
	inline const vectorn<T,2> operator/(const T& num) const {
		const T r = 1.0/num; return vector2(x * r, y * r);
	}
	size_t size() const {
		return 2;
	}
	// cast operations
	operator T*() {
		return &x;
	}
	operator const T*() const	{
		return &x;
	}

	// dot product
	inline T dot(const vectorn<T, 2>& v) const {
		return T(x * v.x + y * v.y);
	}

	inline const vectorn<T, 2>	multiplyComponents(const vectorn<T, 2>& v) const {
		return vectorn(x * v.x, y * v.y);
	}

	// Calculate the normal
	inline T norm() const {
		return static_cast<T>(x*x + y*y);
	}
	// Calculate the length
	inline T len() const {
		return static_cast<T>(sqrt(x*x + y*y));
	}

	// Normalize the vector (in place)
	inline vectorn<T, 2>& normalize() {
		T length = len();
		if (length > 0) {
			T value = static_cast<T>(1.0) / length;
			x *= value; y *= value;
		}
		return *this;
	}

	// Return a Normalized copy of the vector
	inline const vectorn<T, 2> normalized() const {
		vectorn<T, 2> v(*this);
		return v.normalize();
	}
	inline T distance(const vectorn<T, 2>& v) const {
		return ((*this - v).len());
	}

	inline T distance2(const vectorn<T, 2>& v) const {
		return ((*this - v).norm());
	}

	inline T magnitude2() const {
		return norm();
	}

	inline T magnitude() const {
		return len();
	}

	inline const vectorn<T, 2> unitVector() const {
		return normalized();
	}

	/// Checks the vector and returns true if it is null (all zeros)
	inline bool isNull() const {
		return (x == 0 && y == 0);
	}

	inline T angleBetween(const vectorn<T, 2>& v) const {
		return acos(unitVector().dot(v.unitVector()));
	}

	/// Returns a reflected vector, based on the given normal vector
	const vectorn<T, 2> reflect(const vectorn<T, 2>& normalVec) const {
		vectorn<T, 2> unitNormal = normalVec.unitVector();
		return (unitNormal * -2 * this->dot(unitNormal) + (*this));
	}
};

}

#endif

