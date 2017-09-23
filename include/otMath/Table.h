/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       Table.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef Table_H
#define Table_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <vector>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otMath {
template <typename T>
class Table;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif

typedef otMath::Table<double> dTable;
typedef otMath::Table<float> fTable;

namespace otMath {

enum class InterpMethod
{
	INTERP_LINEAR = 0, //1D, 2D, or 3D
	INTERP_NEAREST, //1D, 2D, or 3D
	INTERP_PCHIP,  //1D only - Piecewise Cubic Hermite Interpolating Polynomial
	INTERP_CUBIC, //1D or 2D
	INTERP_AKIMA, //1D or 2D
	INTERP_QUINTIC, //1D or 2D
	INTERP_BESSEL, //1D only
	INTERP_HERMITE, //1D only
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Table class implementation

//////////////////////////
/// 1-D Table example: ///
//////////////////////////

Table<double> new1DTable(2);

*new1DTable
<< 0.0 << 4.0
<< 1.0 << 5.0;

double value = new1DTable->interp(0.5);  //returns 4.5

//////////////////////////
/// 2-D Table example: ///
//////////////////////////

Table<double> new2DTable(2,2);

*new2DTable
		<< 0.0	<< 1.0
<< 0.0	<< 4.0	<< 5.0
<< 1.0	<< 5.0	<< 6.0;

double value = new2DTable->interp(0.5, 0.5); //returns 5.0

//////////////////////////
/// 3-D Table example: ///
//////////////////////////

Table<double> new2DTable1(2,2);

*new2DTable1
		<< 0.0	<< 1.0
<< 0.0	<< 4.0	<< 5.0
<< 1.0	<< 5.0	<< 6.0;

Table<double> new2DTable2(2,2);

*new2DTable2
		<< 0.0	<< 1.0
<< 0.0	<< 7.0	<< 8.0
<< 1.0	<< 8.0	<< 9.0;

//Two methods to create 3D table:

//METHOD 1
//make array of table pointers and a corresponding array of their breakpoints for the 3rd interpolation axis
Table<double>* newTablePointers[2] = { new2DTable1, new2DTable2 };
double newTableBreakPoints[2] = { 0, 1 };

//pass array of pointers, array of breakpoints, and size of each array
//MAKE SURE each array is the the same size
Table<double> new3DTable = new Table<double>(newTablesPointers, newTableBreakPoints, 2);


//METHOD 2
//make vector of table pointers and a corresponding vector of their breakpoints for the 3rd interpolation axis

std::vector<Table<double>*> newTablePointers;
newTablePointers.push_back(new2DTable1);
newTablePointers.push_back(new2DTable2);

std::vector<double> newTableBreakPoints;
newTableBreakPoints.push_back(0);
newTableBreakPoints.push_back(1);

//pass vector of pointers and vector of breakpoints
//MAKE SURE each vector is the same size
Table<double> new3DTable = new Table<double>(newTablesPointers, newTableBreakPoints);

double value = new3DTable->interp(0.5, 0.5, 0.5); //returns 6.5

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

template <typename T>
class MATH_API Table
{
public:
	/// Constructor takes 1 Dimension to make a 1D table
	Table(unsigned int numberRows, InterpMethod method = InterpMethod::INTERP_LINEAR);

	/// Constructor takes 2 Dimensions to make a 2D table
	Table(unsigned int numberRows, unsigned int numberColumns, InterpMethod method = InterpMethod::INTERP_LINEAR);

	/// Constructor takes a vector of table pointers and a corresponding vector of table breakpoint values to make a 3D table
	Table(const std::vector<Table<T>>& tables, const std::vector<T>& breakpoints);

	/// Constructor takes an array of table pointers and a corresponding array of table breakpoint values to make a 3D table
	Table(Table<T>* tables[], T breakpoints[], size_t arraySize);	//3D Table

	/// Copy constructor
	Table(const Table<T>& table);
	/// Copy assignment constructor
	const Table &operator=(const Table<T> &table);

	/// Destructor
	~Table();

	/// Get interpolated value from a 1D table
	T interp(T key, bool extrapolate = false) const;
	/// Get interpolated value from a 2D table
	T interp(T rowKey, T colKey, bool extrapolate = false) const;
	/// Get interpolated value from a 3D table
	T interp(T rowKey, T colKey, T tableKey, bool extrapolate = false) const;

	/// Get table element entry at a given row and column index
	T get(unsigned int row, unsigned int col) const;
	T operator()(unsigned int row, unsigned int col) const;

	/// Get the current interpolation method for the table
	InterpMethod getInterpolationMethod() const;
	/// Change the current interpolation method for the table
	void changeInterpolationMethod(InterpMethod method);

	/// Get the number of rows in the table
	unsigned int getNumRows() const;

	/// Get the number of columns in the table
	unsigned int getNumColumns() const;

	/// Get the number of tables in this root table
	unsigned int getNumTables() const;

	/// Stream operators to feed table data
	Table<T>& operator<<(const T n);
	Table<T>& operator<<(const int n);

private:
	class Impl;
	Impl* mImpl = nullptr;
};


} //namespace otMath

#endif //Table_H