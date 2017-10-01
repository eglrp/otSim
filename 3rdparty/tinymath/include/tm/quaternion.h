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

#ifndef QUATERNION_H
#define QUATERNION_H

namespace tmath{

template<typename T, int N, int M>
class matrix;

/**
	@class 			quaternion
	@brief 			Class that represents a quaternion
	@author 		Cengiz Terzibas
*/
template <typename T>
class quaternion {
public:
	T x, y, z, w;
	// constructors
	quaternion<T>() : x(static_cast<T>(0.0)),
										y(static_cast<T>(0.0)),
										z(static_cast<T>(0.0)),
										w(static_cast<T>(1.0)) {
	}
	quaternion<T>(T qx, T qy, T qz, T qw) : x(qx), y(qy), z(qz), w(qw) {
	}
	quaternion<T>(const T* q) : x(q[0]), y(q[1]), z(q[2]), w(q[3]) {
	}
	quaternion<T>(const quaternion<T>& q) : x(q.x), y(q.y), z(q.z), w(q.w) {
	}
	/// Constructs a quaternion rotation from yaw-pitch-roll euler rotations
	quaternion<T>(T yaw, T pitch, T roll) {
		T cosYaw = cos(yaw / static_cast<T>(2.0));
		T sinYaw = sin(yaw / static_cast<T>(2.0));
		T cosPitch = cos(pitch / static_cast<T>(2.0));
		T sinPitch = sin(pitch / static_cast<T>(2.0));
		T cosRoll = cos(roll / static_cast<T>(2.0));
		T sinRoll = sin(roll / static_cast<T>(2.0));

		double cosRoll_cosPitch = cosRoll*cosPitch;
		double cosRoll_sinPitch = cosRoll*sinPitch;
		double sinRoll_sinPitch = sinRoll*sinPitch;
		double sinRoll_cosPitch = sinRoll*cosPitch;

		x = sinRoll_cosPitch*cosYaw - cosRoll_sinPitch*sinYaw;
		y = cosRoll_sinPitch*cosYaw + sinRoll_cosPitch*sinYaw;
		z = cosRoll_cosPitch*sinYaw - sinRoll_sinPitch*cosYaw;
		w = cosRoll_cosPitch*cosYaw + sinRoll_sinPitch*sinYaw;
	}

	/// Constructs a quaternion rotation from a rotation matrix<T,3,3>
	quaternion<T>(const tmath::matrix<T, 3, 3>& m) {

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
			w = 0.50*sqrt(tempQ[0]);
			x = 0.25*(m.yz - m.zy) / w;
			y = 0.25*(m.zx - m.xz) / w;
			z = 0.25*(m.xy - m.yx) / w;
			break;
		case 1:
			x = 0.50*sqrt(tempQ[1]);
			w = 0.25*(m.yz - m.zy) / x;
			y = 0.25*(m.xy + m.yx) / x;
			z = 0.25*(m.zx + m.xz) / x;
			break;
		case 2:
			y = 0.50*sqrt(tempQ[2]);
			w = 0.25*(m.zx - m.xz) / y;
			x = 0.25*(m.xy + m.yx) / y;
			z = 0.25*(m.yz + m.zy) / y;
			break;
		case 3:
			z = 0.50*sqrt(tempQ[3]);
			w = 0.25*(m.xy - m.yx) / z;
			x = 0.25*(m.xz + m.zx) / z;
			y = 0.25*(m.yz + m.zy) / z;
			break;
		default:
			//error
			break;
		}
	}

