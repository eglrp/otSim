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

#ifndef MATRIX2_H
#define MATRIX2_H

namespace tmath{

template<typename T, int N, int M>
class matrix;

/**
	@author 		Cengiz Terzibas
*/

template<typename T>
class matrix<T,2,2> {
public:
	typedef T value_type;
	T xx, xy;
	T yx, yy;
	// constructors
	matrix<T,2,2>() :	xx(static_cast<T>(0.0)),
										xy(static_cast<T>(0.0)), 
										yx(static_cast<T>(0.0)), 
										yy(static_cast<T>(0.0)) {
	}
	matrix<T,2,2>(	T a0, T a1,
							T a2,	T a3)
						:	xx(a0), xy(a1),
							yx(a2), yy(a3) {
	}
	matrix<T,2,2>(const T* m) {
		xx = m[0];	xy = m[1];
		yx = m[2];	yy = m[3];
	}
	matrix<T,2,2>(const matrix<T,2,2>& m) {
		xx = m.xx; xy = m.xy;
		yx = m.yx; yy = m.yy;
	}
	// assignment operations
	inline matrix<T,2,2> operator+=(const matrix<T,2,2>& m) {
		xx += m.xx; xy += m.xy; 
		yx += m.yx; yy += m.yy;
		return *this;
	}
	inline matrix<T,2,2> operator-=(const matrix<T,2,2>& m) {
		xx -= m.xx; xy -= m.xy;
		yx -= m.yx; yy -= m.yy;
		return *this;
	}
	inline matrix<T,2,2> operator*=(const T &num) {
		xx *= num; xy *= num;
		yx *= num; yy *= num;
		return *this;
	}
	inline matrix<T,2,2> operator/=(const T& num) {
		xx /= num; xy /= num;
		yx /= num; yy /= num;
		return *this;
	}
	// stream operations
	friend std::ostream& operator<<(std::ostream& out, const matrix<T,2,2>& m) {
		out << m.xx << " " << m.xy << std::endl;
		out << m.yx << " " << m.yy << std::endl;
		return out;
	}
	// comparison operations
	inline const bool operator == (const matrix<T,2,2>& m) const {
		return (	xx == m.xx && xy == m.xy &&
							yx == m.yx && yy == m.yy);
	}
	inline const bool operator != ( const matrix<T,2,2>& m) const {
		return !(m == *this);
	}
	// unary operations
	inline const matrix<T,2,2> operator - () const {
		return matrix<T,2,2>(-xx, -xy,
											-yx, -yy);
	}
	// binary operations
	inline friend matrix<T,2,2> operator+(const matrix<T,2,2>& m1,const matrix<T,2,2>& m2) {
		return matrix<T,2,2>(m1.xx + m2.xx, m1.xy + m2.xy,
											m1.yx + m2.yx, m1.yy + m2.yy);
	}
	inline friend matrix<T,2,2> operator-(const matrix<T,2,2>& m1,const matrix<T,2,2>& m2) {
		return matrix<T,2,2>(m1.xx - m2.xx, m1.xy - m2.xy,
											m1.yx - m2.yx, m1.yy - m2.yy);
	}
	inline const matrix<T,2,2> operator*(const T& num) const {
		return matrix<T,2,2>(xx * num, xy * num,
											yx * num, yy * num);
	}
	friend inline const matrix<T,2,2> operator*( const T &s, const matrix<T,2,2>& m ) {
		return m * s;
	}
	inline const matrix<T,2,2> operator/(const T& num) const {
		float val = (static_cast<T>(1.0)/num);
		return matrix<T,2,2>(xx * val, xy * val,
											yx * val, yy * val);
	}
	 inline friend matrix<T,2,2> operator*(const matrix<T,2,2>& m1, const matrix<T,2,2>& m2 )  {
		return matrix<T,2,2>(m1.xx * m2.xx + m1.xy * m2.yx,
											m1.xx * m2.xy + m1.xy * m2.yy, 
											m1.yx * m2.xx + m1.yy * m2.yx,
											m1.yx * m2.xy + m1.yy * m2.yy);
	}
	inline friend const vectorn<T,2> operator*(const matrix<T,2,2>& m, const vectorn<T,2>& v) {
		return vectorn<T,2>(m.xx * v.x + m.xy * v.y,
											m.yx * v.x + m.yy * v.y);

	}
	// indexing operations
	void row(unsigned int pRow, const vectorn<T,2>& pV) {
		unsigned idx = 2*pRow;
		(*this)[idx]			= pV.x;
		(*this)[idx + 1]	= pV.y;
	}
	void column(unsigned int pColumn, const vectorn<T,2>& pV) {
		(*this)[pColumn]			= pV.x;
		(*this)[pColumn + 2]	= pV.y;
	}
	inline const vectorn<T,2> row(int idx) {
		return vectorn<T,2>(*(&xx + 2*idx),*(&xx + 2*idx +1));
	}
	inline const vectorn<T,2> column(int idx) {
		return vectorn<T,2>(*(&xx + idx),*(&xx + idx+ 2));
	}
	// cast operations
	operator T*() {
		return &xx;
	}
	operator const T*() const {
		return &xx;
	}
};


}

#endif

