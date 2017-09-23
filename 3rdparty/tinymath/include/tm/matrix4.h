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

#ifndef MATRIX4_H
#define MATRIX4_H

namespace tmath{

template<typename T, int N, int M>
class matrix;

/**
	@author 		Cengiz Terzibas
*/

template<typename T>
class matrix<T,4,4> {
public:
	typedef T value_type;
	T xx, xy, xz, xw;
	T yx, yy, yz, yw;
	T zx, zy, zz, zw;
	T wx, wy, wz, ww;
	// constructors
	matrix<T,4,4>()	:	xx(static_cast<T>(0.0)),
									xy(static_cast<T>(0.0)),
									xz(static_cast<T>(0.0)),
									xw(static_cast<T>(0.0)),
									yx(static_cast<T>(0.0)),
									yy(static_cast<T>(0.0)),
									yz(static_cast<T>(0.0)),
									yw(static_cast<T>(0.0)),
									zx(static_cast<T>(0.0)),
									zy(static_cast<T>(0.0)),
									zz(static_cast<T>(0.0)),
									zw(static_cast<T>(0.0)),
									wx(static_cast<T>(0.0)),
									wy(static_cast<T>(0.0)),
									wz(static_cast<T>(0.0)),
									ww(static_cast<T>(0.0)) {
	}
	matrix<T,4,4>(	T a0,		T a1,			T a2, 		T a3,
									T a4,		T a5,			T a6, 		T a7,
									T a8,		T a9,			T a10, 		T a11,
									T a12,	T a13,		T a14, 		T a15)
					 		: 	xx(a0),		xy(a1),		xz(a2),		xw(a3),
									yx(a4),		yy(a5),		yz(a6),		yw(a7),
									zx(a8),		zy(a9),		zz(a10),	zw(a11),
									wx(a12),	wy(a13),	wz(a14),	ww(a15) {
	}
	matrix<T,4,4>(const matrix<T,4,4>& m) {
		xx = m.xx;	xy =	m.xy; xz =	m.xz;	xw = m.xw;
		yx = m.yx;	yy =	m.yy; yz =	m.yz;	yw = m.yw;
		zx = m.zx;	zy =	m.zy; zz =	m.zz;	zw = m.zw;
		wx = m.wx;	wy = 	m.wy;	wz = 	m.wz;	ww = m.ww;
	}
	matrix<T,4,4>(const T* m) {
		xx = m[0];	xy = m[1];	xz = m[2];	xw = m[3];
		yx = m[4];	yy = m[5];	yz = m[6];	yw = m[7];
		zx = m[8];	zy = m[9];	zz = m[10]; zw = m[11];
		wx = m[12]; wy = m[13]; wz = m[14]; ww = m[15];
	}
	matrix<T,4,4>(const matrix<T,3,3>& m) {
		xx = m.xx;	xy =	m.xy; xz =	m.xz;	xw = static_cast<T>(0.0);
		yx = m.yx;	yy =	m.yy; yz =	m.yz;	yw = static_cast<T>(0.0);
		zx = m.zx;	zy =	m.zy; zz =	m.zz;	zw = static_cast<T>(0.0);
		wx = static_cast<T>(0.0);		wy = static_cast<T>(0.0); 	wz = static_cast<T>(0.0); 	ww = static_cast<T>(1.0);
	}
	// assignment operations
	inline matrix<T,4,4> operator+=(const matrix<T,4,4>& m) {
		xx += m.xx; xy += m.xy; xz += m.xz; xw += m.xw;
		yx += m.yx; yy += m.yy; yz += m.yz; yw += m.yw;
		zx += m.zx; zy += m.zy; zz += m.zz; zw += m.zw;
		wx += m.wx; wy += m.wy; wz += m.wz; ww += m.ww;
		return *this;
	}
	inline matrix<T,4,4> operator-=(matrix<T,4,4>& m) {
		xx -= m.xx; xy -= m.xy; xz -= m.xz; xw -= m.xw;
		yx -= m.yx; yy -= m.yy; yz -= m.yz; yw -= m.yw;
		zx -= m.zx; zy -= m.zy; zz -= m.zz; zw -= m.zw;
		wx -= m.wx; wy -= m.wy; wz -= m.wz; ww -= m.ww;
		return *this;
	}
	inline matrix<T,4,4> operator*=(const T& num) {
		xx *= num; xy *= num; xz *= num; xw *= num;
		yx *= num; yy *= num; yz *= num; yw *= num;
		zx *= num; zy *= num; zz *= num; zw *= num;
		wx *= num; wy *= num; wz *= num; ww *= num;
		return *this;
	}
	inline matrix<T,4,4> operator/=(const T& num) {
		xx /= num; xy /= num; xz /= num; xw /= num;
		yx /= num; yy /= num; yz /= num; yw /= num;
		zx /= num; zy /= num; zz /= num; zw /= num;
		wx /= num; wy /= num; wz /= num; ww /= num;
		return *this;
	}
	// stream operations
	friend std::ostream& operator<<(std::ostream& out, const matrix<T,4,4>& m) {
		out << m.xx << " " << m.xy << " " << m.xz << " " << m.xw << std::endl;
		out << m.yx << " " << m.yy << " " << m.yz << " " << m.yw << std::endl;
		out << m.zx << " " << m.zy << " " << m.zz << " " << m.zw << std::endl;
		out << m.wx << " " << m.wy << " " << m.wz << " " << m.ww << std::endl;
		return out;
	}
	// comparison operations
	inline const bool operator == (const matrix<T,4,4>& m) const {
		return (	xx == m.xx && xy == m.xy && xz == m.xz && xw == m.xw &&
							yx == m.yx && yy == m.yy && yz == m.yz && yw == m.yw &&
							zx == m.zx && zy == m.zy && zz == m.zz && zw == m.zw &&
							wx == m.wx && wy == m.wy && wz == m.wz && ww == m.ww);
	}
	inline const bool operator != ( const matrix<T,4,4>& m) const {
		return !(m == *this);
	}
	// unary operations
	inline const matrix<T,4,4> operator - () const {
		return matrix<T,4,4>(-xx, -xy, -xz, -xw,
											-yx, -yy, -yz, -yw,
											-zx, -zy, -zz, -zw,
											-wx, -wy, -wz, -ww);
	}
	// binary operations
	inline friend matrix<T,4,4> operator+(const matrix<T,4,4>& m1,const matrix<T,4,4>& m2) {
		return matrix<T,4,4>(m1.xx + m2.xx, m1.xy + m2.xy, m1.xz + m2.xz, m1.xw + m2.xw,
											m1.yx + m2.yx, m1.yy + m2.yy, m1.yz + m2.yz, m1.yw + m2.yw,
											m1.zx + m2.zx, m1.zy + m2.zy, m1.zz + m2.zz, m1.zw + m2.zw,
											m1.wx + m2.wx, m1.wy + m2.wy, m1.wz + m2.wz, m1.ww + m2.ww);
	}
	inline friend matrix<T,4,4> operator-(const matrix<T,4,4>& m1,const matrix<T,4,4>& m2) {
		return matrix<T,4,4>(m1.xx - m2.xx, m1.xy - m2.xy, m1.xz - m2.xz, m1.xw - m2.xw,
											m1.yx - m2.yx, m1.yy - m2.yy, m1.yz - m2.yz, m1.yw - m2.yw,
											m1.zx - m2.zx, m1.zy - m2.zy, m1.zz - m2.zz, m1.zw - m2.zw,
											m1.wx - m2.wx, m1.wy - m2.wy, m1.wz - m2.wz, m1.ww - m2.ww);
	}
	inline const matrix<T,4,4> operator*(const T& num) const {
		return matrix<T,4,4>(	xx * num, xy * num, xz * num, xw * num,
												yx * num, yy * num, yz * num, yw * num,
												zx * num, zy * num, zz * num, zw * num,
												wx * num, wy * num, wz * num, ww * num);
	}
	friend inline const matrix<T,4,4> operator*( const T &s, const matrix<T,4,4> &m ) {
		return m * s;
	}
	friend inline matrix<T,4,4> operator*(const matrix<T,4,4>& m1, const matrix<T,4,4>& m2 ) {
		return matrix<T,4,4>( 	m1.xx * m2.xx + m1.xy * m2.yx + m1.xz * m2.zx + m1.xw * m2.wx,
												m1.xx * m2.xy + m1.xy * m2.yy + m1.xz * m2.zy + m1.xw * m2.wy,
												m1.xx * m2.xz + m1.xy * m2.yz + m1.xz * m2.zz + m1.xw * m2.wz,
												m1.xx * m2.xw + m1.xy * m2.yw + m1.xz * m2.zw + m1.xw * m2.ww,

												m1.yx * m2.xx + m1.yy * m2.yx + m1.yz * m2.zx + m1.yw * m2.wx,
												m1.yx * m2.xy + m1.yy * m2.yy + m1.yz * m2.zy + m1.yw * m2.wy,
												m1.yx * m2.xz + m1.yy * m2.yz + m1.yz * m2.zz + m1.yw * m2.wz,
												m1.yx * m2.xw + m1.yy * m2.yw + m1.yz * m2.zw + m1.yw * m2.ww,

												m1.zx * m2.xx + m1.zy * m2.yx + m1.zz * m2.zx + m1.zw * m2.wx,
												m1.zx * m2.xy + m1.zy * m2.yy + m1.zz * m2.zy + m1.zw * m2.wy,
												m1.zx * m2.xz + m1.zy * m2.yz + m1.zz * m2.zz + m1.zw * m2.wz,
												m1.zx * m2.xw + m1.zy * m2.yw + m1.zz * m2.zw + m1.zw * m2.ww,

												m1.wx * m2.xx + m1.wy * m2.yx + m1.wz * m2.zx + m1.ww * m2.wx,
												m1.wx * m2.xy + m1.wy * m2.yy + m1.wz * m2.zy + m1.ww * m2.wy,
												m1.wx * m2.xz + m1.wy * m2.yz + m1.wz * m2.zz + m1.ww * m2.wz,
												m1.wx * m2.xw + m1.wy * m2.yw + m1.wz * m2.zw + m1.ww * m2.ww);
	}
	inline friend const vectorn<T,4> operator*(const matrix<T,4,4>& m, const vectorn<T,4>& v) {
		return vectorn<T,4>(v.x*m.xx + v.y*m.xy + v.z*m.xz + v.w*m.xw,
											v.x*m.yx + v.y*m.yy + v.z*m.yz + v.w*m.yw,
											v.x*m.zx + v.y*m.zy + v.z*m.zz + v.w*m.zw,
											v.x*m.wx + v.y*m.wy + v.z*m.wz + v.w*m.ww);

	}
	inline const matrix<T,4,4> operator/(const T& num) const {
		T val = 1.0/num;
		return matrix<T,4,4>(xx * val, xy * val, xz * val, xw * val,
											yx * val, yy * val, yz * val, yw * val,
											zx * val, zy * val, zz * val, zw * val,
											wx * val, wy * val, wz * val, ww * val);
	}

