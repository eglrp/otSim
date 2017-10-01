//Source modified by Cory Parks 09/2017

#ifndef TMG_H
#define TMG_H

#include <tm/tm.h>

namespace tmg {


/// Pi value
const double pi =  3.1415926535897932384626433832795;

/// Converts degres to radian
template<typename T>
inline const T d2r(const T a) {
	return (a * static_cast<T>(pi))/static_cast<T>(180.0);
}
/// Converts radian to degres
template<typename T>
inline const T r2d(const T a) {
	return (a * static_cast<T>(180.0f))/static_cast<T>(pi);
}

/// Calculate a quaternion that represents a rotation around an arbitrary axis
template<class T>
void rot(const T angle, const tmath::vectorn<T,3> &axis, tmath::quaternion<T>& q) {
	register T rad = d2r(angle*static_cast<T>(0.5));
  q.w = cos(rad);
  q.x = axis.x * sin(rad);
	q.y = axis.y * sin(rad);
	q.z = axis.z * sin(rad);
}
/// Calculate a quaternion that represents a rotation around the x axis
template<class T>
void rotx(const T angle, tmath::quaternion<T>& q) {
	register T rad = d2r(angle*static_cast<T>(0.5));
  q.w = cos(rad);
  q.x = sin(rad);
	q.y = static_cast<T>(0.0);
	q.z = static_cast<T>(0.0);
}
/// Calculate a quaternion that represents a rotation around the y axis
template<class T>
void roty(const T angle, tmath::quaternion<T>& q) {
	register T rad = d2r(angle*static_cast<T>(0.5));
  q.w = cos(rad);
	q.x = static_cast<T>(0.0);
  q.y = sin(rad);
	q.z = static_cast<T>(0.0);
}
/// Calculate a quaternion that represents a rotation around the z axis
template<class T>
void rotz(const T angle, tmath::quaternion<T>& q) {
	register T rad = d2r(angle*static_cast<T>(0.5));
  q.w = cos(rad);
	q.x = static_cast<T>(0.0);
 	q.y = static_cast<T>(0.0);
	q.z = sin(rad);
}

/// Converts a quaternion orientation into euler angles
template<class T>
void q2e(const tmath::quaternion<T>& q, T& yaw, T& pitch, T& roll) {

	//	ZYX Rotation Sequence (Yaw-Pitch-Roll)

	T qwqw = q.w*q.w;
	T qxqx = q.x*q.x;
	T qyqy = q.y*q.y;
	T qzqz = q.z*q.z;

	T mat11 = qwqw + qxqx - qyqy - qzqz;
	T mat12 = static_cast<T>(2.0)*(q.x*q.y + q.w*q.z);
	T mat13 = static_cast<T>(2.0)*(q.x*q.z - q.w*q.y);
	T mat23 = static_cast<T>(2.0)*(q.y*q.z + q.w*q.x);
	T mat33 = qwqw - qxqx - qyqy + qzqz;

	if (mat11 == 0.0)
		yaw = 0.5*pi;
	else {
		yaw = atan2(mat12, mat11);
	}

	if (mat13 < -1.0)
		pitch = 0.5*pi;
	else if (1.0 < mat13)
		pitch = -0.5*pi;
	else
		pitch = asin(-mat13);

	if (mat33 == 0.0)
		roll = 0.5*pi;
	else
		roll = atan2(mat23, mat33);
}

/// Converts euler angles into a quaternion orienation
template<typename T>
void e2q(T yaw, T pitch, T roll, tmath::quaternion<T>& q) {
  T cosYaw		= cos(yaw / static_cast<T>(2.0));
  T sinYaw		= sin(yaw / static_cast<T>(2.0));
  T cosPitch	= cos(pitch / static_cast<T>(2.0));
  T sinPitch	= sin(pitch / static_cast<T>(2.0));
  T cosRoll		= cos(roll / static_cast<T>(2.0));
  T sinRoll		= sin(roll / static_cast<T>(2.0));

	double cosRoll_cosPitch = cosRoll*cosPitch;
	double cosRoll_sinPitch = cosRoll*sinPitch;
	double sinRoll_sinPitch = sinRoll*sinPitch;
	double sinRoll_cosPitch = sinRoll*cosPitch;

	q.x = sinRoll_cosPitch*cosYaw - cosRoll_sinPitch*sinYaw;
	q.y = cosRoll_sinPitch*cosYaw + sinRoll_cosPitch*sinYaw;
	q.z = cosRoll_cosPitch*sinYaw - sinRoll_sinPitch*cosYaw;
	q.w = cosRoll_cosPitch*cosYaw + sinRoll_sinPitch*sinYaw;
}

/// Converts euler angles into a quaternion orienation
template<typename T>
tmath::quaternion<T> e2q(T yaw, T pitch, T roll) {
	tmath::quaternion<T> q;
	e2q(yaw, pitch, roll, q);
	return q;
}


/// Create a quaternion that represents a spherical interpolation
template<class T>
tmath::quaternion<T>  slerp(tmath::quaternion<T>& q1, tmath::quaternion<T>& q2, T t) {
	tmath::quaternion<T> qInterpolated;
	if(q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w)
 		return q1;

	T result = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

	if(result < 0.0f) {
		q2 = quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
		result = -result;
	}

	T scale0 = 1 - t, scale1 = t;
	if(1 - result > 0.1f) {
		T theta = (T)acos(result);
		T sinTheta = (T)sin(theta);

		scale0 = (T)sin( ( 1 - t ) * theta) / sinTheta;
		scale1 = (T)sin( ( t * theta) ) / sinTheta;
	}

	qInterpolated.x = (scale0 * q1.x) + (scale1 * q2.x);
	qInterpolated.y = (scale0 * q1.y) + (scale1 * q2.y);
	qInterpolated.z = (scale0 * q1.z) + (scale1 * q2.z);
	qInterpolated.w = (scale0 * q1.w) + (scale1 * q2.w);

	return qInterpolated;
}

// ----------------------------------------------------------------------------
// matrix3 functions
//
	/// Creates a matrix<T,3,3> that represents a rotation around the x axis
	template<typename T>
	void rotx(const T& angle, tmath::matrix<T,3,3>& m) {
		register T rad = d2r(angle);
		register T c = cos(rad);
		register T s = sin(rad);
		m.xx = 1.0; m.xy = 0.0; m.xz = 0.0;
		m.yx = 0.0; m.yy = c;	m.yz = -s;
		m.zx = 0.0; m.zy = s;	m.zz = c;
	}
	/// Creates a matrix<T,3,3> that represents a rotation around the y axis
	template<typename T>
	void roty(const T& angle, tmath::matrix<T,3,3>& m) {
		register T rad = d2r(angle);
		register T c = cos(rad);
		register T s = sin(rad);
		m.xx = c;	m.xy = 0.0; m.xz = s;
		m.yx = 0.0; m.yy = 1;	m.yz = 0;
		m.zx = -s;	m.zy = 0;	m.zz = c;
	}
	/// Creates a matrix<T,3,3> that represents a rotation around the z axis
	template<typename T>
	void rotz(const T& angle, tmath::matrix<T,3,3>& m) {
		register T rad = d2r(angle);
		register T c = cos(rad);
		register T s = sin(rad);
		m.xx = c;	m.xy = -s;	m.xz = 0.0;
		m.yx = s;	m.yy = c;	m.yz = 0.0;
		m.zx = 0.0;	m.zy = 0.0;	m.zz = 1.0;
	}
	/// Creates a matrix<T,3,3> that represents a rotation around an arbitrary axis
	template<typename T>
	void rot(const T& angle, const tmath::vectorn<T,3>& axis, tmath::matrix<T,3,3>& pM) {
		T rad = d2r(angle);
		T c = cos(rad);
		T s = sin(rad);
		tmath::vectorn<T,3> v = axis;
		tmath::normalize(v);
		T xy = v.x * v.y;
		T yz = v.y * v.z;
		T zx = v.z * v.x;
		T xs = v.x * s;
		T ys = v.y * s;
		T zs = v.z * s;
		T tt = static_cast<T>(1.0) - c;

		pM.xx = tt * v.x * v.x + c;
		pM.xy = tt * xy - zs;
		pM.xz = tt * zx + ys;

		pM.yx = tt * xy + zs;
		pM.yy = tt * v.y * v.y + c;
		pM.yz = tt * yz - xs;

		pM.zx = tt * zx - ys;
		pM.zy = tt * yz + xs;
		pM.zz = tt * v.z * v.z + c;
	}

// ----------------------------------------------------------------------------
// matrix3 functions
//

