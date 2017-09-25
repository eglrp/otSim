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

#ifndef MATRIX3_H
#define MATRIX3_H

namespace tmath{

template<typename T, int N, int M>
class matrix;

template<typename T>
inline vectorn<T,3>& normalize(vectorn<T,3>& v);

//template<typename T>
//class quaternion;

/**
	@author 		Cengiz Terzibas
*/

template<typename T>
class matrix<T,3,3> {
public:
	typedef T value_type;
	T xx, xy, xz;
	T yx, yy, yz;
	T zx, zy, zz;
	// constructors
	matrix<T,3,3>()	:	xx(static_cast<T>(0.0)),
									xy(static_cast<T>(0.0)),
									xz(static_cast<T>(0.0)),
									yx(static_cast<T>(0.0)),
									yy(static_cast<T>(0.0)),
									yz(static_cast<T>(0.0)),
									zx(static_cast<T>(0.0)),
									zy(static_cast<T>(0.0)),
									zz(static_cast<T>(0.0)) {
	}
	matrix<T,3,3>(	T a0, T a1, T a2,
							T a3,	T a4, T a5,
							T a6, T a7, T a8)
						:	xx(a0), xy(a1), xz(a2),
							yx(a3), yy(a4), yz(a5),
							zx(a6), zy(a7), zz(a8) {
	}
	matrix<T,3,3>(const T* m) {
		xx = m[0];	xy = m[1];	xz = m[2];
		yx = m[4];	yy = m[5];	yz = m[6];
		zx = m[8];	zy = m[9];	zz = m[10];
	}
	matrix<T,3,3>(const matrix<T,3,3>& m) {
		xx = m.xx; xy = m.xy; xz = m.xz;
		yx = m.yx; yy = m.yy; yz = m.yz;
		zx = m.zx; zy = m.zy; zz = m.zz;
	}

	/// Converts a quaternion into a matrix<T,3,3>
	matrix<T, 3, 3>(const quaternion<T>& q) {
		xx = static_cast<T>(1.0) - static_cast<T>(2.0) * (q.y * q.y + q.z * q.z);
		xy = static_cast<T>(2.0) * (q.x * q.y - q.z * q.w);
		xz = static_cast<T>(2.0) * (q.x * q.z + q.y * q.w);

		yx = static_cast<T>(2.0) * (q.x * q.y + q.z * q.w);
		yy = static_cast<T>(1.0) - static_cast<T>(2.0) * (q.x * q.x + q.z * q.z);
		yz = static_cast<T>(2.0) * (q.z * q.y - q.x * q.w);

		zx = static_cast<T>(2.0) * (q.x * q.z - q.y * q.w);
		zy = static_cast<T>(2.0) * (q.y * q.z + q.x * q.w);
		zz = static_cast<T>(1.0) - static_cast<T>(2.0) * (q.x * q.x + q.y * q.y);
	}

