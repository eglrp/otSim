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


#ifndef TM_H
#define TM_H

#include <iostream>
#include <cmath>
#include <cfloat>
#include <tm/vector2.h>
#include <tm/vector3.h>
#include <tm/vector4.h>
#include <tm/vectorn.h>
#include <tm/quaternion.h>
#include <tm/matrix2.h>
#include <tm/matrix3.h>
#include <tm/matrix4.h>
#include <tm/matrix.h>

namespace tmath{

// If stream operator for the different classes
template<typename T>
std::ifstream& operator>>(std::ifstream& in, tmath::vectorn<T,2> &v) {
	in >> v.x >>  v.y; return in;
}
template<typename T>
std::ifstream& operator>>(std::ifstream& in, tmath::vectorn<T,3> &v) {
	in >> v.x >>  v.y >> v.z; return in;
}
template<typename T>
std::ifstream& operator>>(std::ifstream& in, tmath::vectorn<T,4> &v) {
	in >> v.x >>  v.y >> v.z >> v.w; return in;
}
template<typename T>
std::ifstream& operator>>(std::ifstream& in, tmath::matrix<T,3,3>& m) {
	in >> m.xx; in >> m.xy; in >> m.xz;
	in >> m.yx; in >> m.yy; in >> m.yz;
	in >> m.zx; in >> m.zy; in >> m.zz;
	return in;
}
template<typename T>
std::ifstream& operator>>(std::ifstream& in, tmath::quaternion<T> &q) {
	in >> q.x >>  q.y >> q.z >> q.w; return in;
}
template<typename T>
std::ifstream& operator>>(std::ifstream& in, tmath::matrix<T,4,4>& m) {
	in >> m.xx; in >> m.xy; in >> m.xz; in >> m.xw;
	in >> m.yx; in >> m.yy; in >> m.yz; in >> m.yw;
	in >> m.zx; in >> m.zy; in >> m.zz; in >> m.zw;
	in >> m.wx; in >> m.wy; in >> m.wz; in >> m.ww;
	return in;
}

/// Calculates the cross product of two vectorn<T,3>
template<typename T>
const tmath::vectorn<T,3> cross(const tmath::vectorn<T,3>& v1,const tmath::vectorn<T,3>& v2) {
	return tmath::vectorn<T,3>((v1.y * v2.z) - (v1.z * v2.y),
										(v1.z * v2.x) - (v1.x * v2.z),
										(v1.x * v2.y) - (v1.y * v2.x));

}
/// Calculates the cross product of two vectorn<T,3>
template<typename T>
void cross(const tmath::vectorn<T,3>& v1,const tmath::vectorn<T,3>& v2, tmath::vectorn<T,3>& rv) {
	rv.x = (v1.y * v2.z) - (v1.z * v2.y);
	rv.y = (v1.z * v2.x) - (v1.x * v2.z);
	rv.z = (v1.x * v2.y) - (v1.y * v2.x);
}

/// Returns a reflected vector, based on the given original vector and the surface normal vector
template<typename T>
const vectorn<T, 2> reflect(const vectorn<T, 2>& origVec, const vectorn<T, 2>& normalVec)
{
	vectorn<T, 2> unitNormal = normalVec.unitVector();
	return (unitNormal * -2 * origVec->dot(unitNormal) + origVec);
}

/// Returns a reflected vector, based on the given original vector and the surface normal vector
template<typename T>
const vectorn<T, 3> reflect(const vectorn<T, 3>& origVec, const vectorn<T, 3>& normalVec)
{
	vectorn<T, 3> unitNormal = normalVec.unitVector();
	return (unitNormal * -2 * origVec->dot(unitNormal) + origVec);
}

/// Calculates the dot product of two vectorn<T,2>
template<typename T>
inline T dot(const vectorn<T, 2>& v1, const vectorn<T, 2>& v2) {
	return static_cast<T>(v1.x*v2.x + v1.y*v2.y);
}

/// Calculates the dot product of two vectorn<T,3>
template<typename T>
inline T dot(const vectorn<T, 3>& v1, const vectorn<T, 3>& v2) {
	return static_cast<T>(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

/// Calculates the dot product of two vectorn<T,4>
template<typename T>
inline T dot(const vectorn<T, 4>& v1, const vectorn<T, 4>& v2) {
	return static_cast<T>(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w);
}


/// Calculates the distance between two vectorn<T,2>
template<typename T>
inline T distance(const vectorn<T, 2>& v1, const vectorn<T, 2>& v2) {
	return ((v1 - v2).len());
}
template<typename T>
inline T distance2(const vectorn<T, 2>& v1, const vectorn<T, 2>& v2) {
	return ((v1 - v2).norm());
}
/// Calculates the distance between two vectorn<T,3>
template<typename T>
inline T distance(const vectorn<T, 3>& v1, const vectorn<T, 3>& v2) {
	return ((v1 - v2).len());
}
template<typename T>
inline T distance2(const vectorn<T, 3>& v1, const vectorn<T, 3>& v2) {
	return ((v1 - v2).norm());
}

template<typename T>
inline T angleBetween(const vectorn<T, 2>& v1, const vectorn<T, 2>& v2) {
	return acos(v1.unitVector().dot(v2.unitVector()));
}
template<typename T>
inline T angleBetween(const vectorn<T, 3>& v1, const vectorn<T, 3>& v2) {
	return acos(v1.unitVector().dot(v2.unitVector()));
}

/// conjugate the quaternion<T>
template<typename T>
inline quaternion<T> conj(const quaternion<T>& q) {
	tmath::quaternion<T> qres(q);
	qres.x	*= static_cast<T>(-1.0);
	qres.y *= static_cast<T>(-1.0);
	qres.z	*= static_cast<T>(-1.0);
	return qres;
}
/// invert the quaternion<T>
template<typename T>
inline quaternion<T> inv(const quaternion<T>& q) {
	tmath::quaternion<T> qres(q);
	T s = len(qres);
	if( s > static_cast<T>(0.0)) {
		s = static_cast<T>(1.0)/s;
		qres*=s;
		return conj(qres);
	}
	return quaternion<T>(0.0, 0.0, 0.0, 0.0);
}
/// Make an identity quaternion<T>
template<typename T>
inline void identity(quaternion<T>& q) {
	q.w = 1.0; q.x = 0.0; q.y = 0.0;q.z = 0.0;
}

/// Calculates the norm of a quaternion
template<class T>
inline T norm(const tmath::quaternion<T>& q) {
	return (q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
}
/// Calculates the length of a quaternion
template<class T>
inline T len(const tmath::quaternion<T>& q) {
	return static_cast<T>(sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z));
}
/// Normalize the quaternion
template<class T>
tmath::quaternion<T>& normalize(tmath::quaternion<T>& q) {
	T length = q.len();
	if (fabs(length - static_cast<T>(1.000)) < static_cast<T>(1e-10)) {
		//don't do anything, already normalized
	}
	else if (length > static_cast<T>(0.0)) {
		T normFactor = static_cast<T>(1.0) / length;
		q.w *= normFactor;
		q.x *= normFactor;
		q.y *= normFactor;
		q.z *= normFactor;
	}
	else {
		q.w = static_cast<T>(1.0);
		q.x = q.y = q.z = static_cast<T>(0.0);
	}

	return q;
}

/// Return Normalized copy of the quaternion
template<class T>
quaternion<T> normalized(const tmath::quaternion<T>& q) {
	quaternion<T> rq(q);
	rq.normalize();
	return rq;
}

/// Converts a quaternion into a matrix4x4<T>
template<typename T>
inline const tmath::matrix<T,4,4>& conv( const tmath::quaternion<T>& q, tmath::matrix<T,4,4>& m) {
	m.xx = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.y * q.y + q.z * q.z );
	m.xy = static_cast<T>(2.0) * (q.x * q.y - q.z * q.w);
	m.xz = static_cast<T>(2.0) * (q.x * q.z + q.y * q.w);
	m.xw = static_cast<T>(0.0);

	m.yx = static_cast<T>(2.0) * ( q.x * q.y + q.z * q.w );
	m.yy = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.x * q.x + q.z * q.z );
	m.yz = static_cast<T>(2.0) * (q.z * q.y - q.x * q.w );
	m.yw = static_cast<T>(0.0);

	m.zx = static_cast<T>(2.0) * ( q.x * q.z - q.y * q.w );
	m.zy = static_cast<T>(2.0) * ( q.y * q.z + q.x * q.w );
	m.zz = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.x * q.x + q.y * q.y );
	m.zw = static_cast<T>(0.0);