	/// Creates a matrix<T,4,4> that represents a rotation around the x axis
	template<typename T>
	void rotx(const T& angle, tmath::matrix<T,4,4>& m) {
		register T rad = d2r(angle);
		register T c = static_cast<T>(cos(rad));
		register T s = static_cast<T>(sin(rad));
		m.xx = 1.0; m.xy = 0.0; m.xz = 0.0; m.xw = 0.0;
		m.yx = 0.0; m.yy = c;		m.yz = -s;	m.yw = 0.0;
		m.zx = 0.0; m.zy = s;		m.zz = c;		m.zw = 0.0;
		m.wx = 0.0; m.wy = 0.0; m.wz = 0.0; m.ww = 1.0;
	}
	/// Creates a matrix<T,4,4> that represents a rotation around the y axis
	template<typename T>
	void roty(const T& angle, tmath::matrix<T,4,4>& m) {
		register T rad = d2r(angle);
		register T c = static_cast<T>(cos(rad));
		register T s = static_cast<T>(sin(rad));
		m.xx = c;		m.xy = 0.0; m.xz = s;		m.xw = 0.0;
		m.yx = 0.0; m.yy = 1;		m.yz = 0;		m.yw = 0.0;
		m.zx = -s;	m.zy = 0;		m.zz = c;		m.zw = 0.0;
		m.wx = 0.0; m.wy = 0.0; m.wz = 0.0; m.ww = 1.0;
	}
	/// Creates a matrix<T,4,4> that represents a rotation around the z axis
	template<typename T>
	void rotz(const T& angle, tmath::matrix<T,4,4>& m) {
		register T rad = d2r(angle);
		register T c = static_cast<T>(cos(rad));
		register T s = static_cast<T>(sin(rad));
		m.xx = c;		m.xy = -s;	m.xz = 0.0; m.xw = 0.0;
		m.yx = s;		m.yy = c;		m.yz = 0.0;	m.yw = 0.0;
		m.zx = 0.0;	m.zy = 0.0;	m.zz = 1.0;	m.zw = 0.0;
		m.wx = 0.0; m.wy = 0.0; m.wz = 0.0; m.ww = 1.0;
	}
	/// Creates a matrix<T,4,4> that represents a rotation around an arbitrary axis
	template<typename T>
	void rot(const T& angle, const tmath::vectorn<T,3>& axis, tmath::matrix<T,4,4>& pM) {
		T rad = d2r(angle);
		T c = static_cast<T>(cos(rad));
		T s = static_cast<T>( sin(rad));
		tmath::vectorn<T,3> v = axis;
		tmath::normalize(v);
		T xy = v.x * v.y;
		T yz = v.y * v.z;
		T zx = v.z * v.x;
		T xs = v.x * s;
		T ys = v.y * s;
		T zs = v.z * s;
		T tt = static_cast<T>(1.0) - c;

		pM.xx = tt * v.x * v.x + c;
		pM.xy = tt * xy - zs;
		pM.xz = tt * zx + ys;
		pM.xw = 0.0;

		pM.yx = tt * xy + zs;
		pM.yy = tt * v.y * v.y + c;
		pM.yz = tt * yz - xs;
		pM.yw = 0.0;

		pM.zx = tt * zx - ys;
		pM.zy = tt * yz + xs;
		pM.zz = tt * v.z * v.z + c;
		pM.zw = 0.0;

		pM.wx = 0.0;
		pM.wy = 0.0;
		pM.wz = 0.0;
		pM.ww = 1.0;
	}
	/// Creates a matrix<T,4,4> that represents a translation
	template<typename T>
	void transl(const T x, const T y, const T z, tmath::matrix<T,4,4>& m) {
		m.xx = 1.0; m.xy = 0.0; m.xz = 0.0; m.xw = x;
		m.yx = 0.0; m.yy = 1.0; m.yz = 0.0; m.yw = y;
		m.zx = 0.0; m.zy = 0.0; m.zz = 1.0; m.zw = z;
		m.wx = 0.0; m.wy = 0.0; m.wz = 0.0; m.ww = 1.0;
	}
	/// Creates a matrix<T,4,4> that represents a translation
	template<typename T>
	void transl(const tmath::vectorn<T,3>& v, tmath::matrix<T,4,4>& m) {
		m.xx = 1.0; m.xy = 0.0; m.xz = 0.0; m.xw = v.x;
		m.yx = 0.0; m.yy = 1.0; m.yz = 0.0; m.yw = v.y;
		m.zx = 0.0; m.zy = 0.0; m.zz = 1.0; m.zw = v.z;
		m.wx = 0.0; m.wy = 0.0; m.wz = 0.0; m.ww = 1.0;
	}
	/// Creates a matrix<T,4,4> that represents a translation
	template<typename T>
	tmath::matrix<T,4,4> transl(const tmath::vectorn<T,3>& v) {
		return tmath::matrix<T,4,4>(1.0,0.0,0.0,v.x,
																0.0,1.0,0.0,v.y,
																0.0,0.0,1.0,v.z,
																0.0,0.0,0.0,1.0);
	}