	// assignment operations
	inline const quaternion<T> operator+=(const quaternion<T> &q) {
		w += q.w; x += q.x; y += q.y; z += q.z; return *this;
	}
	inline const quaternion<T> operator-=(const quaternion<T> &q) {
		w -= q.w; x -= q.x; y -= q.y; z -= q.z; return *this;
	}
	inline quaternion<T> operator*=(const T &qs) {
		x *= qs; y *= qs; z *= qs; w *= qs;
		return *this;
 	}
	// stream operations
	inline friend std::ostream& operator<<(std::ostream& out, const quaternion<T>& q) {
		out << "[" << q.x << "," << q.y << "," << q.z << "," << q.w << "]"; return out;
	}
	// comparison operations
	inline const bool operator == ( const quaternion<T>& q ) const {
		return (q.x==x && q.y==y && q.z==z && q.w==w );
	}
	inline const bool operator != ( const quaternion<T>& q ) const {
		return !(q == *this);
	}
	// unary operations
	inline const quaternion<T> operator - () const {
		return quaternion<T>( -x, -y, -z, -w);
	}
	// binary operations
	inline friend const quaternion<T> operator+(const quaternion<T> &q1, const quaternion<T> &q2) {
		return quaternion<T>(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
	}
	inline friend const quaternion<T> operator-(const quaternion<T> &q1, const quaternion<T> &q2) {
		return quaternion<T>(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
	}
	inline quaternion<T> operator*(const T &qs) const {
  	return quaternion<T>(x * qs, y * qs, z * qs,  w * qs);
 	}
	friend inline quaternion<T> operator*(const T& pS, const quaternion<T>& q) {
		return quaternion<T>(q.x * pS, q.y * pS, q.z * pS, q.w * pS);
	}
	inline friend const quaternion<T> operator * ( const quaternion<T>& q1, const quaternion<T>& q2) {
		return quaternion<T>( q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
													q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
													q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
													q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
	}
	// cast operations
	operator T*() {
		return &x;
	}
	operator const T*() const {
		return &x;
	}


	// Calculates the norm of a quaternion
	inline T norm() const {
		return (w*w + x*x + y*y + z*z);
	}
	// Calculates the length of a quaternion
	inline T len() const {
		return static_cast<T>(sqrt(w*w + x*x + y*y + z*z));
	}
	// Normalize the quaternion (in place)
	quaternion<T>& normalize() {
		T length = len();
		if (fabs(length - static_cast<T>(1.000)) < static_cast<T>(1e-10)) {
			//don't do anything, already normalized
		}
		else if (length > static_cast<T>(0.0)) {
			T normFactor = static_cast<T>(1.0) / length;
			w *= normFactor;
			x *= normFactor;
			y *= normFactor;
			z *= normFactor;
		}
		else {
			w = static_cast<T>(1.0);
			x = y = z = static_cast<T>(0.0);
		}

		return *this;
	}
	// Return Normalized copy of the quaternion
	quaternion<T> normalized() const {
		quaternion<T> q(this);
		q.normalize();

		return q;
	}

	// conjugate the quaternion
	inline quaternion<T> conj() const {
		tmath::quaternion<T> qres(this);
		qres.x *= static_cast<T>(-1.0);
		qres.y *= static_cast<T>(-1.0);
		qres.z *= static_cast<T>(-1.0);
		return qres;
	}
	// invert the quaternion
	inline quaternion<T> inv() const {
		tmath::quaternion<T> qres(this);
		T s = qres.len();
		if (s > static_cast<T>(0.0)) {
			s = static_cast<T>(1.0) / s;
			qres *= s;
			return conj(qres);
		}
		return quaternion<T>(0.0, 0.0, 0.0, 0.0);
	}


	inline quaternion<T> conjugate() const {
		return conj();
	}

	inline quaternion<T> inverse() const {
		return inv();
	}

	inline T magnitude() const {
		return len();
	}

	/// Returns the euler angles from the quaternion rotation
	void getEulerAngles(T& yaw, T& pitch, T& roll) {

		//	ZYX Rotation Sequence (Yaw-Pitch-Roll)

		T qwqw = w*w;
		T qxqx = x*x;
		T qyqy = y*y;
		T qzqz = z*z;

		T mat11 = qwqw + qxqx - qyqy - qzqz;
		T mat12 = static_cast<T>(2.0)*(x*y + w*z);
		T mat13 = static_cast<T>(2.0)*(x*z - w*y);
		T mat23 = static_cast<T>(2.0)*(y*z + w*x);
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

	/// Returns the rotation matrix from the quaternion rotation
	tmath::matrix<T, 3, 3> getMatrix() {
		tmath::matrix<T, 3, 3> m;
		T qwqw = w*w;
		T qxqx = x*x;
		T qyqy = y*y;
		T qzqz = z*z;
		T qxqw = x*w;
		T qyqw = y*w;
		T qzqw = z*w;
		T qxqy = x*y;
		T qxqz = x*z;
		T qyqz = y*z;

		m.xx = static_cast<T>(1.0) - static_cast<T>(2.0) * (qyqy + qzqz);
		m.xy = static_cast<T>(2.0) * (qxqy + qzqw);
		m.xz = static_cast<T>(2.0) * (qxqz - qyqw);

		m.yx = static_cast<T>(2.0) * (qxqy - qzqw);
		m.yy = static_cast<T>(1.0) - static_cast<T>(2.0) * (qxqx + qzqz);
		m.yz = static_cast<T>(2.0) * (qyqz + qxqw);

		m.zx = static_cast<T>(2.0) * (qxqz + qyqw);
		m.zy = static_cast<T>(2.0) * (qyqz - qxqw);
		m.zz = static_cast<T>(1.0) - static_cast<T>(2.0) * (qxqx + qyqy);

		return m;
	}
};

}

#endif