	// indexing operations
	void row(unsigned int pRow, const vectorn<T,4>& pV) {
		unsigned idx = 4*pRow;
		(*this)[idx]			= pV.x;
		(*this)[idx + 1]	= pV.y;
		(*this)[idx + 2]	= pV.z;
		(*this)[idx + 3]	= pV.w;
	}
	vectorn<T,4> row(unsigned int pRow) {
		unsigned idx = 4*pRow;
		return vectorn<T,4>((*this)[idx],(*this)[idx+1],(*this)[idx+2],(*this)[idx+3]);
	}
	void column(unsigned int pColumn, const vectorn<T,4>& pV) {
		(*this)[pColumn]	= pV.x;
		(*this)[pColumn + 4]	= pV.y;
		(*this)[pColumn + 8]	= pV.z;
		(*this)[pColumn + 12]	= pV.w;
	}
	vectorn<T,4> column(unsigned int pColumn) {
		return vectorn<T,4>((*this)[pColumn],(*this)[pColumn+4],(*this)[pColumn+8],(*this)[pColumn+12]);
	}

	inline const vectorn<T,4> row(int idx) {
		return vectorn<T,4>(*(&xx + 4*idx),*(&xx + 4*idx +1),*(&xx + 4*idx +2),*(&xx + 4*idx +3));
	}
	inline const vectorn<T,4> column(int idx) {
		return vectorn<T,4>(*(&xx + idx),*(&xx + idx+ 4),*(&xx + idx + 8),*(&xx + idx + 12));
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