	m.wx = static_cast<T>(0.0);
	m.wy = static_cast<T>(0.0);
	m.wz = static_cast<T>(0.0);
	m.ww = static_cast<T>(1.0);
	return m;
}
/// Calculate the normal of a vector2
template<typename T>
inline T norm(const vectorn<T,2>& v) {
	return static_cast<T>(v.x*v.x + v.y*v.y);
}
/// Calculate the length of a vector2
template<typename T>
inline T len(const vectorn<T,2>& v) {
	return static_cast<T>(sqrt(v.x*v.x + v.y*v.y));
}
/// Normalize a vector2 (in place)
template<typename T>
inline vectorn<T, 2>& normalize(vectorn<T, 2>& v) {
	T length = len(v);
	if (length > 0) {
		T value = static_cast<T>(1.0) / length;
		v.x *= value; v.y *= value;
	}
	return v;
}
/// Return a Normalized copy of a vector2
template<typename T>
inline const vectorn<T, 2> normalized(const vectorn<T, 2>& v) {
	vectorn<T, 2> rv(v);
	return normalize(rv);
}
/// Convert vectorn<T,3> to vector2
template<typename T>
inline void conv(const vectorn<T,3>& v1, vectorn<T,2>& v2) {
	v2.x = v1.x;
	v2.y = v1.y;
}
/// Convert vectorn to vector2
template<typename T>
inline void conv(const vectorn<T,4>& v1, vectorn<T,2>& v2) {
	v2.x = v1.x;
	v2.y = v1.y;
}
/// Convert vectorn to vector2
template<typename T>
inline void conv(const vectorn<T,2>& v1, vectorn<T,3>& v2) {
	v2.x = v1.x;
	v2.y = v1.y;
	v2.z = 0.0f;
}
/// Convert vectorn to vector2
template<typename T>
inline void conv(const vectorn<T,2>& v1, vectorn<T,4>& v2) {
	v2.x = v1.x;
	v2.y = v1.y;
	v2.z = 0.0f;
	v2.w = 0.0f;
}
/// Calculate the normal of a vectorn<T,3>
template<typename T>
inline T norm(const vectorn<T,3>& v) {
	return static_cast<T>(v.x*v.x + v.y*v.y + v.z*v.z);
}
/// Calculate the length of a vectorn<T,3>
template<typename T>
inline T len(const vectorn<T,3>& v) {
	return static_cast<T>(sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
}
/// Normalize a vectorn<T,3>
template<typename T>
inline vectorn<T, 3>& normalize(vectorn<T, 3>& v) {
	T length = len(v);
	if (length > 0) {
		T value = static_cast<T>(1.0) / length;
		v.x *= value; v.y *= value; v.z *= value;
	}
	return v;
}
/// Return a Normalized copy of a vectorn<T,3>
template<typename T>
inline const vectorn<T, 3> normalized(const vectorn<T, 3>& v) {
	vectorn<T, 3> rv(v);
	return normalize(rv);
}
/// Calculate the normal of a vectorn
template<typename T>
inline T norm(const vectorn<T,4>& v) {
	return static_cast<T>(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}
/// Convert vectorn to vectorn<T,3>
template<typename T>
inline void conv(const vectorn<T,4>& v1, vectorn<T,3>& v2) {
	v2.x = v1.x;
	v2.y = v1.y;
	v2.z = v1.z;
}
/// Convert vectorn<T,3> to vectorn
template<typename T>
inline void conv(const vectorn<T,3>& v1, vectorn<T,4>& v2) {
	v2.x = v1.x;
	v2.y = v1.y;
	v2.z = v1.z;
	v2.w = 0.0f;
}
/// Calculate the length of a vectorn
template<typename T>
inline T len(const vectorn<T,4>& v) {
	return static_cast<T>(sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w));
}
/// Normalize a vectorn
template<typename T>
inline vectorn<T, 4>& normalize(vectorn<T, 4>& v) {
	T length = len(v);
	if (length > 0) {
		T value = static_cast<T>(1.0) / length;
		v.x *= value; v.y *= value; v.z *= value; v.w *= value;
	}
	return v;
}
/// Return a Normalized copy of a vectorn<T,4>
template<typename T>
inline const vectorn<T, 4> normalized(const vectorn<T, 4>& v) {
	vectorn<T, 4> rv(v);
	return normalize(rv);
}
/// Calculate the normal of a vectorn
template<typename T, int NUM>
inline T norm(const vectorn<T,NUM>& v) {
	T res = static_cast<T>(0.0);
	for(int idx = 0; idx < NUM; ++idx)
		res += v.cmp[idx]*v.cmp[idx];
	return res;
}
/// Calculate the length of a vectorn
template<typename T, int NUM>
inline T len(const vectorn<T,NUM>& v) {
	T res = static_cast<T>(0.0);
	for(int idx = 0; idx < NUM; ++idx)
		res += v.cmp[idx]*v.cmp[idx];
	res = sqrt(res);
	return res;
}
/// Normalize a vectorn
template<typename T, int NUM>
inline const vectorn<T, NUM> normalize(vectorn<T, NUM>& v) {
	T length = len(v);
	if (length > 0) {
		T value = static_cast<T>(1.0) / length;
		for (int idx = 0; idx < NUM; ++idx)
			v.cmp[idx] *= value;
	}
	return v;
}
/// Return a Normalized copy of a vectorn
template<typename T, int NUM>
inline const vectorn<T, NUM> normalized(const vectorn<T, NUM>& v) {
	vectorn<T, NUM> rv(v);
	return normalize(rv);
}



template<class T>
inline T magnitude2(const vectorn<T, 2>& v) {
	return norm(v);
}

template<class T>
inline T magnitude2(const vectorn<T, 3>& v) {
	return norm(v);
}

template<class T>
inline T magnitude2(const vectorn<T, 4>& v) {
	return norm(v);
}

template<typename T, int NUM>
inline T magnitude2(const vectorn<T, NUM>& v) {
	return norm(v);
}


template<class T>
inline T magnitude(const vectorn<T, 2>& v) {
	return len(v);
}

template<class T>
inline T magnitude(const vectorn<T, 3>& v) {
	return len(v);
}

template<class T>
inline T magnitude(const vectorn<T, 4>& v) {
	return len(v);
}

template<typename T, int NUM>
inline T magnitude(const vectorn<T, NUM>& v) {
	return len(v);
}





template<typename T>
inline quaternion<T> conjugate(const quaternion<T>& q) {
	return conj(q);
}

template<typename T>
inline quaternion<T> inverse(const quaternion<T>& q) {
	return inv(q);
}







/// Calculate the determinant of a matrix
template<typename T>
T det(const tmath::matrix<T,3,3>& m) {
	T det;
	det = m[0] * ( m.yy * m.zz - m.yz * m.zy )
			- m[3] * ( m.zz * m.xy - m.zy * m.xz )
			+ m[7] * ( m.yz * m.xy - m.yy * m.xz );
	return det;
}
//// Calculate the inverse of a matrix
template<typename T>
const tmath::matrix<T,3,3> inv(const tmath::matrix<T,3,3>& m) {
	T detinv;
	T detval = det( m );
	if ( fabs( detval ) < static_cast<T>(0.0005) ) {
		return tmath::matrix<T,3,3>();
	}
	detinv = static_cast<T>(1.0)/detval;
	return tmath::matrix<T,3,3>((m.zz*m.yy - m.zy*m.yz)*detinv,		-(m.zz*m.xy - m.zy*m.xz)*detinv,	(m.yz*m.xy - m.yy*m.xz)*detinv,
										-(m.zz*m.yx - m.zx*m.yz)*detinv,	(m.zz*m.xx - m.zx*m.xz)*detinv,		-(m.yz*m.xx - m.yx*m.xz)*detinv,
										(m.zy*m.yx - m.zx*m.yy)*detinv,		-(m.zy*m.xx - m.zx*m.xy)*detinv,	(m.yy*m.xx - m.yx*m.xy)*detinv);
}
/// Calculate the inverse of a matrix
template<typename T>
const tmath::matrix<T,3,3>& inv(const tmath::matrix<T,3,3>& m, tmath::matrix<T,3,3>& mdst) {
	T detinv;
	T det = det( m );
	if ( fabs( det ) < static_cast<T>(0.0005) ) {
		mdst = tmath::matrix<T,3,3>();
		return mdst;
	}
	detinv = static_cast<T>(1.0)/det;
	mdst[0] = (m.zz*m.yy - m.zy*m.yz)*detinv;
	mdst[1] = -(m.zz*m.xy - m.zy*m.xz)*detinv;
	mdst[2] = (m.yz*m.xy - m.yy*m.xz)*detinv;
	mdst[3] = -(m.zz*m.yx - m.zx*m.yz)*detinv;
	mdst[4] = (m.zz*m.xx - m.zx*m.xz)*detinv;
	mdst[5] = -(m.yz*m.xx - m.yx*m.xz)*detinv;
	mdst[6] = (m.zy*m.yx - m.zx*m.yy)*detinv;
	mdst[7] = 	-(m.zy*m.xx - m.zx*m.xy)*detinv;
	mdst[8] = (m.yy*m.xx - m.yx*m.xy)*detinv;
	return mdst;
}
/// Calculates the determinant of a matrix<T,4,4>
template<typename T>
T det(const tmath::matrix<T,4,4>& m) {
	T det;
	det = m[0] * m[5] * m[10];
	det += m[4] * m[9] * m[2];
	det += m[8] * m[1] * m[6];
	det -= m[8] * m[5] * m[2];
	det -= m[4] * m[1] * m[10];
	det -= m[0] * m[9] * m[6];
	return det;
}

/// Calculates the inverse of a matrix<T,4,4>
template<typename T>
const tmath::matrix<T,4,4> inv(const tmath::matrix<T,4,4>& m) {
	tmath::matrix<T,4,4> mat;
	T tmp[12];
	T src[16];


//		trp(m,mat);

	for ( int i = 0; i < 4; i++) {
		src[i] = m[i*4];
		src[i + 4] = m[i*4 + 1];
		src[i + 8] = m[i*4 + 2];
		src[i + 12] = m[i*4 + 3];
	}

	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];

	// calculate first 8 elements (cofactors)
	mat.xx = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	mat.xx -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	mat.xy = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	mat.xy -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	mat.xz = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	mat.xz -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	mat.xw = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	mat.xw -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	mat.yx = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	mat.yx -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	mat.yy = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	mat.yy -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	mat.yz = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	mat.yz -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	mat.yw = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	mat.yw -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

	tmp[0] = src[2]*src[7];
	tmp[1] = src[3]*src[6];
	tmp[2] = src[1]*src[7];
	tmp[3] = src[3]*src[5];
	tmp[4] = src[1]*src[6];
	tmp[5] = src[2]*src[5];
	tmp[6] = src[0]*src[7];
	tmp[7] = src[3]*src[4];
	tmp[8] = src[0]*src[6];
	tmp[9] = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];


	mat.zx = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	mat.zx -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	mat.zy = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	mat.zy -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	mat.zz = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	mat.zz-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	mat.zw = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	mat.zw-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	mat.wx = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	mat.wx-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	mat.wy = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	mat.wy-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	mat.wz = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	mat.wz-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	mat.ww = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	mat.ww-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

		// calculate determinant
	T det=src[0]*mat.xx+src[1]*mat.xy+src[2]*mat.xz+src[3]*mat.xw;
	if ( fabs( det ) < static_cast<T>(0.0005) ) {
		return tmath::matrix<T,4,4>();
	}

	det = static_cast<T>(1.0)/det;
	for ( int j = 0; j < 16; j++)
		mat[j] *= det;
	return mat;
}
/// Calculates the inverse of a matrix4<t>
template<typename T>
const tmath::matrix<T,4,4>& inv(const tmath::matrix<T,4,4>& m, tmath::matrix<T,4,4>& mdst ) {
	T det = det( m );
	if ( fabs( det ) < static_cast<T>(0.0005) ) {
		mdst = tmath::matrix<T,4,4>();
		return mdst;
	}
	T idet = 1.0f / det;
	mdst[0] =  (m[5] * m[10] - m[9] * m[6]) * idet;
	mdst[1] = -(m[1] * m[10] - m[9] * m[2]) * idet;
	mdst[2] =  (m[1] * m[6] - m[5] * m[2]) * idet;
	mdst[3] = 0.0;
	mdst[4] = -(m[4] * m[10] - m[8] * m[6]) * idet;
	mdst[5] =  (m[0] * m[10] - m[8] * m[2]) * idet;
	mdst[6] = -(m[0] * m[6] - m[4] * m[2]) * idet;
	mdst[7] = 0.0;
	mdst[8] =  (m[4] * m[9] - m[8] * m[5]) * idet;
	mdst[9] = -(m[0] * m[9] - m[8] * m[1]) * idet;
	mdst[10] =  (m[0] * m[5] - m[4] * m[1]) * idet;
	mdst[11] = 0.0;
	mdst[12] = -(m[12] * mdst[0] + m[13] * mdst[4] + m[14] * mdst[8]);
	mdst[13] = -(m[12] * mdst[1] + m[13] * mdst[5] + m[14] * mdst[9]);
	mdst[14] = -(m[12] * mdst[2] + m[13] * mdst[6] + m[14] * mdst[10]);
	mdst[15] = 1.0;
	return mdst;
}
/// Converts a quaternion into a matrix<T,3,3>
template<typename T>
inline const tmath::matrix<T,3,3>& conv( const tmath::quaternion<T>& q, tmath::matrix<T,3,3>& mres ) {
	mres.xx = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.y * q.y + q.z * q.z );
	mres.xy = static_cast<T>(2.0) * (q.x * q.y - q.z * q.w);
	mres.xz = static_cast<T>(2.0) * (q.x * q.z + q.y * q.w);

	mres.yx = static_cast<T>(2.0) * ( q.x * q.y + q.z * q.w );
	mres.yy = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.x * q.x + q.z * q.z );
	mres.yz = static_cast<T>(2.0) * (q.z * q.y - q.x * q.w );

	mres.zx = static_cast<T>(2.0) * ( q.x * q.z - q.y * q.w );
	mres.zy = static_cast<T>(2.0) * ( q.y * q.z + q.x * q.w );
	mres.zz = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.x * q.x + q.y * q.y );
	return mres;
}

