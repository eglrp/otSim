/**
\file     cmatrix.h
\brief    'c' functions for vector and matrix operations.
\author   Glenn D. MacGougan (GDM)
\date     2009-01-08
\version  0.07 Beta

\b Version \b Information \n
This is the open source version (BSD license). The Professional Version
is avaiable via http://www.zenautics.com. The Professional Version
is highly optimized using SIMD and includes optimization for multi-core
processors.

\b License \b Information \n
Copyright (c) 2008, Glenn D. MacGougan, Zenautics Technologies Inc. \n

Redistribution and use in source and binary forms, with or without
modification, of the specified files is permitted provided the following
conditions are met: \n

- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer. \n
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution. \n
- The name(s) of the contributor(s) may not be used to endorse or promote
products derived from this software without specific prior written
permission. \n

THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

\b NOTES: \n
This code was developed using rigourous unit testing for every function
and operation. Despite any rigorous development process, bugs are
inevitable. Please report bugs and suggested fixes to glenn @ zenautics.com.\n
*/

#ifndef ZENUATICS_MTX_H
#define ZENUATICS_MTX_H

#ifdef __cplusplus
extern "C"
{
#endif

	typedef int BOOL;

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (1)
#endif

	/// \brief  A complex data struct.
	typedef struct
	{
		double re; //!< The real part.
		double im; //!< The imaginary part.
	} stComplex;

	/// \brief  The deep level matrix struct. The matrix is either real or complex.
	typedef struct
	{
		unsigned   nrows;   //!< The number of rows in the matrix.
		unsigned   ncols;   //!< The number of columns in the matrix.
		BOOL       isReal;  //!< This indicates if is the matrix real or complex.
		double     **data;  //!< This is a pointer to an array of double column vectors.
		stComplex  **cplx;  //!< Thsi is a pointer to an array of complex column vectors.
		char      *comment; //!< This is a comment string (if applicable).
	} MTX;


	/// \brief  This function must be called first by users of cmatrix!
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Initialize_MTXEngine();


	/// \brief  This function is used to set if matrices that are single 
	///         elements (1x1) are treated as scalars for math operations
	///         or whether the regular matrix rules apply. THIS IS ENABLED
	///         BY DEFAULT.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Enable1x1MatricesForTreatmentAsScalars(BOOL enable);

	/// \brief  Is this a null matrix?
	///
	/// \return TRUE if the matrix is null, FALSE otherwise.
	BOOL MTX_isNull(const MTX *M);


	/// \brief  Are matrices A & B conformal for multiplication, real * real
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_isConformalForMultiplication(const MTX *A, const MTX *B);

	/// \brief  Are matrices A & B conformat for addition/subtraction, real + real
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_isConformalForAddition(const MTX *A, const MTX *B);

	/// \brief  Is this a square matrix?
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_isSquare(const MTX *A);

	/// \brief  are A and B the same size?
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_isSameSize(const MTX *A, const MTX *B);

	/// \brief  Initialize a MTX matrix struct to appropriate zero values. This must always be called for proper operation!
	/// \code
	/// MTX matrix;
	/// MTX_Init( &matrix );
	/// \endcode
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Init(MTX *M);



	/// \brief  Set the matrix comment string
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SetComment(MTX *M, const char *comment);

	/// \brief  Clear the matrix data from memory if dynamically allocated. Zero the struct members.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Free(MTX *M);

	/// \brief  Allocate matrix data (set to zero).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Calloc(MTX *M, const unsigned nrows, const unsigned ncols, const BOOL isReal);

	/// \brief  Allocate matrix data (not set to zero).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Malloc(MTX *M, const unsigned nrows, const unsigned ncols, const BOOL isReal);

	/// \brief  Set a scalar value in the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SetValue(MTX *M, const unsigned row, const unsigned col, const double value);

	/// \brief  Set a complex value in the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SetComplexValue(MTX *M, const unsigned row, const unsigned col, const double re, const double im);

	/// \brief  Matrix M = Re + Im*i, where Re and Im are real matrices.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Complex(MTX *M, const MTX *Re, const MTX *Im);

	/// \brief  Set the specified column in Matrix M to Re + Im*i, where Re and Im are real matrices.
	/// The dimensions of M must already be valid.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SetComplexColumn(MTX *M, const unsigned col, const MTX *Re, const MTX *Im);

	/// \brief  Convert a real matrix to a complex matrix
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ConvertRealToComplex(MTX *M);

	/// \brief  Convert a complex marix to a real matrix using only the imaginary component A = real(B).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ConvertComplexToReal(MTX *M);

	/// \brief  Convert a complex marix to a real matrix using only the imaginary component A = imag(B).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ConvertComplexToImag(MTX *M);

	/// \brief  Extract the real component of matrix M.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Real(const MTX *M, MTX *Re);

	/// \brief  Check if the matrix contains only real values. 
	/// Alter the matrix if it is stored as complex and only has real values.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_isReal(MTX *M, BOOL *isReal);

	/// \brief  Extract the real component of column col of matrix M.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RealColumn(const MTX *M, const unsigned col, MTX *Re);

	/// \brief  Extract the imaginary component of matrix M.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Imag(const MTX *M, MTX *Im);

	/// \brief  Extract the imaginary component of column col of matrix M.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ImagColumn(const MTX *M, const unsigned col, MTX *Im);

	/// \brief  If M is a real matrix, Magnitude is a copy.
	/// If M is a complex matrix, Magnitude is a real matrix = sqrt( re*re + im*im ).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Magnitude(const MTX *M, MTX *Magnitude);

	/// \brief  If M is a real matrix, Phase is a zero matrix.
	/// If M is a complex matrix, Phase is a real matrix = atan2(im,re).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Phase(const MTX *M, MTX *Phase);

	/// \brief  If M is a real matrix, nothing is done.
	/// If M is a complex matrix, the conjugate is set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Conjugate(MTX *M);

	/// \brief  Remove a single column from the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RemoveColumn(MTX *M, const unsigned col);

	/// \brief  remove all the columns 'after' the column index given.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RemoveColumnsAfterIndex(MTX *dst, const unsigned col);

	/// \brief  insert a column into another matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_InsertColumn(MTX *dst, const MTX *src, const unsigned dst_col, const unsigned src_col);

	/// \brief  Add a column to the Matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_AddColumn(MTX *dst, const MTX *src, const unsigned src_col);

	/// \brief  Combine two matrices with the same nrows, A becomes A|B,
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Concatonate(MTX *dst, const MTX *src);

	/// \brief  A becomes A|0|0|0|.. etc      
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_AddZeroValuedColumns(MTX *dst, const unsigned nr_new_cols);


	/// \brief  Redimension the matrix, original data is saved in place, new data is set to zero.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Redim(MTX *dst, const unsigned nrows, const unsigned ncols);

	/// \brief  Resize the matrix, original data is lost, new data is set to zero, must specify if the matrix is real or complex.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Resize(MTX *dst, const unsigned nrows, const unsigned ncols, const BOOL isReal);



	/// \brief  Copy the src data to dst matrix, resize dst if possible & necessary.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Copy(const MTX *src, MTX *dst);

	/// \brief  Copy the src matrix data [m cols x n rows] to dst vector [1 col x m*n rows], resize dst if possible & necessary.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_CopyIntoColumnWiseVector(const MTX *src, MTX *dst);

	/// \brief  Set the dst matrix from the static 'c' style matrix indexed by mat[i*ncols + j].
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SetFromStaticMatrix(MTX *dst, const double mat[], const unsigned nrows, const unsigned ncols);

	/// \brief  Copy the src data in column col to dst matrix, resize dst if possible & necessary.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_CopyColumn(const MTX *src, const unsigned col, MTX *dst);

	/// \brief  Copy the src data in row, row, to dst matrix, resize dst if possible & necessary.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_CopyRow(const MTX *src, const unsigned row, MTX *dst);

	/// \brief  Copy the src data in row 'row' (1xn) to dst matrix (nx1), resize dst if possible & necessary.
	/// dst becomes (nx1).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_CopyRowIntoAColumnMatrix(const MTX *src, const unsigned row, MTX *dst);

	/// \brief  Insert a submatrix (src) into dst, starting at indices dst(row,col).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_InsertSubMatrix(MTX *dst, const MTX *src, const unsigned dst_row, const unsigned dst_col);

	/**
	\brief  Extract a submatrix (dst) from this matrix from (inclusive)
	the rows and columns specified.
	\code
	MTX A;
	MTX B;
	BOOL result;
	MTX_Init( &A );
	MTX_Init( &B );

	result = MTX_SetFromMatrixString( &A, "[1 2 3; 4 5 6; 7 8 9]" );
	result = MTX_ExtractSubMatrix( &A, &B, 1, 0, 2, 2 );
	// B == [4 5 6; 7 8 9]
	\endcode

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_ExtractSubMatrix(
		const MTX* src,          //!< The source matrix.                        
		MTX* dst,                //!< The destination matrix to contain the submatrix.
		const unsigned from_row, //!< The zero-based index for the from row.
		const unsigned from_col, //!< The zero-based index for the from column.
		const unsigned to_row,   //!< The zero-based index for the to row.
		const unsigned to_col    //!< The zero-based index for the to column.
	);


	/// \brief  Zero the entire matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Zero(MTX *dst);

	/// \brief  Zero all elements in a specified column.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ZeroColumn(MTX *dst, const unsigned col);

	/// \brief  Zero all elements in a specified row.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ZeroRow(MTX *dst, const unsigned row);

	/// \brief  Fill the matrix with the given value.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Fill(MTX *dst, const double value);

	/// \brief  Fill the matrix with the given complex value.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FillComplex(MTX *dst, const double re, const double im);

	/// \brief  Fill the matrix column with the given value.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FillColumn(MTX *dst, const unsigned col, const double value);

	/// \brief  Fill the matrix column with the given complex value.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FillColumnComplex(MTX *dst, const unsigned col, const double re, const double im);

	/// \brief  Fill the matrix row with the given value.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FillRow(MTX *dst, const unsigned row, const double value);

	/// \brief  Fill the matrix row with the given complex value.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FillRowComplex(MTX *dst, const unsigned row, const double re, const double im);

	/// \brief  Reverse the order of elements of a column.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FlipColumn(MTX *M, const unsigned col);

	/// \brief  Reverse the order of elements of a row.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FlipRow(MTX *M, const unsigned row);

	/// \brief  Set the matrix to an identity.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Identity(MTX *dst);

	/// \brief  Force this square matrix to be symmetric 
	///         by M = (M + T.())/2 using minimal operations.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ForceSymmetric(MTX *M);

	/// \brief  Transpose the matrix src into the matris dst.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Transpose(const MTX *src, MTX *dst);

	/// \brief  Transpose the matrix as an inplace operation.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_TransposeInplace(MTX *M);


	/// \brief  Round the matrix elements to the specified precision.\n
	/// e.g. precision = 0    1.8    -> 2\n
	/// e.g. precision = 1,   1.45   -> 1.5\n
	/// e.g. precision = 2    1.456  -> 1.46\n
	/// e.g. precision = 3,   1.4566 -> 1.457\n
	/// precision has a maximum of 32. After which no rounding occurs.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Round(MTX *M, const unsigned precision);

	/// \brief  Round the matrix elements to the nearest integers towards minus infinity.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Floor(MTX *M);

	/// \brief  Round the matrix elements to the nearest integers towards infinity.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Ceil(MTX *M);

	/// \brief  Round the matrix elements to the nearest integers towards zero. 
	///         Sometimes known as trunc().
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Fix(MTX *M);


	/// \brief  Set the destination matrix to be 1.0 minus the source matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_OneMinus(const MTX* src, MTX *dst);


	/// \brief  Determine the matrix file delimiter and if a comment line is available.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_DetermineFileDelimiter(
		const char *path,    //!< path to the input file
		char *delimiter,     //!< delimiter, 'b' is binary
		BOOL *hasComment,    //!< BOOL to indicate if a comment line is present
		char **comment       //!< pointer to a string to store the comment line, *comment memory must be freed later.
	);

	/// \brief  Determine the size of a file.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_DetermineFileSize(const char *path, unsigned *size);

	/// \brief  Determine the number of columns in the data string provided.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_DetermineNumberOfColumnsInDataString(const char *datastr, unsigned *ncols);

	/// \brief  Determine the number of columns in the complex data string provided. 
	/// The delimiter is needed, 'w' indicates whitespace.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_DetermineNumberOfColumnsInDataStringCplx(const char *datastr, const char delimiter, unsigned *ncols);



	/// \brief  Read a real-only matrix from a file (ASCII formatted, any common delimiters).
	/// This function will also read in MTX BINARY formatted files.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ReadFromFileRealOnly(MTX *M, const char *path);


	/// \brief  Read either a real or complex matrix from a file (ASCII formatted, any common delimiters).
	/// This function will also read in MTX BINARY formatted files.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ReadFromFile(MTX *M, const char *path);


	/// \brief  Set the matrix from a matrix string.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SetFromMatrixString(MTX *M, const char *strMatrix);



	/// \brief  Convert a value to a string with the specified width and precision.
	/// analogous to sprintf( ValueBuffer, "%'blank''-'width.precision'g'", value );
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ValueToString(
		const double value,             //!< The double value to output.
		const unsigned width,           //!< The width of the field.
		const unsigned precision,       //!< The precision, %g style.
		const BOOL isReal,              //!< The the value the real part or the imaginary part.
		const BOOL alignLeft,           //!< Align the output left (for real data only).
		char *ValueBuffer,              //!< The output buffer.
		const unsigned ValueBufferSize  //!< The size of the output buffer.
	);

	/// \brief  Print the matrix to a file with specifed width and precision.
	/// MTX_PrintAutoWidth is recommended over this function, "%'blank''-'width.precision'g'".
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Print(const MTX *M, const char *path, const unsigned width, const unsigned precision, const BOOL append);

	/// \brief  Print the matrix to a buffer of maxlength with specifed width and precision.
	/// MTX_PrintAutoWidth is recommended over this function, "%'blank''-'width.precision'g'".
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Print_ToBuffer(const MTX *M, char *buffer, const unsigned maxlength, const unsigned width, const unsigned precision);

	/// \brief  Print the matrix to a file with automatically determined column width.
	/// and the specified precision, uses "%'blank''-'autowidth.precision'g'".
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PrintAutoWidth(const MTX *M, const char *path, const unsigned precision, const BOOL append);

	/// \brief  Print the matrix to stdout with automatically determined column width.
	/// and the specified precision, uses "%'blank''-'autowidth.precision'g'".
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PrintStdoutAutoWidth(const MTX *M, const unsigned precision);

	/// \brief  Print the matrix to a buffer of maxlenth with automatically determined column width.
	/// and the specified precision, uses "%'blank''-'autowidth.precision'g'".
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PrintAutoWidth_ToBuffer(const MTX *M, char *buffer, const unsigned maxlength, const unsigned precision);

	/// \brief  Print the matrix to a file with specifed precision and delimiter.
	/// Use MTX_PrintAutoWidth if print using whitespace as a delimiter is required, uses "%.precision'g'"
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PrintDelimited(const MTX *M, const char *path, const unsigned precision, const char delimiter, const BOOL append);

	/// \brief  Print the matrix to a file with specifed precision and delimiter.
	/// Use MTX_PrintAutoWidth if print using whitespace as a delimiter is required, uses "%.precision'g'".
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PrintDelimited_ToBuffer(const MTX *M, char *buffer, const unsigned maxlength, const unsigned precision, const char delimiter);

	/// \brief  Print a row to a string buffer.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PrintRowToString(const MTX *M, const unsigned row, char *buffer, const unsigned maxlength, const int width, const int precision);


	////
	// Math operations

	/// \brief  Adds a scalar double to matrix M, ie: M += 5.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Add_Scalar(MTX *M, const double scalar);

	/// \brief  Adds a scalar complex to matrix M, ie: M += (5 + 3i).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Add_ScalarComplex(MTX *M, const double re, const double im);

	/// \brief  Subtracts a scalar double from matrix M, ie: M -= 5.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Subtract_Scalar(MTX *M, const double scalar);

	/// \brief  Subtracts a scaler complex from matrix M, ie: M -= (5+3i).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Subtract_ScalarComplex(MTX *M, const double re, const double im);

	/// \brief  Multiply M with a double scalar inplace, ie: M *= 5.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Multiply_Scalar(MTX *M, const double scalar);

	/// \brief  Multiply M with a complex scalar inplace, ie: M *= (5+3i).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Multiply_ScalarComplex(MTX *M, const double re, const double im);

	/// \brief  Divide M by scaler double inplace, ie: M /= 5.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Divide_Scalar(MTX *M, const double scalar);

	/// \brief  Divide M by scaler complex inplace, ie: M /= (5+3i).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Divide_ScalarComplex(MTX *M, const double re, const double im);

	/// \brief  Change the sign of all the data in the matrix. M *= -1.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Negate(MTX *M);


	/// \brief  Computes the absolute value of each element in the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Abs(MTX *M);

	/// \brief  Compute the arc-cosine of each element of the matrix inplace.
	///         Complex results are obtained if elements are greater than abs(1).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_acos(MTX *M);

	/// \brief  Compute the phase angle in radians of the elements in the matrix.
	/// If all elements are real, the results are 0.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_angle(MTX *M);

	/// \brief  Compute the arc-sine of each element of the matrix inplace.
	///         Complex results are obtained if elements are greater than abs(1).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_asin(MTX *M);


	/// \brief  Computes the value^2 of each element in the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Sqr(MTX *M);

	/// \brief  Computes the sqrt(value) of each element in the matrix.
	/// A real matrix is converted to complex if any elements are negative.
	/// e.g. sqrt(-1) = -i.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Sqrt(MTX *M);

	/// \brief  If real, computes the exp(value) of each element in the matrix.
	/// If complex, computes exp(M) = exp(real)*(cos(imag)+i*sin(imag)).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Exp(MTX *M);

	/// \brief  Create an indentity matrix with nrows and ncols.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Eye(MTX *M, const unsigned nrows, const unsigned ncols);

	/// \brief  Computes the natural logarithm, ln(value) of each element in the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Ln(MTX *M);

	/// \brief  Raise all elements in src^(power_re + power_im*i) and store in dst.
	/// If power is just real, power_im = 0.0.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Pow(const MTX *src, MTX *dst, const double power_re, const double power_im);

	/// \brief  Raise all elements in src^(power_re + power_im*i).
	/// If power is just real, power_im = 0.0.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PowInplace(MTX *src, const double power_re, const double power_im);


	/// \brief  Computes the arctan, atan(value) of each element in the matrix
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_atan(MTX *M);

	/// \brief  Add +1.0 to all elements, e.g. M++.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Increment(MTX *M);

	/// \brief  Subtract 1.0 from all elements, e.g. M--.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Decrement(MTX *M);

	/// \brief  Add A += B, inplace.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Add_Inplace(MTX *A, const MTX *B);

	/// \brief  Subtract A -= B, inplace.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Subtract_Inplace(MTX *A, const MTX *B);

	/// \brief  Multiply A = B*A, inplace.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PreMultiply_Inplace(MTX *A, const MTX *B); // A = B*A

														/// \brief  Multiply A = tranpose(B)*A, inplace.
														///
														/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_TransposePreMultiply_Inplace(MTX *A, const MTX *B); // A = tranpose(B)*A

																 /// \brief  Multiply A = A*B, inplace.
																 ///
																 /// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PostMultiply_Inplace(MTX *A, const MTX* B); // A = A*B

														 /// \brief  Multiply A = A*transpose(B), inplace.
														 ///
														 /// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PostMultiplyTranspose_Inplace(MTX *A, const MTX* B); // A = A*tranpose(B)


																  /// \brief  Dot multiply A .*= B, inplace (A.data[col][row] = A.data[col][row]*B.data[col][row]).
																  ///
																  /// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_DotMultiply_Inplace(MTX *A, const MTX *B);

	/// \brief  Dot divide A ./= B, inplace (A.data[col][row] = A.data[col][row]/B.data[col][row]).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_DotDivide_Inplace(MTX *A, const MTX *B);



	/// \brief  Add A = B+C.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Add(MTX *A, const MTX *B, const MTX *C);

	/// \brief  Subtract A = B-C.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Subtract(MTX *A, const MTX *B, const MTX *C);

	/// \brief  Multiply A = B*C.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Multiply(MTX *A, const MTX *B, const MTX *C);

	/// \brief  Multiply A = transpose(B)*C.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_TransposeMultiply(MTX *A, const MTX* B, const MTX* C);

	/// \brief  Multiply A = B*transpose(C).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MultiplyTranspose(MTX *A, const MTX* B, const MTX* C); // A = B*transpose(C)

																	/// \brief  Rest if A == B to within the specified tolerance.
																	///
																	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_IsEqual(const MTX *A, const MTX *B, const double tolerance, BOOL *isEqual);


	/// \brief  Add this matrix and an identity matrix. Adds 1.0 to the diagonal even if not square.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_AddIdentity(const MTX *src, MTX *dst);

	/// \brief  Add this matrix and an identity matrix. Adds 1.0 to the diagonal even if not square.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_AddIdentity_Inplace(MTX *src);

	/// \brief  Subtract an identity matrix from this matrix. Subtracts 1.0 from the diagonal even if not square.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinusIdentity(const MTX *src, MTX *dst);

	/// \brief  Subtract an identity matrix from this matrix. Subtracts 1.0 from the diagonal even if not square.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinusIdentity_Inplace(MTX *src);

	/// \brief  Subtract this matrix from an identity matrix. Subtracts the diagonal from 1.0 even if not square.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_IdentityMinus(const MTX *src, MTX *dst);

	/// \brief  Subtract this matrix from an identity matrix. Subtracts the diagonal from 1.0 even if not square.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_IdentityMinus_Inplace(MTX *src);


	/// \brief  Difference and approximte derivative for column col.
	/// The Diff is the column difference vector.
	/// diff = col[1:N-2] - col[0:N-1].
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnDiff(const MTX *M, MTX *Diff, const unsigned col);

	/// \brief  Difference and approximate derivative.
	/// The Diff matrix is composed of the column difference vectors.
	/// for(i=0:M-1){ diff_i = col_i[1:N-2] - col_i[0:N-1] }
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Diff(const MTX *M, MTX *Diff);



	//// 
	// Statistics

	/// \brief  Computes the maximum element in the specified column and its index.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	/// If there are several equal maximum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxColIndex(const MTX *M, const unsigned col, double *re, double *im, unsigned *row);

	/// \brief  Computes the maximum element in the specified row and its index.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	/// If there are several equal maximum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxRowIndex(const MTX *M, const unsigned row, double *re, double *im, unsigned *col);


	/// \brief  Computes the minimum element in the specified column and its index.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	/// If there are several equal minimum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinColIndex(const MTX *M, const unsigned col, double *re, double *im, unsigned *row);

	/// \brief  Computes the minimum element in the specified row and its index.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	/// If there are several equal minimum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinRowIndex(const MTX *M, const unsigned row, double *re, double *im, unsigned *col);


	/// \brief  Computes the absolute maximum element in the specified column and its index.
	/// If there are several equal maximum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxAbsColIndex(const MTX *M, const unsigned col, double *value, unsigned *row);

	/// \brief  Computes the absolue maximum element in the specified row and a its column index.
	/// If there are several equal maximum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxAbsRowIndex(const MTX *M, const unsigned row, double *value, unsigned *col);

	/// \brief  Computes the absolute minimum element in the specified column and its index.
	/// If there are several equal minimum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinAbsColIndex(const MTX *M, const unsigned col, double *value, unsigned *row);

	/// \brief  Computes the absolute minimum element in the specified row and its index.
	/// If there are several equal minimum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinAbsRowIndex(const MTX *M, const unsigned row, double *value, unsigned *col);


	/// \brief  Computes the maximum element in the specified column.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxColumn(const MTX *M, const unsigned col, double *re, double *im);

	/// \brief  Computes the maximum element in the specified row.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxRow(const MTX *M, const unsigned row, double *re, double *im);


	/// \brief  Computes the minimum element in the specified column.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinColumn(const MTX *M, const unsigned col, double *re, double *im);


	/// \brief  Computes the minimum element in the specified row.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinRow(const MTX *M, const unsigned row, double *re, double *im);


	/// \brief  Computes the absolute maximum element in the specified column.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxAbsColumn(const MTX *M, const unsigned col, double *value);

	/// \brief  Computes the absolute maximum element in the specified row.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxAbsRow(const MTX *M, const unsigned row, double *value);


	/// \brief  Computes the absolute minimum element in the specified column.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinAbsColumn(const MTX *M, const unsigned col, double *value);

	/// \brief  Computes the absolute minimum element in the specified row.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinAbsRow(const MTX *M, const unsigned row, double *value);


	/// \brief  Computes the absolute maximum element for the entire matrix and its row and column index.
	/// If there are several equal maximum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxAbsIndex(const MTX *M, double* value, unsigned *row, unsigned *col);

	/// \brief  Computes the maximum element for the entire matrix and its row and column index.
	/// If there are several equal maximum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxIndex(const MTX *M, double *re, double *im, unsigned *row, unsigned *col);

	/// \brief  Computes the absolute maximum element for the entire matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MaxAbs(const MTX *M, double* value);

	/// \brief  Computes the maximum element for the entire matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Max(const MTX *M, double *re, double *im);


	/// \brief  Computes the absolute minimum element for the entire matrix and its row and column index.
	/// If there are several equal minimum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinAbsIndex(const MTX *M, double* value, unsigned *row, unsigned *col);

	/// \brief  Computes the minimum element for the entire matrix and its row and column index.
	/// If there are several equal minimum elements, the first index from the beginning is returned.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinIndex(const MTX *M, double *re, double *im, unsigned *row, unsigned *col);

	/// \brief  Computes the absolute minimum element for the entire matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MinAbs(const MTX *M, double* value);

	/// \brief  Computes the minimum element for the entire matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Min(const MTX *M, double *re, double *im);


	/// \brief  Computes the range of the data in the specified column. 
	/// Range = MaxVal - MinVal.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnRange(const MTX *M, const unsigned col, double *re, double *im);


	/// \brief  Computes the range of the data in the specified row. 
	/// Range = MaxVal - MinVal.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RowRange(const MTX *M, const unsigned row, double *re, double *im);

	/// \brief  Computes the range of the data in the matrix. 
	/// Range = MaxVal - MinVal.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Range(const MTX *M, double *re, double *im);


	/// \brief  Computes the sum for the specified column.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnSum(const MTX *M, const unsigned col, double *re, double *im);

	/// \brief  Computes the sum of the absolute values for the specified column.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnSumAbs(const MTX *M, const unsigned col, double *value);


	/// \brief  Computes the sum for the specified row.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RowSum(const MTX *M, const unsigned row, double *re, double *im);

	/// \brief  Computes the sum of the data in the matrix .
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Sum(const MTX *M, double *re, double *im);



	/// \brief  Computes the sample mean for the specified column.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnMean(const MTX *M, const unsigned col, double *re, double *im);

	/// \brief  Computes the sample mean for the specified row.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RowMean(const MTX *M, const unsigned row, double *re, double *im);

	/// \brief  Computes the sample mean for the matrix.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Mean(const MTX *M, double *re, double *im);




	/// \brief  Computes the sample standard deviation for the specified column.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnStdev(const MTX *M, const unsigned col, double *value);

	/// \brief  Computes the sample standard deviation for the specified row.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RowStdev(const MTX *M, const unsigned row, double *value);

	/// \brief  Computes the sample standard deviation for the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Stdev(const MTX *M, double *value);



	/// \brief  Computes the sample variance for the specified column.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnVar(const MTX *M, const unsigned col, double *value);

	/// \brief  Computes the sample variance for the specified row.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RowVar(const MTX *M, const unsigned row, double *value);

	/// \brief  Computes the sample variance for the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Var(const MTX *M, double *value);


	/// \brief  Computes the norm of the specified column.
	/// If real, norm = sqrt( sum( val*val ) ).
	/// If complex, norm = sqrt( sum( val*conjugate(val) ) ).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnNorm(const MTX *M, const unsigned col, double *value);

	/// \brief  Computes the norm of the specified row.
	/// If real, norm = sqrt( sum( val*val ) ).
	/// If complex, norm = sqrt( sum( val*conjugate(val) ) ).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RowNorm(const MTX *M, const unsigned row, double *value);

	/// \brief  Computes the norm of the matrix.
	/// If real, norm = sqrt( sum( val*val ) ).
	/// If complex, norm = sqrt( sum( val*conjugate(val) ) ).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Norm(const MTX *M, double *value);


	/// \brief  Computes the sample RMS value for the specified column.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnRMS(const MTX *M, const unsigned col, double *value);

	/// \brief  Computes the sample RMS value for the specified row.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RowRMS(const MTX *M, const unsigned row, double *value);

	/// \brief  Computes the sample RMS value for the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RMS(const MTX *M, double *value);


	/// \brief  Computes the sample skewness value for the specified column.
	/// The skewness is the third central moment divided by the cube of the standard deviation.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnSkewness(const MTX *M, const unsigned col, double *re, double* im);

	/// \brief  Computes the sample skewness value for the specified row.
	/// The skewness is the third central moment divided by the cube of the standard deviation.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_RowSkewness(const MTX *M, const unsigned row, double *re, double* im);

	/// \brief  Computes the sample skewness value for the matrix.
	/// The skewness is the third central moment divided by the cube of the standard deviation.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Skewness(const MTX *M, double *re, double* im);



	/// \brief  Computes the sample kurtosis value for the specified column.
	/// The kurtosis is the fourth central moment divided by fourth power of the standard deviation.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	/// To adjust the computed kurtosis value for bias, subtract 3 from the real component.
	/// Reference: http://en.wikipedia.org/wiki/Kurtosis.
	/// Reference: http://mathworld.wolfram.com/Kurtosis.html (kurtosis proper is computed).
	/// \return TRUE if successful, FALSE otherwise.
	// g_2 = \frac{m_4}{m_{2}^2} = \frac{n\,\sum_{i=1}^n (x_i - \overline{x})^4}{\left(\sum_{i=1}^n (x_i - \overline{x})^2\right)^2}.
	BOOL MTX_ColumnKurtosis(const MTX *M, const unsigned col, double *re, double *im);

	/// \brief  Computes the sample kurtosis value for the specified row.
	/// The kurtosis is the fourth central moment divided by fourth power of the standard deviation.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	/// To adjust the computed kurtosis value for bias, subtract 3 from the real component.
	/// Reference: http://en.wikipedia.org/wiki/Kurtosis.
	/// Reference: http://mathworld.wolfram.com/Kurtosis.html (kurtosis proper is computed).
	///
	/// \return TRUE if successful, FALSE otherwise.
	// g_2 = \frac{m_4}{m_{2}^2} = \frac{n\,\sum_{i=1}^n (x_i - \overline{x})^4}{\left(\sum_{i=1}^n (x_i - \overline{x})^2\right)^2}.
	BOOL MTX_RowKurtosis(const MTX *M, const unsigned row, double *re, double *im);


	/// \brief  Computes the sample kurtosis value for the matrix.
	/// The kurtosis is the fourth central moment divided by fourth power of the standard deviation.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	/// To adjust the computed kurtosis value for bias, subtract 3 from the real component.
	/// Reference: http://en.wikipedia.org/wiki/Kurtosis.
	/// Reference: http://mathworld.wolfram.com/Kurtosis.html (kurtosis proper is computed).
	///
	/// \return TRUE if successful, FALSE otherwise.
	// g_2 = \frac{m_4}{m_{2}^2} = \frac{n\,\sum_{i=1}^n (x_i - \overline{x})^4}{\left(\sum_{i=1}^n (x_i - \overline{x})^2\right)^2}.
	BOOL MTX_Kurtosis(const MTX *M, double *re, double *im);





	////
	// Matrix specific

	/// \brief  Computes the trace of M where M is a square matrix.
	/// Trace = Sum of diagonal elements.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Trace(const MTX *M, double *re, double *im);

	/// \brief  Sets the diagonal elements of M into D as a column vector
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Diagonal(const MTX *M, MTX *D);

	/// \brief  Sorts each column of M in ascending order.
	/// If complex, sorts based on magnitude.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SortAscending(MTX *M);

	/// \brief  Sorts each column of M in descending order.
	/// If complex, sorts based on magnitude.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SortDescending(MTX *M);

	/// \brief  Sorts a specific column in ascending order.
	/// If complex, sorts based on magnitude.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SortColumnAscending(MTX *M, const unsigned col);

	/// \brief  Sorts a specific column in descending order.
	/// If complex, sorts based on magnitude.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SortColumnDescending(MTX *M, const unsigned col);

	/// \brief  Sorts a specific column in ascending order and fills a MTX column vector with the sorted index.
	/// The index vector will be resized if index->nrows != M->nrows
	/// If complex, sorts based on magnitude.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SortColumnIndexed(MTX *M, const unsigned col, MTX *index);

	/// \brief  Sorts the entire matrix by a specific column.
	/// If complex, sorts based on magnitude.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SortByColumn(MTX *M, const unsigned col);



	/// \brief  Saves a matrix to the specified file path using a proprietary compressed format.
	/// ADVANCED EDITION ONLY!
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SaveCompressed(const MTX *M, const char *path);


	/// \brief  Loads a binary compressed matrix that was saved using the MTX_SaveCompressed function.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ReadCompressed(MTX *M, const char *path);

	/// \brief  Get attributes of the compressed file.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_GetCompressedFileAttributes(
		const char *path,
		unsigned* nrows,
		unsigned* ncols,
		BOOL* isReal
	);


	/// \brief  Read an ASCII matrix data file and save it using MTX_SaveCompressed.
	/// ADVANCED EDITION ONLY!
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_LoadAndSave(const char* infilepath, const char* outfilepath);

	/// \brief  Read an ASCII matrix data file and save it using MTX_SaveCompressed.
	/// This version saves the data to the same base filename and uses the .mtx extension.
	/// ADVANCED EDITION ONLY!
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_LoadAndSaveQuick(const char* infilepath);


	/// \brief  Alter the matrix, M, so that its data is within the startTime to the startTime+duration
	/// and compensate for any rollovers in the time system (e.g. GPS time in seconds rolls over
	/// at 604800.0 s). This function assumes that time is one of the matrix columns and requires
	/// this index, the timeColumn.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_TimeWindow(
		MTX* M,                     //!< Matrix to be altered
		const unsigned timeColumn,   //!< The column containing time
		const double startTime,      //!< The specified start time (inclusive)
		const double duration,       //!< The duration to include
		const double rolloverTime); //!< The potential time at which system time rolls over

									/// \brief  Alter the matrix, M, so that its data is within [startTime endTime].
									/// This function assumes that time is one of the matrix columns and requires
									/// this index, the timeColumn.
									///
									/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_TimeLimit(
		MTX* M,                     //!< Matrix to be altered
		const unsigned timeColumn,   //!< The column containing time
		const double startTime,      //!< The specified start time (inclusive)
		const double endTime);      //!< The duration to include


									/// \brief  This function matches matrices in time with specified precision
									/// where time is a column of each matrix. This function also
									/// allows time to rollover at a specified interval.
									///
									///    precision 0 = match to whole number \n
									///    precision 1 = match to nearest 0.1 \n
									///    precision 2 = match to nearest 0.01 \n
									///    etc. \n
									/// rolloverTime examples \n
									///     GPS time of week (s): rolloverTime= 604800.0 \n
									///     hours               : rolloverTime = 24.0 \n
									///     minutes             : rolloverTime = 60.0 \n
									///
									/// The time data must be non-decreasing but the time may rollover
									/// by the specified amount. 
									/// e.g. rolloverTime = 60.0 \n
									///      0,1,2,3,4,...59,60,1,2,5,10,60,1,2,3... \n
									///
									/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_TimeMatch(
		MTX *A,                      //!< The matrix with interpolation times
		const unsigned timeColumnA,  //!< The zero based column index for matrix A
		MTX *B,                      //!< The matrix to be interpolated
		const unsigned timeColumnB,  //!< The zero based column index for matrix B
		const unsigned precision,    //!< The rounding precision used for time matching, 0 = whole, 1 = 0.1, 2 = 0.01, etc
		const double rolloverTime    //!< The rollover time, e.g. 60 s for minute based timing, 0.0 means rollovers not allowed
	);



	/// \brief  This function interpolates Matrix B values by the times defined 
	/// in the column in Matrix A. Time must be increasing but times can 
	/// rollover with the specified rolloverTime.
	///
	/// This function returns A and B with the same number of rows and 
	/// time aligned time columns.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Interpolate(
		MTX *A,                     //!< The matrix with interpolation times
		const unsigned timeColumnA, //!< The zero based column index for matrix A
		MTX *B,                     //!< The matrix to be interpolated
		const unsigned timeColumnB, //!< The zero based column index for matrix B
		const double maxInterpolationInterval, //!< The largest interpolation interval allowed
		const double rolloverTime   //!< The rollover time, e.g. 60 s for minute based timing, 0.0 means rollovers not allowed
	);


	/// \brief  Compute the inverse, 1.0/x, inplace for each element
	///         of the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Inv(MTX *src);


	/// \brief  Compute the inplace inverse of the matrix.
	///         Uses fast closed form solutions for:
	///         Only for: 1x1, 2x2, 3x3
	///
	/// If the matrix is singular, the original matrix is unchanged.
	///
	/// \return TRUE if successful, FALSE if empty or has dimensions larger
	///         than 3x3, false if singular or not square
	BOOL MTX_InvertInPlaceClosedForm(MTX *M);

	/// \brief  Compute the inplace inverse of the matrix.
	///         Uses fast closed form solutions for:
	///         Only for: 1x1, 2x2, 3x3
	///
	/// \return TRUE if successful, FALSE if empty or has dimensions larger
	///         than 3x3, false if singular or not square
	BOOL MTX_InvertClosedForm(const MTX *M, MTX *dst);


	/// \brief  Compute the inplace inverse of a matrix.
	///
	/// The matrix is first tested to determine if it is a symmetric 
	/// positive-definite matrix. If so, Cholesky decomposition is used
	/// to facilitate the inversion of a lower triangular matrix. If the
	/// matrix is not symmetric and positive-definite robust inversion
	/// using gaussing elimination is attempted.
	///
	/// 3x3 matrices or smaller dimensions are computed using 
	/// MTX_InvertInPlaceClosedForm.
	/// 
	/// If the matrix is singular, the original matrix is unchanged.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_InvertInPlace(MTX *M);


	/// \brief  Compute the inverse of a matrix.
	///
	/// The matrix is first tested to determine if it is a symmetric 
	/// positive-definite matrix. If so, Cholesky decomposition is used
	/// to facilitate the inversion of a lower triangular matrix. If the
	/// matrix is not symmetric and positive-definite robust inversion
	/// using gaussing elimination is attempted.
	///
	/// 3x3 matrices or smaller dimensions are computed using 
	/// MTX_InvertClosedForm.
	/// 
	/// If the matrix is singular, the original matrix is unchanged.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Invert(const MTX *src, MTX *dst);

	/// \brief  Perfroms an inplace matrix inverse using Gaussian Elimination methods.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_InvertInPlaceRobust(MTX *M);

	/// \brief  Computes a moving average using N lead samples and M lagging samples
	/// for the specified column and stores it in dst.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ColumnMovAvg(const MTX *src, const unsigned col, const unsigned lead, const unsigned lag, MTX *dst);

	/// \brief  Computes a moving average using N lead samples and M lagging samples
	/// for the matrix and stores it in dst.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_MovAvg(const MTX *src, const unsigned lead, const unsigned lag, MTX *dst);



	/// \brief  Computes: InvATA = inverse( transpose(A) * A ).
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_ATAInverse(const MTX *A, MTX *InvATA);

	/// \brief  Compute the inplace inverse of a unit lower triangular matrix.
	/// An example unit lower triangular matrix is: \n
	///      A = [     1    0    0;          \n
	///               -2    2    0;          \n
	///                4   -3    3 ]; with   \n
	/// inv(A) = [     1    0    0;          \n
	///                1  1/2    0;          \n
	///             -1/3  1/2  1/3 ];        \n
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_LowerTriangularInverseInplace(MTX *src);


	/// \brief  Computes the determinatnt of the square matrix M.
	/// If the matrix is real, only the real value, re is set, im = 0. 
	/// If the matrix is complex, both re and im are set.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Det(const MTX *M, double *re, double *im);


	/// \brief  LU factorization.
	/// Performs a factorization to produce a unit lower  triangular matrix, L, 
	/// an upper triangular matrix, U, and permutation matrix P so that
	/// P*X = L*U.
	/// P, L and U are copmuted correctly if IsFullRank is set to true.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_LUFactorization(const MTX *src, BOOL *IsFullRank, MTX *P, MTX *L, MTX *U);


	/// \brief  Retrieve the elements of the matrix specified by the index vectors. 
	/// The index vectors must be nx1 real vectors.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_IndexedValues(const MTX *src, const MTX *row_index, const MTX *col_index, MTX *dst);


	/// \brief  Set the elements of the matrix specified by the index vectors. 
	/// The index vectors must be nx1 real vectors.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_SetIndexedValues(MTX *dst, const MTX *row_index, const MTX *col_index, const MTX *src);


	/// \brief  Compute the Fast Fourier Transform of each columns in the src matrix and
	/// store it in the dst matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FFT(const MTX *src, MTX *dst);

	/// \brief  Compute the inverse Fast Fourier Transform of each columns in the src matrix and
	/// store it in the dst matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_IFFT(const MTX *src, MTX *dst);

	/// \brief  Compute the Two-Dimensional Fast Fourier Transform of the src matrix and
	/// store it in the dst matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FFT2(const MTX *src, MTX *dst);

	/// \brief  Compute the Two-Dimensional Inverse Fast Fourier Transform of the src matrix and
	/// store it in the dst matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_IFFT2(const MTX *src, MTX *dst);


	/// \brief  Compute the inplace Fast Fourier Transform of each column of the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FFT_Inplace(MTX *src);

	/// \brief  Compute the inplace inverse Fast Fourier Transform of each column of the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_IFFT_Inplace(MTX *src);


	/// \brief  Compute the inplace two dimensional Fast Fourier Transform of the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_FFT2_Inplace(MTX *src);

	/// \brief  Compute the inplace two dimensional inverse Fast Fourier Transform of the matrix.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_IFFT2_Inplace(MTX *src);



	/// \brief  Compute the sine of each element in the matrix. Assumes elements are radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_sin(MTX *src);

	/// \brief  Compute the sin(pi*x)/(pi*) of each element in the matrix. 
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_sinc(MTX *src);

	/// \brief  Compute the hyperbolic sine of each element in the matrix. Assumes elements are radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_sinh(MTX *src);

	/// \brief  Compute the inverse hyperbolic sine of each element in the matrix. 
	/// Results in radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_asinh(MTX *src);

	/// \brief  Compute the cosine of each element in the matrix. Assumes elements are radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_cos(MTX *src);

	/// \brief  Compute the hyperbolic cosine of each element in the matrix. Assumes elements are radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_cosh(MTX *src);

	/// \brief  Compute the inverse hyperbolic cosine of each element in the matrix. 
	/// Results in radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_acosh(MTX *src);

	/// \brief  Compute the tangent of each element in the matrix. Assumes elements are radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_tan(MTX *src);

	/// \brief  Compute the hyperbolic tangent of each element in the matrix. Assumes elements are radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_tanh(MTX *src);

	/// \brief  Compute the inverse hyperbolic tangent of each element in the matrix. 
	/// Results in radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_atanh(MTX *src);


	/// \brief  Compute the cotangent of each element in the matrix. Assumes elements are radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_cot(MTX *src);

	/// \brief  Compute the hyperbolic cotangent of each element in the matrix. Assumes elements are radians.
	///
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_coth(MTX *src);



	/// \brief  Create a column vector [start:increment:end) beginning at start
	/// with step size of increment until less than or equal to end. 
	/// Note that arguments must be real scalars. \n
	/// e.g. a = 2:2:9   = [2; 4; 6; 8;] \n
	/// e.g. b = 2:-2:-9 = [2; 0; -2; -4; -6; -9;] \n
	///
	/// \return TRUE if successful, FALSE otherwise.    
	BOOL MTX_Colon(MTX *dst, const double start, const double increment, const double end);


	/** \brief  A very efficient method to remove rows and columns from the matrix.
	*
	* \code
	*  MTX A;
	*  unsigned rows[2];
	*  unsigned cols[2];
	*  MTX_Init(&A);
	*  MTX_Calloc( &A, 4, 4 );
	*  MTX_Identity( &A );
	*  A.data[0][0] = 100.0;
	*  A.data[2][1] = 10.0;
	*  A.data[1][2] = 20.0;
	*  // remove the first row and column and the third row and column.
	*  rows[0] = 0;
	*  rows[1] = 2;
	*  cols[0] = 0;
	*  cols[1] = 2;
	*  MTX_RemoveRowsAndColumns( &A, 2, (unsigned*)rows, 2 (unsigned*)cols );
	*  // A is now a 2x2 identity matrix.
	*  \endcode
	*
	*  \return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_RemoveRowsAndColumns(
		MTX *src,        //!< The pointer to the matrix object.
		const unsigned nrows,  //!< The number of rows to remove (the length of the rows array).
		const unsigned rows[], //!< The array of row indices to remove.
		const unsigned ncols,  //!< The number of columns to remove (the length of hte cols array).
		const unsigned cols[]
	);


	/**
	\brief  Sets the matrix as the NxN hilbert matrix. H_ij = 1.0 / (i+j-1.0) for i=1:N, j=1:N.

	\code
	MTX H;
	BOOL result;
	MTX_Init( &H );
	result = MTX_Hilbert( &H, 3);
	// H == "[1 1/2 1/3; 1/2 1/3 1/4; 1/3 1/4 1/5]";
	\endcode

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_Hilbert(MTX *src, const unsigned N);


	/**
	\brief Produce a matrix that is composed of pseudo-random numbers.
	Values are elements are standard normal distribution with mean zero,
	variance of one and standard of deviation one. N(0,1)

	\code
	MTX A;
	MTX_Init(&A);
	MTX_randn( 1000, 1 ); // create a standard-normal distributed random vector of 1000 rows by 1 column.
	\endcode

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_randn(
		MTX* M,
		const unsigned nrows,
		const unsigned ncols,
		const unsigned seed
	);

	/**
	\brief Produce a matrix that is composed of pseudo-random numbers.
	Values are elements are uniform distribution [0,1].

	\code
	MTX A;
	MTX_Init(&A);
	MTX_rand( 1000, 1 ); // create a uniformly distributed random vector of 1000 rows by 1 column.
	\endcode

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_rand(
		MTX* M,
		const unsigned nrows,
		const unsigned ncols,
		const unsigned seed
	);


	/// \brief  Test if a double value is NaN.
	BOOL MTX_IsNAN(double value);

	/// \brief  Test if a double value is +INF.
	BOOL MTX_IsPostiveINF(double value);

	/// \brief  Test if a double value is -INF.
	BOOL MTX_IsNegativeINF(double value);


	/// \brief  A quick plot, to a RLE compressed windows bitmap file, x_column vs y_column of the matrix M.
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_PlotQuick(MTX* M, const char* bmpfilename, const unsigned x_col, const unsigned y_col);


	/// \brief  These are the supported colors.
	typedef enum
	{
		MTX_WHITE = 0,
		MTX_BLACK = 1,
		MTX_BLUE = 2,
		MTX_GREEN = 3,
		MTX_PURPLE = 4,
		MTX_MAGENTA = 5,
		MTX_DARKBLUE = 6,
		MTX_INDIANRED = 7,
		MTX_BABYBLUE = 8,
		MTX_PAISLYBLUE = 9,
		MTX_LIGHTPURPLE = 10,
		MTX_DARKPURPLE = 11,
		MTX_GREYPURPLE = 12,
		MTX_BROWN = 13,
		MTX_RED = 14,
		MTX_PINK = 15,
		MTX_YELLOW = 16,
		MTX_ORANGE = 17,
		MTX_CYAN = 18,
		MTX_LIMEGREEN = 19,
		MTX_GREY = 20,
		MTX_LIGHTGREY = 21
	} MTX_enumColor;

	typedef struct
	{
		MTX* M;                 //!< A pointer to the series data matrix. 
		unsigned x_col;         //!< The series x data column.
		unsigned y_col;         //!< The series y data column.
		BOOL connected;         //!< Are the data points connected.
		MTX_enumColor color;    //!< The color of the data points/line.
		char* label;            //!< The series label, NULL if none.
		char* units;            //!< The series units, NULL if none.
		int precision;          //!< The number of significant digits in the statistics.
		BOOL markOutlierData;   //!< Should the outlier data be marked.
	} MTX_PLOT_structSeries;


	/// \brief  A container struct used in MTX_structAxisOptions.
	typedef struct
	{
		BOOL doNotUseDefault;
		double val;
	} MTX_structAxisSubOption;

	/// \brief  A container struct for axis options.
	typedef struct
	{
		MTX_structAxisSubOption lowerlimit;
		MTX_structAxisSubOption upperlimit;
		MTX_structAxisSubOption tickstart;
		MTX_structAxisSubOption ticksize;
		MTX_structAxisSubOption tickend;
	}MTX_structAxisOptions;


	/// \brief  Plot up to 12 series on one figure directly to an RLE compressed BITMAP file.
	/// \return TRUE if successful, FALSE otherwise.
	BOOL MTX_Plot(
		const char* bmpfilename,         //!< The output RLE compressed BITMAP filename.
		const char* title,               //!< The plot title (NULL if not used).
		const unsigned plot_height_cm,   //!< The plot height in cm.
		const unsigned plot_width_cm,    //!< The plot width in cm.
		const BOOL includeStats,         //!< A boolean to indicate if statistics info should be included on the plot.
		const BOOL isXGridOn,            //!< A boolean to indicate if the x grid lines are on.
		const BOOL isYGridOn,            //!< A boolean to indicate if the y grid lines are on.
		const char* xlabel,              //!< The x axis label (NULL if not used).
		const char* ylabel,              //!< The y axis label (NULL if not used).
		const MTX_structAxisOptions x,   //!< Limits and ticks for x.
		const MTX_structAxisOptions y,   //!< Limits and ticks for y.
		const MTX_PLOT_structSeries* series, //!< A pointer to an array of series structs.
		const unsigned nrSeries              //!< The number of series.
	);


	/**
	\brief  Swap the contents of matrix A with matrix B.

	\code
	MTX A,B;
	MTX_Init( &A );
	MTX_Init( &B );
	MTX_Malloc( &A, 2, 2, TRUE );
	MTX_Malloc( &B, 1, 1, TRUE );
	MTX_Fill( &A, 88.0 );
	MTX_Fill( &B, 44.0 );
	MTX_Swap( &A, &B );
	// A == [44], B == [88 88; 88 88]
	MTX_Free( &A );
	MTX_Free( &B );
	\endcode

	\return TRUE if succesful, FALSE otherwise.
	*/
	BOOL MTX_Swap(MTX* A, MTX *B);


#ifdef _DEBUG

	/* Take a filename and return a pointer to its final element.  This
	function is called on __FILE__ to fix a MSVC nit where __FILE__
	contains the full path to the file.  This is bad, because it
	confuses users to find the home directory of the person who
	compiled the binary in their warrning messages. */
	const char * _shortfile(const char *fname);
#define _SHORT_FILE_  _shortfile(__FILE__)

#if defined(_MSC_VER) && _MSC_VER < 1300 /* Windows compilers before VC7 don't have __FUNCTION__ or __LINE__. */
#define MTX_ERROR_MSG( msg ) { const char *themsg = msg; if( themsg != NULL ){ printf( "\n%s, %s\n", _SHORT_FILE_, themsg ); }else{ printf( "\n%s, %s, %d, Unknown Error\n", __FILE__, __FUNCTION__, __LINE__ ); } }
#else
#define MTX_ERROR_MSG( msg ) { const char *themsg = msg; if( themsg != NULL ){ printf( "\n%s, %s, %d, %s\n", _SHORT_FILE_, __FUNCTION__, __LINE__, themsg ); }else{ printf( "\n%s, %s, %d, Unknown Error\n", __FILE__, __FUNCTION__, __LINE__ ); } }
#endif

#else

	const char* _shortfile(const char *fname);
#define _SHORT_FILE_  _shortfile(__FILE__)
#define MTX_ERROR_MSG( msg ) {}

#endif


	/// \brief  Compute the LDLt decomposition of a square matrix. 
	///         This method avoids using square roots and can be used 
	///         for any square, full rank, symmetrical matrix.  
	///
	/// \return TRUE if succesful, FALSE otherwise. FALSE if not full rank.
	BOOL MTX_LDLt(
		MTX* src,           //!< src = L*D*Lt
		MTX *L,             //!< src = L*D*Lt
		MTX* d,             //!< src = L*D*Lt, d it the vector diagonal of D.
		BOOL checkSymmetric //!< Option to enable/disable checking the src matrix for symmetry. Runs faster if the input is known to be symmetric.
	);

	/// \brief  Compute the UDUt decomposition of a square matrix.
	///         This method avoids using square roots and can be used 
	///         for any square, full rank, symmetrical matrix.  
	///
	/// \return TRUE if succesful, FALSE otherwise. FALSE if not full rank.
	BOOL MTX_UDUt(
		MTX* src,           //!< src = U*D*Ut
		MTX *U,             //!< src = U*D*Ut
		MTX* d,             //!< src = U*D*Ut, d it the vector diagonal of D.
		BOOL checkSymmetric //!< Option to enable/disable checking the src matrix for symmetry.
	);


	/**
	\brief  Compute the error function (erf) for all values in the matrix inplace. \n
	erf(x) = 2/sqrt(pi) * [integral from 0 to x of]( e^(-t^2) )dt.

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_erf_Inplace(MTX* src);


	/**
	\brief  Compute the inverse error function (erfinv) for all values in the matrix inplace. \n
	y = erf(x), compute x given y, i.e. x = erfinv(y).

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_erfinv_Inplace(MTX* src);

	/**
	\brief  Compute the complementary error function (erfc) for all values in the matrix inplace.
	erfc(x) = 1 - erf(x) =  2/sqrt(pi) * [integral from x to inf of]( e^(-t^2) )dt.

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_erfc_Inplace(MTX* src);


	/**
	\brief  Compute the inverse complementary error function (erfcinv) for all values in the matrix inplace.

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_erfcinv_Inplace(MTX* src);


	/**
	\brief  Set the index vector so that it contains are the indices of values that are equal
	to the value specified with the given tolerance from the column of the src matrix.

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_find_column_values_equalto(
		const MTX* src,        //!< The source matrix to search.
		const unsigned col,    //!< The zero-based index of the column which is searched.
		MTX* indexVector,      //!< This is the index vector corresponding to the equal values in the source matrix.  
		const double re,       //!< The real part of the equal to value.
		const double im,       //!< The imaginary part of the equal to value.
		const double tolerance //!< The search tolerance. e.g. 1.0e-12.
	);

	/**
	\brief  Set the index vector so that it contains are the indices of values that are not equal
	to the value specified with the given tolerance from the column of the src matrix.

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_find_column_values_not_equalto(
		const MTX* src,        //!< The source matrix to search.
		const unsigned col,    //!< The zero-based index of the column which is searched.
		MTX* indexVector,      //!< This is the index vector corresponding to the values that are not equal in the source matrix.  
		const double re,       //!< The real part of the value.
		const double im,       //!< The imaginary part of the value.
		const double tolerance //!< The search tolerance. e.g. 1.0e-12.
	);


	/**
	\brief  Set the index vector so that it contains are the indices of values that are less then
	to the value specified with the given tolerance from the column of the src matrix.
	Complex matrix values are compared to the value by magnitude (i.e. sqrt(re*re+im*im)).

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_find_column_values_less_than(
		const MTX* src,        //!< The source matrix to search.
		const unsigned col,    //!< The zero-based index of the column which is searched.
		MTX* indexVector,      //!< This is the index vector of the values desired.  
		const double value     //!< The comparison value.   
	);


	/**
	\brief  Set the index vector so that it contains are the indices of values that are more then
	to the value specified with the given tolerance from the column of the src matrix.
	Complex matrix values are compared to the value by magnitude (i.e. sqrt(re*re+im*im)).

	\return TRUE if successful, FALSE otherwise.
	*/
	BOOL MTX_find_column_values_more_than(
		const MTX* src,        //!< The source matrix to search.
		const unsigned col,    //!< The zero-based index of the column which is searched.
		MTX* indexVector,      //!< This is the index vector of the values desired.  
		const double value     //!< The comparison value.   
	);





#ifdef __cplusplus
}
#endif


#endif // ZENUATICS_MTX_H