	/// Creates a matrix<T,4,4> that represents a scaling
	template<typename T>
	void scale(const tmath::vectorn<T,3>& v, tmath::matrix<T,4,4>& m) {
		m.xx = v.x; m.xy = 0.0; m.xz = 0.0; m.xw = 0.0;
		m.yx = 0.0; m.yy = v.y; m.yz = 0.0; m.yw = 0.0;
		m.zx = 0.0; m.zy = 0.0; m.zz = v.z; m.zw = 0.0;
		m.wx = 0.0; m.wy = 0.0; m.wz = 0.0; m.ww = 1.0;
	}

	template<typename T>
	tmath::matrix<T, 4, 4>& frustum(T fleft, T fright, T fbottom, T ftop, T fnear, T ffar, tmath::matrix<T, 4, 4>& pSrc)
	{
		pSrc.xx = 2.0f*fnear/(fright-fleft);	pSrc.xy = 0.0f;												pSrc.xz =(fright+fleft)/(fright-fleft);		pSrc.xw = 0.0f;
		pSrc.yx = 0.0f;												pSrc.yy = 2.0f*fnear/(ftop-fbottom);	pSrc.yz = (ftop+fbottom)/(ftop-fbottom);	pSrc.yw = 0.0f;
		pSrc.zx = 0.0f;												pSrc.zy = 0.0f;												pSrc.zz = -(ffar+fnear)/(ffar-fnear);			pSrc.zw = -2.0f*ffar*fnear/(ffar-fnear);
		pSrc.wx = 0.0f;												pSrc.wy = 0.0f;												pSrc.wz = -1.0f;													pSrc.ww = 0.0f;
		return pSrc;
	}