	// assignment operations
	inline matrix<T,3,3> operator+=(const matrix<T,3,3>& m) {
		xx += m.xx; xy += m.xy; xz += m.xz;
		yx += m.yx; yy += m.yy; yz += m.yz;
		zx += m.zx; zy += m.zy; zz += m.zz;
		return *this;
	}
	inline matrix<T,3,3> operator-=(const matrix<T,3,3>& m) {
		xx -= m.xx; xy -= m.xy; xz -= m.xz;
		yx -= m.yx; yy -= m.yy; yz -= m.yz;
		zx -= m.zx; zy -= m.zy; zz -= m.zz;
		return *this;
	}
	inline matrix<T,3,3> operator*=(const T &num) {
		xx *= num; xy *= num; xz *= num;
		yx *= num; yy *= num; yz *= num;
		zx *= num; zy *= num; zz *= num;
		return *this;
	}
	inline matrix<T,3,3> operator/=(const T& num) {
		xx /= num; xy /= num; xz /= num;
		yx /= num; yy /= num; yz /= num;
		zx /= num; zy /= num; zz /= num;
		return *this;
	}
	// stream operations
	friend std::ostream& operator<<(std::ostream& out, const matrix<T,3,3>& m) {
		out << m.xx << " " << m.xy << " " << m.xz << std::endl;
		out << m.yx << " " << m.yy << " " << m.yz << std::endl;
		out << m.zx << " " << m.zy << " " << m.zz << std::endl;
		return out;
	}
	// comparison operations
	inline const bool operator == (const matrix<T,3,3>& m) const {
		return (	xx == m.xx && xy == m.xy && xz == m.xz &&
							yx == m.yx && yy == m.yy && yz == m.yz &&
							zx == m.zx && zy == m.zy && zz == m.zz);
	}
	inline const bool operator != ( const matrix<T,3,3>& m) const {
		return !(m == *this);
	}
	// unary operations
	inline const matrix<T,3,3> operator - () const {
		return matrix<T,3,3>(-xx, -xy, -xz,
											-yx, -yy, -yz,
											-zx, -zy, -zz);
	}
	// binary operations
	inline friend matrix<T,3,3> operator+(const matrix<T,3,3>& m1,const matrix<T,3,3>& m2) {
		return matrix<T,3,3>(m1.xx + m2.xx, m1.xy + m2.xy, m1.xz + m2.xz,
											m1.yx + m2.yx, m1.yy + m2.yy, m1.yz + m2.yz,
											m1.zx + m2.zx, m1.zy + m2.zy, m1.zz + m2.zz);
	}
	inline friend matrix<T,3,3> operator-(const matrix<T,3,3>& m1,const matrix<T,3,3>& m2) {
		return matrix<T,3,3>(m1.xx - m2.xx, m1.xy - m2.xy, m1.xz - m2.xz,
											m1.yx - m2.yx, m1.yy - m2.yy, m1.yz - m2.yz,
											m1.zx - m2.zx, m1.zy - m2.zy, m1.zz - m2.zz);
	}
	inline const matrix<T,3,3> operator*(const T& num) const {
		return matrix<T,3,3>(xx * num, xy * num, xz * num,
											yx * num, yy * num, yz * num,
											zx * num, zy * num, zz * num);
	}
	friend inline const matrix<T,3,3> operator*( const T &s, const matrix<T,3,3>& m ) {
		return m * s;
	}
	inline friend matrix<T,3,3> operator*(const matrix<T,3,3>& m1, const matrix<T,3,3>& m2 )  {
		return matrix<T,3,3>(m1.xx * m2.xx + m1.xy * m2.yx + m1.xz * m2.zx,
											m1.xx * m2.xy + m1.xy * m2.yy + m1.xz * m2.zy,
											m1.xx * m2.xz + m1.xy * m2.yz + m1.xz * m2.zz,
											m1.yx * m2.xx + m1.yy * m2.yx + m1.yz * m2.zx,
											m1.yx * m2.xy + m1.yy * m2.yy + m1.yz * m2.zy,
											m1.yx * m2.xz + m1.yy * m2.yz + m1.yz * m2.zz,
											m1.zx * m2.xx + m1.zy * m2.yx + m1.zz * m2.zx,
											m1.zx * m2.xy + m1.zy * m2.yy + m1.zz * m2.zy,
											m1.zx * m2.xz + m1.zy * m2.yz + m1.zz * m2.zz);
	}
	inline friend const vectorn<T,3> operator*(const matrix<T,3,3>& m, const vectorn<T,3>& v) {
		return vectorn<T,3>(v.x*m.xx + v.y*m.xy + v.z*m.xz,
											v.x*m.yx + v.y*m.yy + v.z*m.yz,
											v.x*m.zx + v.y*m.zy + v.z*m.zz);

	}
	inline const matrix<T,3,3> operator/(const T& num) const {
		float val = (static_cast<T>(1.0)/num);
		return matrix<T,3,3>(xx * val, xy * val, xz * val,
											yx * val, yy * val, yz * val,
											zx * val, zy * val, zz * val);
	}
	// indexing operations
	void row(unsigned int pRow, const vectorn<T,3>& pV) {
		unsigned idx = 4*pRow;
		(*this)[idx]			= pV.x;
		(*this)[idx + 1]	= pV.y;
		(*this)[idx + 2]	= pV.z;
	}
	void column(unsigned int pColumn, const vectorn<T,3>& pV) {
		(*this)[pColumn]			= pV.x;
		(*this)[pColumn + 3]	= pV.y;
		(*this)[pColumn + 6]	= pV.z;
	}
	inline const vectorn<T,3> row(int idx) {
		return vectorn<T,3>(*(&xx + 3*idx),*(&xx + 3*idx +1),*(&xx + 3*idx +2));
	}
	inline const vectorn<T,3> column(int idx) {
		return vectorn<T,3>(*(&xx + idx),*(&xx + idx+ 3),*(&xx + idx + 6));
	}
	inline void normalize_column() {
		vectorn<T,3> v0,v1,v2, u0, u1, u2;
		v0 = vectorn<T,3>(xx, yx,zx);
		v1 = vectorn<T,3>(xy, yy,zy);
		v2 = vectorn<T,3>(xz, yz,zz);
		u0 = v0;
		normalize(u0);
		u1 = v1 - ((v1*u0)/(u0*u0))*u0;
		normalize(u1);
		u2 = v2 - ((v2*u0)/(u0*u0))*u0 - ((v2*u1)/(u1*u1))*u1;
		normalize(u2);
		column(0, u0);
		column(1, u0);
		column(2, u0);
	}
	// cast operations
	operator T*() {
		return &xx;
	}
	operator const T*() const {
		return &xx;
	}