/// Transpose the matrix<T,2,2>
template<typename T>
void transp(const tmath::matrix<T,2,2>& m, tmath::matrix<T,2,2>& rm) {
	rm.xx = m.xx; rm.yy = m.yy;
	rm.yx = m.xy; rm.xy = m.yx;
}

/// Transpose the matrix<T,2,2>
template<typename T>
const tmath::matrix<T,2,2> transp(const tmath::matrix<T,2,2>& m) {
	return tmath::matrix<T,2,2>(	m.xx, m.yx,
														m.xy, m.yy);
}
/// Transpose the matrix<T,3,3>
template<typename T>
const tmath::matrix<T,3,3> transp(const tmath::matrix<T,3,3>& m) {
	return tmath::matrix<T,3,3>(	m.xx, m.yx, m.zx,
														m.xy, m.yy, m.zy,
														m.xz, m.yz, m.zz);
}
/// Transpose the matrix<T,3,3>
template<typename T>
void transp(const tmath::matrix<T,3,3>& m, tmath::matrix<T,3,3>& rm) {
	rm.xx = m.xx; rm.yy = m.yy; rm.zz = m.zz;
	rm.yx = m.xy; rm.xy = m.yx; rm.zx = m.xz;
	rm.xz = m.zx; rm.zy = m.yz; rm.yz = m.zy;
}
/// Transpose the matrix<T,4,4>
template<typename T>
const tmath::matrix<T,4,4> transp(const tmath::matrix<T,4,4>& m) {
	return tmath::matrix<T,4,4>(	m.xx, m.yx, m.zx, m.wx,
														m.xy, m.yy, m.zy, m.wy,
														m.xz, m.yz, m.zz, m.wz,
														m.xw, m.yw, m.zw, m.ww);
}
/// Transpose the matrix<T,4,4>
template<typename T>
void transp(const tmath::matrix<T,4,4>& m, tmath::matrix<T,4,4>& rm) {
	rm.xx = m.xx; rm.yy	= m.yy;
	rm.zz	= m.zz;	rm.ww	= m.ww;
	rm.xy = m.yx; rm.xz = m.zx;
	rm.xw = m.wx; rm.yx = m.xy;
	rm.yz = m.zy; rm.yw = m.wy;
	rm.zx = m.xz; rm.zy = m.yz;
	rm.zw = m.wz; rm.wx = m.xw;
	rm.wy = m.yw; rm.wz = m.zw;
}
/// Make a null matrix<T,2,2>
template<typename T>
inline tmath::matrix<T,2,2>& null(tmath::matrix<T,2,2>& mat) {
	mat.xx = static_cast<T>(0.0); mat.xy = static_cast<T>(0.0);
	mat.yx = static_cast<T>(0.0); mat.yy = static_cast<T>(0.0);
	return mat;
}
/// Make a identity matrix<T,2,2>
template<typename T>
inline tmath::matrix<T,2,2>& identity(tmath::matrix<T,2,2>& mat) {
	mat.xx = static_cast<T>(1.0); mat.xy = static_cast<T>(0.0);
	mat.yx = static_cast<T>(0.0); mat.yy = static_cast<T>(1.0);
	return mat;
}
/// Make a null matrix<T,3,3>
template<typename T>
inline tmath::matrix<T,3,3>& null(tmath::matrix<T,3,3>& mat) {
	mat.xx = static_cast<T>(0.0); mat.xy = static_cast<T>(0.0); mat.xz = static_cast<T>(0.0);
	mat.yx = static_cast<T>(0.0); mat.yy = static_cast<T>(0.0); mat.yz = static_cast<T>(0.0);
	mat.zx = static_cast<T>(0.0); mat.zy = static_cast<T>(0.0); mat.zz = static_cast<T>(0.0);
	return mat;
}
/// Make a identity matrix<T,3,3>
template<typename T>
inline tmath::matrix<T,3,3>& identity(tmath::matrix<T,3,3>& mat) {
	mat.xx = static_cast<T>(1.0); mat.xy = static_cast<T>(0.0); mat.xz = static_cast<T>(0.0);
	mat.yx = static_cast<T>(0.0); mat.yy = static_cast<T>(1.0); mat.yz = static_cast<T>(0.0);
	mat.zx = static_cast<T>(0.0); mat.zy = static_cast<T>(0.0); mat.zz = static_cast<T>(1.0);
	return mat;
}
/// Make a null matrix<T,4,4>
template<typename T>
inline tmath::matrix<T,4,4>& null(tmath::matrix<T,4,4>& mat) {
	mat.xx = static_cast<T>(0.0); mat.xy = static_cast<T>(0.0); mat.xz = static_cast<T>(0.0); mat.xw = static_cast<T>(0.0);
	mat.yx = static_cast<T>(0.0); mat.yy = static_cast<T>(0.0); mat.yz = static_cast<T>(0.0); mat.yw = static_cast<T>(0.0);
	mat.zx = static_cast<T>(0.0); mat.zy = static_cast<T>(0.0); mat.zz = static_cast<T>(0.0); mat.zw = static_cast<T>(0.0);
	mat.wx = static_cast<T>(0.0); mat.wy = static_cast<T>(0.0); mat.wz = static_cast<T>(0.0); mat.ww = static_cast<T>(0.0);
	return mat;
}
/// Make a identity matrix<T,4,4>
template<typename T>
inline tmath::matrix<T,4,4>& identity(tmath::matrix<T,4,4>& mat) {
	mat.xx = static_cast<T>(1.0); mat.xy = static_cast<T>(0.0); mat.xz = static_cast<T>(0.0); mat.xw = static_cast<T>(0.0);
	mat.yx = static_cast<T>(0.0); mat.yy = static_cast<T>(1.0); mat.yz = static_cast<T>(0.0); mat.yw = static_cast<T>(0.0);
	mat.zx = static_cast<T>(0.0); mat.zy = static_cast<T>(0.0); mat.zz = static_cast<T>(1.0); mat.zw = static_cast<T>(0.0);
	mat.wx = static_cast<T>(0.0); mat.wy = static_cast<T>(0.0); mat.wz = static_cast<T>(0.0); mat.ww = static_cast<T>(1.0);
	return mat;
}