	template<typename T>
	tmath::matrix<T, 4, 4> frustum(T fleft, T fright, T fbottom, T ftop, T fnear, T ffar)
	{
		return tmath::matrix<T, 4, 4>(	2.0f*fnear/(fright-fleft) , 0.0f											, (fright+fleft)/(fright-fleft)	, 0.0f,
																		0.0f											,	2.0f*fnear/(ftop-fbottom)	, (ftop+fbottom)/(ftop-fbottom)	, 0.0f,
																		0.0f											, 0.0f											, -(ffar+fnear)/(ffar-fnear)		, -2.0f*ffar*fnear/(ffar-fnear),
																		0.0f											, 0.0f											, -1.0f													, 0.0f);

	}

	template<typename T>
	tmath::matrix<T, 4, 4>& ortho(T fleft, T fright, T fbottom, T ftop, T fnear, T ffar, tmath::matrix<T, 4, 4>& pSrc)
	{
		pSrc.xx = 2.0f/(fright-fleft);	pSrc.xy = 0.0f;									pSrc.xz = 0.0f;								pSrc.xw = -(fright+fleft)/(fright-fleft);
		pSrc.yx = 0.0f;									pSrc.yy = 2.0f/(ftop-fbottom);	pSrc.yz = 0.0f;								pSrc.yw = -(ftop+fbottom)/(ftop-fbottom);
		pSrc.zx = 0.0f;									pSrc.zy = 0.0f;									pSrc.zz = -2.0f/(ffar-fnear); pSrc.zw = -(ffar + fnear)/(ffar-fnear);
		pSrc.wx = 0.0f;									pSrc.wy = 0.0f;									pSrc.wz = 0.0f;								pSrc.ww = 1.0f;
		return pSrc;
	}