	/// Transpose the matrix<T,3,3>
	const matrix<T, 3, 3> transp() const {
		return matrix<T, 3, 3>(xx, yx, zx,
										xy, yy, zy,
										xz, yz, zz);
		//matrix<T, 3, 3> transpM();
		//for (int i = 0; i < 3; i++) {
		//	for (int j = 0; j < 3; j++) {
		//		std::copy(&this->[i + j * 3], &this->[i + j * 3] + 1, &transpM[j + i * 3]);
		//	}
		//}
		//return transpM;
	}

	/// Transpose the matrix<T,3,3>
	const matrix<T, 3, 3> transpose() const {
		return transp();
	}

	/// Make a null matrix<T,3,3>
	inline void null() {
		xx = static_cast<T>(0.0); xy = static_cast<T>(0.0); xz = static_cast<T>(0.0);
		yx = static_cast<T>(0.0); yy = static_cast<T>(0.0); yz = static_cast<T>(0.0);
		zx = static_cast<T>(0.0); zy = static_cast<T>(0.0); zz = static_cast<T>(0.0);
	}

	/// Make a identity matrix<T,3,3>
	inline void identity() {
		xx = static_cast<T>(1.0); xy = static_cast<T>(0.0); xz = static_cast<T>(0.0);
		yx = static_cast<T>(0.0); yy = static_cast<T>(1.0); yz = static_cast<T>(0.0);
		zx = static_cast<T>(0.0); zy = static_cast<T>(0.0); zz = static_cast<T>(1.0);
	}

	/// Calculate the determinant of a matrix
	T det() const {
		T det;
		det = this->[0] * (yy * zz - yz * zy)
			- this->[3] * (zz * xy - zy * xz)
			+ this->[7] * (yz * xy - yy * xz);
		return det;
	}

	/// Calculate the determinant of a matrix
	T determinant() const {
		return det();
	}

	/// Calculate the inverse of a matrix
	const matrix<T, 3, 3> inv() const {
		T detinv;
		T detval = det();
		if (fabs(detval) < static_cast<T>(0.0005)) {
			return matrix<T, 3, 3>();
		}
		detinv = static_cast<T>(1.0) / detval;
		return matrix<T, 3, 3>((zz*yy - zy*yz)*detinv, -(zz*xy - zy*xz)*detinv,  (yz*xy - yy*xz)*detinv,
									 -(zz*yx - zx*yz)*detinv,  (zz*xx - zx*xz)*detinv, -(yz*xx - yx*xz)*detinv,
									  (zy*yx - zx*yy)*detinv, -(zy*xx - zx*xy)*detinv,  (yy*xx - yx*xy)*detinv);
	}

	/// Calculate the inverse of a matrix
	const matrix<T, 3, 3> inverse() const {
		return inv();
	}

};


}

#endif