/// Convert a matrix3 into a matrix4
template<typename T>
inline const tmath::matrix<T,4,4>&  conv(const tmath::matrix<T,3,3>& m1, tmath::matrix<T,4,4>& m2) {
	m2.xx = m1.xx; m2.xy = m1.xy; m2.xz = m1.xz; m2.xw = static_cast<T>(0.0);
	m2.yx = m1.yx; m2.yy = m1.yy; m2.yz = m1.yz; m2.yw = static_cast<T>(0.0);
	m2.zx = m1.zx; m2.zy = m1.zy; m2.zz = m1.zz; m2.zw = static_cast<T>(0.0);
	m2.wx = static_cast<T>(0.0); m2.wy = static_cast<T>(0.0); m2.wz = static_cast<T>(0.0); m2.ww = static_cast<T>(1.0);
	return m2;
}

/// Convert a matrix3 into a matrix3
template<typename T>
inline const tmath::matrix<T,3,3>&  conv(const tmath::matrix<T,4,4>& m1, tmath::matrix<T,3,3>& m2) {
	m2.xx = m1.xx; m2.xy = m1.xy; m2.xz = m1.xz;
	m2.yx = m1.yx; m2.yy = m1.yy; m2.yz = m1.yz;
	m2.zx = m1.zx; m2.zy = m1.zy; m2.zz = m1.zz;
	return m2;
}