	template<typename T>
	tmath::matrix<T, 4, 4> ortho(T fleft, T fright, T fbottom, T ftop, T fnear, T ffar)
	{
		return tmath::matrix<T, 4, 4>(	2.0f/(fright-fleft) , 0.0f								, 0.0f							, -(fright+fleft)/(fright-fleft),
																		0.0f								,	2.0f/(ftop-fbottom)	, 0.0f							, -(ftop+fbottom)/(ftop-fbottom),
																		0.0f								, 0.0f								, -2.0f/(ffar-fnear), -(ffar + fnear)/(ffar-fnear),
																		0.0f								, 0.0f								, 0.0								, 1.0f);

	}

	template<typename T>
	tmath::matrix<T, 4, 4> lookat(tmath::vectorn<T,3>& eye, tmath::vectorn<T,3>& center, tmath::vectorn<T,3>& up )
	{
			tmath::vectorn<float,3> forward, side;
			tmath::matrix<float,4,4> m, trans;
			// Calculate the direction of the view.
			forward = center - eye;
			tmath::normalize(forward);

			// Calculate the positive right coordinate
			tmath::cross(forward, up, side);
			tmath::normalize(side);

			// Now we must recompute the up vector to ajust
			// it to the new right vector.
			tmath::cross(side, forward, up);

			// Fill the right information into the matrix
			m.xx = side.x;
			m.xy = side.y;
			m.xz = side.z;

			m.yx = up.x;
			m.yy = up.y;
			m.yz = up.z;

			m.zx = -forward.x;
			m.zy = -forward.y;
			m.zz = -forward.z;

			m.ww = 1.0f;
			transl(eye, trans);
			return trans;
	}

	template<typename T>
	void e2m(T yaw, T pitch, T roll, tmath::matrix<T,3,3>& m) {
		T sinYaw = sin(yaw);
		T cosYaw = cos(yaw);
		T sinPitch = sin(pitch);
		T cosPitch = cos(pitch);
		T sinRoll = sin(roll);
		T cosRoll = cos(roll);

		//	ZYX Rotation Sequence (Yaw-Pitch-Roll)
		//    [	cosPitch*cosYaw,								cosPitch*sinYaw,								-sinPitch			]
		//    [	sinPitch*sinRoll*cosYaw - sinYaw*cosRoll,		sinPitch*sinRoll*sinYaw + cosYaw*cosRoll,		cosPitch*sinRoll	]
		//    [	sinPitch*cosRoll*cosYaw + sinYaw*sinRoll,		sinPitch*cosRoll*sinYaw - cosYaw*sinRoll,		cosPitch*cosRoll	]

		m.xx = cosYaw*cosPitch;								m.xy = sinYaw*cosPitch;								m.xz = -sinPitch;
		m.yx = -sinYaw*cosRoll + cosYaw*sinPitch*sinRoll; 	m.yy = cosYaw*cosRoll + sinYaw*sinPitch*sinRoll;	m.yz = cosPitch*sinRoll;
		m.zx = cosYaw*sinPitch*cosRoll + sinYaw*sinRoll;	m.zy = -cosYaw*sinRoll + sinYaw*sinPitch*cosRoll;	m.zz = cosPitch*cosRoll;
	}

	template<typename T>
	tmath::matrix<T, 3, 3> e2m(T yaw, T pitch, T roll) {
		tmath::matrix<T, 3, 3> m;
		e2m(yaw, pitch, roll, m);
		return m;
	}

	/// Converts a rotation matrix into euler angles
	template<class T>
	void m2e(const tmath::matrix<T,3,3>& m, T& yaw, T& pitch, T& roll) {

		//	ZYX Rotation Sequence (Yaw-Pitch-Roll)

		if (m.xx == 0.0)
			yaw = 0.5*pi;
		else {
			yaw = atan2(m.xy, m.xx);
		}

		if (m.xz < -1.0)
			pitch = 0.5*pi;
		else if (1.0 < m.xz)
			pitch = -0.5*pi;
		else
			pitch = asin(-m.xz);

		if (m.zz == 0.0)
			roll = 0.5*pi;
		else
			roll = atan2(m.yz, m.zz);
	}

