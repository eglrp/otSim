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

#ifndef VECTORN_H
#define VECTORN_H

#include <vector>

namespace tmath{

/**
	@class 			vectorn
	@brief 			Class that represents a n-dimensional vector
	@author 		Cengiz Terzibas
*/
/*
template<typename T, int NUM>
class vectorn {
public:
	typedef T value_type;
	T cmp[NUM];
	// constructors
	vectorn<T,NUM>() {
		for(int idx = 0; idx < NUM; ++idx)
			cmp[idx] = static_cast<T>(0.0);
	}
	vectorn<T,NUM>(const T* vector){
		for(int idx = 0; idx < NUM; ++idx)
			cmp[idx] = vector[idx];
	}
	vectorn<T,NUM>(const vectorn<T,NUM>& v)  {
		for(int idx = 0; idx < NUM; ++idx)
			cmp[idx] = v.cmp[idx];
	}
	template<typename U>
	vectorn<T, 4>(const vectorn<U, NUM>& v)  {
		for (int idx = 0; idx < NUM; ++idx)
			cmp[idx] = static_cast<T>(v.cmp[idx]);
	}
	void init() {
		for (int idx = 0; idx < NUM; ++idx)
			cmp[idx] = static_cast<T>(0);
	}
	// assignment operations
	inline const vectorn<T,NUM> operator = ( const vectorn<T,NUM>& v ) {
		for(int idx = 0; idx < NUM; ++idx)
			cmp[idx] = v.cmp[idx];
		return *this;
	}
	inline const vectorn<T,NUM> operator+=(const vectorn<T,NUM>& v) {
		for(int idx = 0; idx < NUM; ++idx)
			cmp[idx] += v.cmp[idx];
		return *this;
	}
	inline const vectorn<T,NUM> operator-=(const vectorn<T,NUM>& v) {
		for(int idx = 0; idx < NUM; ++idx)
			cmp[idx] -= v.cmp[idx];
		return *this;
	}
	inline const vectorn<T,NUM> operator*=(const T& num) {
		for(int idx = 0; idx < NUM; ++idx)
			cmp[idx] *= num;
		return *this;
	}
	inline const vectorn<T,NUM> operator/=(const T& num) {
		const T r = static_cast<T>(1.0)/num;
		for(int idx = 0; idx < NUM; ++idx)
			cmp[idx] *= r;
		return *this;
	}
	// stream operations
	friend std::ostream& operator<<(std::ostream& out, const vectorn<T,NUM>& v) {
		out << "[";
		for(int idx = 0; idx < NUM; ++idx) {
			out << v.cmp[idx];
			if(idx + 1 <  NUM)
				out << ",";
		}
		out << "]";
		return out;
	}
	// comparison operations
	inline const bool operator == ( const vectorn<T,NUM>& v ) const {
		bool res = true;
		for(int idx = 0; idx < NUM; ++idx)
			res &= v.cmp[idx] == cmp[idx];
		return res;
	}
	inline const bool operator != ( const vectorn<T,NUM>& v ) const {
		return !(v == *this);
	}
	// unary operations
	inline const vectorn<T,NUM> operator - () const {
		vectorn<T,NUM> v;
		for(int idx = 0; idx < NUM; ++idx)
			v.cmp[idx] = -cmp[idx];
		return v;
	}
	// binary operations
	inline friend const vectorn<T,NUM> operator+(const vectorn<T,NUM>& v1,const vectorn<T,NUM>& v2) {
		vectorn<T,NUM> res;
		for(int idx = 0; idx < NUM; ++idx)
			res[idx] = v1.cmp[idx] + v2.cmp[idx];
		return res;
	}
	inline friend const vectorn<T,NUM> operator-(const vectorn<T,NUM>& v1, const vectorn<T,NUM>& v2) {
		vectorn<T,NUM> res;
		for(int idx = 0; idx < NUM; ++idx)
			res[idx] = v1.cmp[idx] - v2.cmp[idx];
		return res;
	}
	//inline friend const T	operator*(const vectorn<T,NUM>& v1,const vectorn<T,NUM>& v2) {
	//	T res = static_cast<T>(0.0);
	//	for(int idx = 0; idx < NUM; ++idx)
	//		res += v1.cmp[idx] * v2.cmp[idx];
	//	return res;
	//}
	inline const vectorn<T,NUM> operator*(const T& num) const {
		vectorn<T,NUM> v;
		for(int idx = 0; idx < NUM; ++idx)
			v.cmp[idx] = cmp[idx]*num;
		return v;
	}
	friend inline const vectorn<T,NUM> operator * ( const T& s, const vectorn<T,NUM>& v ) {
		return v * s;
	}
	inline const vectorn<T,NUM> operator/(const T& num) const {
		vectorn<T,NUM> v;
		const T r = static_cast<T>(1.0)/num;
		for(int idx = 0; idx < NUM; ++idx)
			v.cmp[idx] = cmp[idx]*r;
		return v;
	}
	size_t size() const {
		return NUM;
	}
	// cast operations
	operator T*() {
		return cmp;
	}
	operator const T*() const {
		return cmp;
	}

	// dot product
	inline const T	dot(const vectorn<T,NUM>& v) {
		T res = static_cast<T>(0.0);
		for(int idx = 0; idx < NUM; ++idx)
			res += cmp[idx] * v.cmp[idx];
		return res;
	}

	inline const vectorn<T, NUM> multiplyComponents(const vectorn<T, NUM>& v) const {
		vectorn<T, NUM> v;
		for (int idx = 0; idx < NUM; ++idx)
			v.cmp[idx] = cmp[idx] * v.cmp[idx];
		return v;
	}

	// Calculate the normal
	inline T norm() const {
		T res = static_cast<T>(0.0);
		for (int idx = 0; idx < NUM; ++idx)
			res += cmp[idx] * cmp[idx];
		return res;
	}
	// Calculate the length
	inline T len() const {
		T res = static_cast<T>(0.0);
		for (int idx = 0; idx < NUM; ++idx)
			res += cmp[idx] * cmp[idx];
		res = sqrt(res);
		return res;
	}

	// Normalize the vector (in place)
	inline const vectorn<T, NUM>& normalize() {
		T length = len();
		if (length > 0) {
			T value = static_cast<T>(1.0) / length;
			for (int idx = 0; idx < NUM; ++idx)
				cmp[idx] *= value;
		}
		return *this;
	}

	// Return a Normalized copy of the vector
	inline const vectorn<T, NUM> normalized() const {
		vectorn<T, NUM> v(*this);
		return normalize(v);
	}

	inline T magnitude2() const {
		return norm();
	}

	inline T magnitude() const {
		return len();
	}

	/// Checks the vector and returns true if it is null (all zeros)
	inline bool isNull() const {
		bool allNull = true;
		for (int idx = 0; idx < NUM; ++idx) {
			if (cmp[idx] != 0) {
				return false;
			}
		}
		return true;
	}

	inline const vectorn<T, NUM> unitVector() const {
		return normalized();
	}
};

*/
}
#endif

