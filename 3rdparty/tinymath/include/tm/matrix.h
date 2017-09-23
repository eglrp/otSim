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

#ifndef MATRIXNXM_H
#define MATRIXNXM_H

#include <iostream>
#include <tm/vectorn.h>

namespace tmath{

/**
	@class 			matrix
	@brief 			Class that represents a n x m matrix
	@author 		Cengiz Terzibas
*/

template<typename T, int N, int M>
class matrix {
public:
	typedef T value_type;
	T cmp[M][N];
	// constructors
	matrix<T,N,M>() {
		null();
	}
	matrix<T,N,M>(const T* mat) {
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				cmp[m][n] = mat[n + N*m];
	}
	matrix<T,N,M>(const matrix<T,N,M>& mat) {
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				cmp[m][n] = mat.cmp[m][n];
	}
	inline void null() {
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				cmp[m][n] = static_cast<T>(0.0);
	}
	// assignment operations
	inline matrix<T,N,M> operator+=(const matrix<T,N,M>& mat) {
		matrix<T,N,M> res;
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				cmp[m][n] += mat.cmp[m][n];
		return res;
	}
	inline matrix<T,N,M> operator-=(const matrix<T,N,M>& mat) {
		matrix<T,N,M> res;
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				cmp[m][n] -= mat.cmp[m][n];
		return res;
	}
	inline matrix<T,N,M> operator*=(const T &num) {
			for(int m = 0; m < M; ++m)
				for(int n = 0; n < N; ++n)
					cmp[m][n] *= num;
		return *this;
	}
	inline matrix<T,N,M> operator/=(const T& num) {
	  float val = (static_cast<T>(1.0)/num);
      for(int m = 0; m < M; ++m)
        for(int n = 0; n < N; ++n)
          cmp[m][n] /= num;
		return *this;
	}
	// stream operations
  friend std::ostream& operator<<(std::ostream& out, const matrix<T,N,M>& mat) {
		for(int m = 0; m < M; ++m) {
			for(int n = 0; n < N; ++n) {
				out << mat.cmp[m][n] << " " ;
			}
			out << std::endl;
		}
		return out;
	}
	// comparison operations
	inline const bool operator == (const matrix<T,N,M>& mat) const {
		bool res = true;
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				res &= mat.cmp[m][n] == cmp[m][n];
		return res;
	}
	inline const bool operator != ( const matrix<T,N,M>& m) const {
		return !(m == *this);
	}
	// unary operations
	inline const matrix<T,N,M> operator - () const {
		matrix<T,N,M> mat;
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				mat.cmp[m][n] = -cmp[m][n];
		return mat;
	}
	// binary operations
	inline friend matrix<T,N,M> operator+(const matrix<T,N,M>& mat1,const matrix<T,N,M>& mat2) {
		matrix<T,N,M> res;
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				res.cmp[m][n] = mat1.cmp[m][n] + mat2.cmp[m][n];
		return res;
	}
	inline friend matrix<T,N,M> operator-(const matrix<T,N,M>& mat1,const matrix<T,N,M>& mat2) {
		matrix<T,N,M> res;
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				res.cmp[m][n] = mat1.cmp[m][n] - mat2.cmp[m][n];
		return res;
	}
	inline const matrix<T,N,M> operator*(const T& num) const {
		matrix<T,N,M> res;
		for(int m = 0; m < M; ++m)
			for(int n = 0; n < N; ++n)
				res.cmp[m][n] = cmp[m][n]*num;
		return res;
	}
	friend inline const matrix<T,N,M> operator*( const T &s, const matrix<T,N,M>& mat ) {
		return mat * s;
	}
	friend inline matrix<T,N,M> operator*(const matrix<T,N,M>& m1, const matrix<T,N,M>& m2 )  {
      matrix<T,N,M> res;
      res.null();
      for(int m = 0; m < M; ++m)
        for(int n = 0; n < N; ++n)
          for(int j = 0; j < M; ++j)
              res.cmp[m][n] += m1.cmp[m][j]*m2.cmp[j][n];
      return res;
	}
	inline friend const vectorn<T,N> operator*(const matrix<T,N,M>& mat, const vectorn<T,M>& v) {
	  vectorn<T,N> res;
    for(int m = 0; m < M; ++m)
        for(int n = 0; n < M; ++n)
          res.cmp[m] += mat.cmp[n][m]*v.cmp[n];
		return res;

	}
	inline const matrix<T,N,M> operator/(const T& num) const {
    float val = (static_cast<T>(1.0)/num);
    matrix<T,N,M> res;
      for(int m = 0; m < M; ++m)
        for(int n = 0; n < N; ++n)
          res.cmp[m][n] = cmp[m][n]*val;
      return res;
	}
	// indexing operations
	inline void row(unsigned int pRow, const vectorn<T,M>& pV) {
		for(int m = 0; m < M; ++m)
			cmp[pRow][m] = pV[m];
	}
	inline void column(unsigned int pColumn, const vectorn<T,M>& pV) {
		for(int n = 0; n < N; ++n)
			cmp[n][pColumn] = pV[n];
	}
	inline const vectorn<T,N> row(int idx) {
	  vectorn<T,N> res;
	  for(int n = 0; n < N; ++n)
			res.cmp[n] = cmp[idx][n];
		return res;
	}
	inline const vectorn<T,M> column(int idx) {
	  vectorn<T,M> res;
	  for(int m = 0; m < M; ++m)
			res.cmp[m] = cmp[m][idx];
		return res;
	}
	// cast operations
	operator T*() {
		return &(cmp[0][0]);
	}
	operator const T*() const {
		return &(cmp[0][0]);
	}

};

}

#endif