/// Convert a matrix2 into a matrix4
template<typename T>
inline const tmath::matrix<T,4,4>&  conv(const tmath::matrix<T,2,2>& m1, tmath::matrix<T,4,4>& m2) {
	m2.xx = m1.xx; m2.xy = m1.xy; m2.xz = static_cast<T>(0.0); m2.xw = static_cast<T>(0.0);
	m2.yx = m1.yx; m2.yy = m1.yy; m2.yz = static_cast<T>(0.0); m2.yw = static_cast<T>(0.0);
	m2.zx = static_cast<T>(0.0); m2.zy = static_cast<T>(0.0);	m2.zz = static_cast<T>(1.0); m2.zw = static_cast<T>(0.0);
	m2.wx = static_cast<T>(0.0); m2.wy = static_cast<T>(0.0); m2.wz = static_cast<T>(0.0); m2.ww = static_cast<T>(1.0);
	return m2;
}

/// Convert a matrix4 into a matrix2
template<typename T>
inline const tmath::matrix<T,2,2>&  conv(const tmath::matrix<T,4,4>& m1, tmath::matrix<T,2,2>& m2) {
	m2.xx = m1.xx; m2.xy = m1.xy;
	m2.yx = m1.yx; m2.yy = m1.yy;
	return m2;
}

/// Convert a matrix2 into a matrix3
template<typename T>
inline const tmath::matrix<T,3,3>&  conv(const tmath::matrix<T,2,2>& m1, tmath::matrix<T,3,3>& m2) {
	m2.xx = m1.xx; m2.xy = m1.xy; m2.xz =static_cast<T>(0.0);
	m2.yx = m1.yx; m2.yy = m1.yy; m2.yz = static_cast<T>(0.0);
	m2.zx =static_cast<T>(0.0); m2.zy =static_cast<T>(0.0); m2.zz =static_cast<T>(1.0);;
	return m2;
}

/// Convert a matrix3 into a matrix2
template<typename T>
inline const tmath::matrix<T,2,2>&  conv(const tmath::matrix<T,3,3>& m1, tmath::matrix<T,2,2>& m2) {
	m2.xx = m1.xx; m2.xy = m1.xy;
	m2.yx = m1.yx; m2.yy = m1.yy;
	return m2;
}

}

#endif