	/// Converts a quaternion into a matrix<T,3,3>
	template<typename T>
	void q2m(const tmath::quaternion<T>& q, tmath::matrix<T, 3, 3>& m) {

		T qwqw = q.w*q.w;
		T qxqx = q.x*q.x;
		T qyqy = q.y*q.y;
		T qzqz = q.z*q.z;
		T qxqw = q.x*q.w;
		T qyqw = q.y*q.w;
		T qzqw = q.z*q.w;
		T qxqy = q.x*q.y;
		T qxqz = q.x*q.z;
		T qyqz = q.y*q.z;

		m.xx = static_cast<T>(1.0) - static_cast<T>(2.0) * (qyqy + qzqz);
		m.xy = static_cast<T>(2.0) * (qxqy + qzqw);
		m.xz = static_cast<T>(2.0) * (qxqz - qyqw);

		m.yx = static_cast<T>(2.0) * (qxqy - qzqw);
		m.yy = static_cast<T>(1.0) - static_cast<T>(2.0) * (qxqx + qzqz);
		m.yz = static_cast<T>(2.0) * (qyqz + qxqw);

		m.zx = static_cast<T>(2.0) * (qxqz + qyqw);
		m.zy = static_cast<T>(2.0) * (qyqz - qxqw);
		m.zz = static_cast<T>(1.0) - static_cast<T>(2.0) * (qxqx + qyqy);
	}

	/// Converts a quaternion into a matrix<T,3,3>
	template<typename T>
	tmath::matrix<T, 3, 3> q2m(const tmath::quaternion<T>& q) {
		tmath::matrix<T, 3, 3> m;
		q2m(q, m);
		return m;
	}

	/// Converts a rotation matrix<T,3,3> into a quaternion rotation
	template<typename T>
	void m2q(const tmath::matrix<T, 3, 3>& m, tmath::quaternion<T>& q) {

		T tempQ[4];
		int idx;

		tempQ[0] = static_cast<T>(1.0) + m.xx + m.yy + m.zz;

		tempQ[1] = static_cast<T>(1.0) + m.xx - m.yy - m.zz;
		tempQ[2] = static_cast<T>(1.0) - m.xx + m.yy - m.zz;
		tempQ[3] = static_cast<T>(1.0) - m.xx - m.yy + m.zz;

		// Find largest of the above
		idx = 0;
		for (int i = 1; i < 4; i++) if (tempQ[i] > tempQ[idx]) idx = i;

		switch (idx) {
		case 0:
			q.w = 0.50*sqrt(tempQ[0]);
			q.x = 0.25*(m.yz - m.zy) / q.w;
			q.y = 0.25*(m.zx - m.xz) / q.w;
			q.z = 0.25*(m.xy - m.yx) / q.w;
			break;
		case 1:
			q.x = 0.50*sqrt(tempQ[1]);
			q.w = 0.25*(m.yz - m.zy) / q.x;
			q.y = 0.25*(m.xy + m.yx) / q.x;
			q.z = 0.25*(m.zx + m.xz) / q.x;
			break;
		case 2:
			q.y = 0.50*sqrt(tempQ[2]);
			q.w = 0.25*(m.zx - m.xz) / q.y;
			q.x = 0.25*(m.xy + m.yx) / q.y;
			q.z = 0.25*(m.yz + m.zy) / q.y;
			break;
		case 3:
			q.z = 0.50*sqrt(tempQ[3]);
			q.w = 0.25*(m.xy - m.yx) / q.z;
			q.x = 0.25*(m.xz + m.zx) / q.z;
			q.y = 0.25*(m.yz + m.zy) / q.z;
			break;
		default:
			//error
			break;
		}
	}

	/// Converts a rotation matrix<T,3,3> into a quaternion rotation
	template<typename T>
	tmath::quaternion<T> m2q(const tmath::matrix<T, 3, 3>& m) {
		tmath::quaternion<T> q;
		m2q(m, q);
		return q;
	}

} // End of namespace

#endif

