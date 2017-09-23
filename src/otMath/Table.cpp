/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       Table.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Table class implementation.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Table.h"
#include "Splines.hh"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otMath {

struct TableSplines
{
	void clearSplinePtrs()
	{
		pchipSpline_1D = nullptr;
		cubicSpline_1D = nullptr;
		akimaSpline_1D = nullptr;
		qunticSpline_1D = nullptr;
		besselSpline_1D = nullptr;
		hermiteSpline_1D = nullptr;

		cubicSpline_2D = nullptr;
		akimaSpline_2D = nullptr;
		qunticSpline_2D = nullptr;
	}

	void deleteSplines()
	{
		if (pchipSpline_1D) { delete pchipSpline_1D; pchipSpline_1D = nullptr; }
		if (cubicSpline_1D) { delete cubicSpline_1D; cubicSpline_1D = nullptr; }
		if (akimaSpline_1D) { delete akimaSpline_1D; akimaSpline_1D = nullptr; }
		if (qunticSpline_1D) { delete qunticSpline_1D; qunticSpline_1D = nullptr; }
		if (besselSpline_1D) { delete besselSpline_1D; besselSpline_1D = nullptr; }
		if (hermiteSpline_1D) { delete hermiteSpline_1D; hermiteSpline_1D = nullptr; }

		if (cubicSpline_2D) { delete cubicSpline_2D; cubicSpline_2D = nullptr; }
		if (akimaSpline_2D) { delete akimaSpline_2D; akimaSpline_2D = nullptr; }
		if (qunticSpline_2D) { delete qunticSpline_2D; qunticSpline_2D = nullptr; }
	}

	template <typename T>
	T evaluate(T val, InterpMethod method)
	{
		switch (method)
		{
		case InterpMethod::INTERP_PCHIP:
			if (pchipSpline_1D) return (T)pchipSpline_1D->eval((Splines::valueType)val);
			break;
		case InterpMethod::INTERP_CUBIC:
			if (cubicSpline_1D) return (T)cubicSpline_1D->eval((Splines::valueType)val);
			break;
		case InterpMethod::INTERP_AKIMA:
			if (akimaSpline_1D) return (T)akimaSpline_1D->eval((Splines::valueType)val);
			break;
		case InterpMethod::INTERP_QUINTIC:
			if (qunticSpline_1D) return (T)qunticSpline_1D->eval((Splines::valueType)val);
			break;
		case InterpMethod::INTERP_BESSEL:
			if (besselSpline_1D) return (T)besselSpline_1D->eval((Splines::valueType)val);
			break;
		case InterpMethod::INTERP_HERMITE:
			if (hermiteSpline_1D) return (T)hermiteSpline_1D->eval((Splines::valueType)val);
			break;
		default:
			//TODO: WARNING message for using unsupported Interpolation Method
			break;
		}

		return (T)0;
	}

	template <typename T>
	T evaluate(T rowVal, T colVal, InterpMethod method)
	{
		switch (method)
		{
		case InterpMethod::INTERP_CUBIC:
			if (cubicSpline_2D) return (T)cubicSpline_2D->eval((Splines::valueType)rowVal, (Splines::valueType)colVal);
			break;
		case InterpMethod::INTERP_AKIMA:
			if (akimaSpline_2D) return (T)akimaSpline_2D->eval((Splines::valueType)rowVal, (Splines::valueType)colVal);
			break;
		case InterpMethod::INTERP_QUINTIC:
			if (qunticSpline_2D) return (T)qunticSpline_2D->eval((Splines::valueType)rowVal, (Splines::valueType)colVal);
			break;
		default:
			//TODO: WARNING message for using unsupported Interpolation Method
			break;
		}

		return (T)0;
	}

	~TableSplines() {
		deleteSplines();
	}

	Splines::PchipSpline* pchipSpline_1D = nullptr;
	Splines::CubicSpline* cubicSpline_1D = nullptr;
	Splines::AkimaSpline* akimaSpline_1D = nullptr;
	Splines::QuinticSpline* qunticSpline_1D = nullptr;
	Splines::BesselSpline* besselSpline_1D = nullptr;
	Splines::HermiteSpline* hermiteSpline_1D = nullptr;

	Splines::BiCubicSpline* cubicSpline_2D = nullptr;
	Splines::Akima2Dspline* akimaSpline_2D = nullptr;
	Splines::BiQuinticSpline* qunticSpline_2D = nullptr;
};

template <typename T>
class Table<T>::Impl
{
public:
	//Number of dimensions of data
	unsigned int dimensions = 0;
	//Number of Rows of data
	unsigned int numRows = 0;
	//Number of Columns of data (1 for 1D tables)
	unsigned int numColumns = 0;
	//Number of tables of 2D data
	unsigned int numTables = 0;
	//Row counter for data insertion
	unsigned int rowInsertCtr = 0;
	//Column counter for data insertion
	unsigned int columnInsertCtr = 0;
	//Cached row index from last interpolation
	mutable unsigned int lastRowIdx = 0;
	//Cached column index from last interpolation
	mutable unsigned int lastColumnIdx = 0;
	//Interpolation method
	InterpMethod interpMethod = InterpMethod::INTERP_LINEAR;
	//Have splines already been built for the data?
	bool splinesBuilt = false;
	//2D Array of table data (only one dimension used in 1D tables)
	T** data = nullptr;
	//Vector of 2D tables (constitutes the 3rd dimension in the root table)
	std::vector<Table<T>> tables;

	TableSplines splines;

	//Allocate memory for the 2D array sizes
	T** allocateArraySize()
	{
		data = new T*[numRows + 1];
		for (unsigned int r = 0; r <= numRows; r++) {
			data[r] = new T[numColumns + 1];
			for (unsigned int c = 0; c <= numColumns; c++) {
				data[r][c] = 0.0;
			}
		}
		return data;
	}

	void buildSplines()
	{
		if (interpMethod > InterpMethod::INTERP_NEAREST)
		{
			if (numColumns == 1 && tables.size() == 0)
			{
				//1D table
				if (checkInterpolationMethod(interpMethod))
				{
					std::vector<Splines::valueType> xVals;
					std::vector<Splines::valueType> yVals;
					for (unsigned int r = 1; r <= numRows; r++) {
						xVals.push_back((Splines::valueType)data[r][0]);
						yVals.push_back((Splines::valueType)data[r][1]);
					}

					splines.deleteSplines();

					switch (interpMethod)
					{
					case InterpMethod::INTERP_PCHIP:
						splines.pchipSpline_1D = new Splines::PchipSpline();
						splines.pchipSpline_1D->build(xVals, yVals);
						break;
					case InterpMethod::INTERP_CUBIC:
						splines.cubicSpline_1D = new Splines::CubicSpline();
						splines.cubicSpline_1D->build(xVals, yVals);
						break;
					case InterpMethod::INTERP_AKIMA:
						splines.akimaSpline_1D = new Splines::AkimaSpline();
						splines.akimaSpline_1D->build(xVals, yVals);
						break;
					case InterpMethod::INTERP_QUINTIC:
						splines.qunticSpline_1D = new Splines::QuinticSpline();
						splines.qunticSpline_1D->build(xVals, yVals);
						break;
					case InterpMethod::INTERP_BESSEL:
						splines.besselSpline_1D = new Splines::BesselSpline();
						splines.besselSpline_1D->build(xVals, yVals);
						break;
					case InterpMethod::INTERP_HERMITE:
						splines.hermiteSpline_1D = new Splines::HermiteSpline();
						splines.hermiteSpline_1D->build(xVals, yVals);
						break;
					default:
						//should never happen
						return;
					}

					splinesBuilt = true;
				}
				else {
					//TODO: WARNING message for using unsupported Interpolation Method
					interpMethod = InterpMethod::INTERP_LINEAR;
					splinesBuilt = false;
				}

			}
			else if (numColumns > 1)
			{
				//2D table
				if (checkInterpolationMethod(interpMethod))
				{
					std::vector<Splines::valueType> xVals;
					std::vector<Splines::valueType> yVals;
					std::vector<Splines::valueType> zVals;
					for (unsigned int c = 1; c <= numColumns; c++) {
						yVals.push_back((Splines::valueType)data[0][c]);
					}
					for (unsigned int r = 1; r <= numRows; r++) {
						xVals.push_back((Splines::valueType)data[r][0]);
						for (unsigned int c = 1; c <= numColumns; c++) {
							zVals.push_back((Splines::valueType)data[r][c]);
						}
					}

					splines.deleteSplines();

					switch (interpMethod)
					{
					case InterpMethod::INTERP_CUBIC:
						splines.cubicSpline_2D = new Splines::BiCubicSpline();
						splines.cubicSpline_2D->build(xVals, yVals, zVals);
						break;
					case InterpMethod::INTERP_AKIMA:
						splines.akimaSpline_2D = new Splines::Akima2Dspline();
						splines.akimaSpline_2D->build(xVals, yVals, zVals);
						break;
					case InterpMethod::INTERP_QUINTIC:
						splines.qunticSpline_2D = new Splines::BiQuinticSpline();
						splines.qunticSpline_2D->build(xVals, yVals, zVals);
						break;
					default:
						//should never happen
						return;
					}

					splinesBuilt = true;
				}
				else {
					//TODO: WARNING message for using unsupported Interpolation Method
					interpMethod = InterpMethod::INTERP_LINEAR;
					splinesBuilt = false;
				}
			}
			else {
				//3D table, no splines
			}
		}
	}

	//Constructor
	Impl() {

	}
	Impl(unsigned int numberRows, InterpMethod method);

	Impl(unsigned int numberRows, unsigned int numberColumns, InterpMethod method);

	Impl(const std::vector<Table<T>>& tables, const std::vector<T>& breakpoints);

	Impl(Table<T>* tables[], T breakpoints[], size_t arraySize);

	bool checkInterpolationMethod(InterpMethod method)
	{
		if (dimensions == 1) {
			if (method > InterpMethod::INTERP_NEAREST) {
				return true;
			}
		}
		else if (dimensions == 2) {
			if (method >= InterpMethod::INTERP_CUBIC && method <= InterpMethod::INTERP_QUINTIC) {
				return true;
			}
		}
		else if (dimensions == 3) {
			if (method <= InterpMethod::INTERP_NEAREST) {
				return true;
			}
		}

		return false;
	}

	void deleteData() {
		for (unsigned int r = 0; r <= numRows; r++) delete[] data[r];
		delete[] data;
		data = nullptr;
	}

	//Destructor
	~Impl() {
		tables.clear();
		deleteData();
	}

	/// Copy constructor
	Impl(const Impl& impl);

	/// Copy assignment constructor
	Impl &operator=(const Impl &impl)
	{
		if (data) deleteData();
		dimensions = impl.dimensions;
		numRows = impl.numRows;
		numColumns = impl.numColumns;
		numTables = impl.numTables;
		rowInsertCtr = impl.rowInsertCtr;
		columnInsertCtr = impl.columnInsertCtr;
		lastRowIdx = impl.lastRowIdx;
		lastColumnIdx = impl.lastColumnIdx;
		interpMethod = impl.interpMethod;
		tables = impl.tables;
		data = allocateArraySize();
		splinesBuilt = false;

		for (unsigned int r = 0; r <= numRows; r++)
		{
			for (unsigned int c = 0; c <= numColumns; c++) {
				data[r][c] = impl.data[r][c];
			}
		}

		return *this;
	}
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Impl::Impl(unsigned int numberRows, InterpMethod method)
{
	dimensions = 1;
	numRows = numberRows;
	if (numRows < 1) {
		numRows = 1;
	}
	numColumns = 1;
	if (checkInterpolationMethod(method)) {
		interpMethod = method;
	}
	else {
		//TODO: WARNING message for using unsupported Interpolation Method
		interpMethod = InterpMethod::INTERP_LINEAR;
	}
	rowInsertCtr = 1;
	columnInsertCtr = 0;
	numTables = 0;

	data = allocateArraySize();
	lastRowIdx = 2;
	lastColumnIdx = 2;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Impl::Impl(unsigned int numberRows, unsigned int numberColumns, InterpMethod method)
{
	dimensions = 2;
	numRows = numberRows;
	numColumns = numberColumns;
	rowInsertCtr = 0;
	columnInsertCtr = 1;
	if (numColumns < 2) {
		dimensions = 1;
		numColumns = 1;
		rowInsertCtr = 1;
		columnInsertCtr = 0;
	}
	numTables = 0;
	if (checkInterpolationMethod(method)) {
		interpMethod = method;
	}
	else {
		//TODO: WARNING message for using unsupported Interpolation Method
		interpMethod = InterpMethod::INTERP_LINEAR;
	}

	data = allocateArraySize();
	lastRowIdx = 2;
	lastColumnIdx = 2;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Impl::Impl(const std::vector<Table<T>>& tables, const std::vector<T>& breakpoints)
{
	numTables = (unsigned int)tables.size();
	numRows = numTables;
	numColumns = 1;
	if (numTables <= 0 || breakpoints.size() != numTables)
		return;

	dimensions = 3;
	rowInsertCtr = 1;
	columnInsertCtr = 1;
	lastRowIdx = 2;
	lastColumnIdx = 2;

	data = allocateArraySize();
	this->tables.reserve(numTables);
	bool allTablesNearestInterp = true;
	for (unsigned int i = 0; i < numTables; i++)
	{
		//If all tables within 3d table are nearest neighbor, also make 3rd dimension nearest neighbor
		if (tables.at(i).mImpl->interpMethod != InterpMethod::INTERP_NEAREST) {
			allTablesNearestInterp = false;
		}
		this->tables.push_back(tables.at(i));
		data[i + 1][1] = breakpoints.at(i);
	}

	if (allTablesNearestInterp) {
		interpMethod = InterpMethod::INTERP_NEAREST;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Impl::Impl(Table<T>* tables[], T breakpoints[], size_t arraySize)
{
	numTables = (unsigned int)arraySize;
	numRows = numTables;
	numColumns = 1;
	if (arraySize <= 0)
		return;

	dimensions = 3;
	columnInsertCtr = 1;
	rowInsertCtr = 1;
	lastRowIdx = 2;
	lastColumnIdx = 2;

	data = allocateArraySize();
	this->tables.reserve(numTables);
	bool allTablesNearestInterp = true;
	for (unsigned int i = 0; i < numTables; i++)
	{
		//If all tables within 3d table are nearest neighbor, also make 3rd dimension nearest neighbor
		if (tables[i]->mImpl->interpMethod != InterpMethod::INTERP_NEAREST) {
			allTablesNearestInterp = false;
		}
		this->tables.push_back(*tables[i]);
		data[i + 1][1] = breakpoints[i];
	}

	if (allTablesNearestInterp) {
		interpMethod = InterpMethod::INTERP_NEAREST;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Impl::Impl(const Impl& impl)
{
	dimensions = impl.dimensions;
	numRows = impl.numRows;
	numColumns = impl.numColumns;
	numTables = impl.numTables;
	rowInsertCtr = impl.rowInsertCtr;
	columnInsertCtr = impl.columnInsertCtr;
	lastRowIdx = impl.lastRowIdx;
	lastColumnIdx = impl.lastColumnIdx;
	interpMethod = impl.interpMethod;
	tables = impl.tables;
	data = allocateArraySize();
	splinesBuilt = false;

	for (unsigned int r = 0; r <= numRows; r++)
	{
		for (unsigned int c = 0; c <= numColumns; c++) {
			data[r][c] = impl.data[r][c];
		}
	}

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
InterpMethod Table<T>::getInterpolationMethod() const
{
	return mImpl->interpMethod;
}

template <typename T>
void Table<T>::changeInterpolationMethod(InterpMethod method)
{
	if (mImpl->interpMethod != method) {
		if (mImpl->checkInterpolationMethod(method)) {
			mImpl->interpMethod = method;
			mImpl->splinesBuilt = false; //splines will be rebuilt next time interp() is called
		}
		else {
			//TODO: WARNING message for using unsupported Interpolation Method
		}
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
unsigned int Table<T>::getNumRows() const
{
	return mImpl->numRows;
}

template <typename T>
unsigned int Table<T>::getNumColumns() const
{
	return mImpl->numColumns;
}

template <typename T>
unsigned int Table<T>::getNumTables() const
{
	return mImpl->numTables;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Table(unsigned int numberRows, InterpMethod method)
{
	mImpl = new Table::Impl(numberRows, method);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Table(unsigned int numberRows, unsigned int numberColumns, InterpMethod method)
{
	mImpl = new Table::Impl(numberRows, numberColumns, method);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Table(const std::vector<Table<T>>& tables, const std::vector<T>& breakpoints)
{
	mImpl = new Table::Impl(tables, breakpoints);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Table(Table<T>* tables[], T breakpoints[], size_t arraySize)
{
	mImpl = new Table::Impl(tables, breakpoints, arraySize);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
const Table<T> &Table<T>::operator=(const Table &table)
{
	delete mImpl;
	mImpl = new Table<T>::Impl();
	*mImpl = *table.mImpl;

	return *this;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::Table(const Table<T>& table)
{
	delete mImpl;
	mImpl = new Table<T>::Impl();
	*mImpl = *table.mImpl;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>::~Table()
{
	delete mImpl;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
T Table<T>::interp(T val, bool extrapolate) const
{
	T interpVal = 0;
	if (mImpl->interpMethod > InterpMethod::INTERP_NEAREST)
	{
		if (!mImpl->splinesBuilt)
			mImpl->buildSplines();
	}

	//cannot extrapolate with Nearest Neighbor selection
	if (mImpl->interpMethod == InterpMethod::INTERP_NEAREST) {
		extrapolate = false;
	}
	//check for extrapolation
	if (!extrapolate)
	{
		if (val <= mImpl->data[1][0])
		{
			mImpl->lastRowIdx = 2;
			return mImpl->data[1][1];
		}
		else if (val >= mImpl->data[mImpl->numRows][0])
		{
			mImpl->lastRowIdx = mImpl->numRows;
			return mImpl->data[mImpl->numRows][1];
		}
	}

	if (mImpl->interpMethod > InterpMethod::INTERP_NEAREST)
	{
		//Spline interpolation
		interpVal = mImpl->splines.evaluate(val, mImpl->interpMethod);
	}
	else
	{
		unsigned int r = mImpl->lastRowIdx;

		while (r > 2 && mImpl->data[r - 1][0] > val) {
			r--;
		}
		while (r < mImpl->numRows && mImpl->data[r][0] < val) {
			r++;
		}

		mImpl->lastRowIdx = r;

		T rng = mImpl->data[r][0] - mImpl->data[r - 1][0];
		T fac = 0;
		if (rng != 0.0)
		{
			fac = (val - mImpl->data[r - 1][0]) / rng;
			if (!extrapolate) {
				fac = fac > 1.0 ? (T)1.0 : fac < 0.0 ? (T)0.0 : fac;
			}
		}
		else {
			fac = (T)1.0;
		}

		T interpVal;
		if (mImpl->interpMethod == InterpMethod::INTERP_LINEAR) {
			interpVal = fac*(mImpl->data[r][1] - mImpl->data[r - 1][1]) + mImpl->data[r - 1][1];
		}
		else if (mImpl->interpMethod == InterpMethod::INTERP_NEAREST)
		{
			if (fac < 0.5) {
				interpVal = mImpl->data[r - 1][1];
			}
			else {
				interpVal = mImpl->data[r][1];
			}
		}
	}

	return interpVal;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
T Table<T>::interp(T rowVal, T colVal, bool extrapolate) const
{
	T interpVal = 0;
	if (mImpl->interpMethod > InterpMethod::INTERP_NEAREST)
	{
		if (!mImpl->splinesBuilt)
			mImpl->buildSplines();
	}

	//cannot extrapolate with Nearest Neighbor selection
	if (mImpl->interpMethod == InterpMethod::INTERP_NEAREST) {
		extrapolate = false;
	}

	bool outsideBounds = false;
	outsideBounds = (rowVal <= mImpl->data[1][0] || rowVal >= mImpl->data[mImpl->numRows][0]) ||
					(colVal <= mImpl->data[0][1] || colVal >= mImpl->data[0][mImpl->numColumns]);

	if (mImpl->interpMethod > InterpMethod::INTERP_NEAREST && (!outsideBounds || (outsideBounds && extrapolate)))
	{
		//Spline interpolation
		interpVal = mImpl->splines.evaluate(rowVal, colVal, mImpl->interpMethod);
	}
	else
	{
		unsigned int r = mImpl->lastRowIdx;
		unsigned int c = mImpl->lastColumnIdx;

		while (r > 2 && mImpl->data[r - 1][0] > rowVal) {
			r--;
		}
		while (r < mImpl->numRows && mImpl->data[r][0] < rowVal) {
			r++;
		}

		while (c > 2 && mImpl->data[0][c - 1] > colVal) {
			c--;
		}
		while (c < mImpl->numColumns && mImpl->data[0][c] < colVal) {
			c++;
		}

		mImpl->lastRowIdx = r;	mImpl->lastColumnIdx = c;

		T rFac = (rowVal - mImpl->data[r - 1][0]) /
			(mImpl->data[r][0] - mImpl->data[r - 1][0]);

		T cFac = (colVal - mImpl->data[0][c - 1]) /
			(mImpl->data[0][c] - mImpl->data[0][c - 1]);

		if (!extrapolate)
		{
			rFac = rFac > 1.0 ? (T)1.0 : rFac < 0.0 ? (T)0.0 : rFac;
			cFac = cFac > 1.0 ? (T)1.0 : cFac < 0.0 ? (T)0.0 : cFac;
		}

		if (mImpl->interpMethod == InterpMethod::INTERP_NEAREST)
		{
			if (rFac < 0.5 && cFac < 0.5) {
				interpVal = mImpl->data[r - 1][c - 1];
			}
			else if (rFac < 0.5 && cFac >= 0.5) {
				interpVal = mImpl->data[r - 1][c];
			}
			else if (rFac >= 0.5 && cFac < 0.5) {
				interpVal = mImpl->data[r][c - 1];
			}
			else {
				interpVal = mImpl->data[r][c];
			}
		}
		else
		{
			//linear
			T lowerColVal = rFac*(mImpl->data[r][c - 1] - mImpl->data[r - 1][c - 1]) + mImpl->data[r - 1][c - 1];
			T upperColVal = rFac*(mImpl->data[r][c] - mImpl->data[r - 1][c]) + mImpl->data[r - 1][c];
			interpVal = lowerColVal + cFac*(upperColVal - lowerColVal);
		}
	}

	return interpVal;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
T Table<T>::interp(T rowVal, T colVal, T tableVal, bool extrapolate) const
{
	unsigned int r = mImpl->lastRowIdx;

	//cannot use spline interpolation on 3rd dimension
	if (mImpl->interpMethod > InterpMethod::INTERP_NEAREST) {
		mImpl->interpMethod = InterpMethod::INTERP_LINEAR;
	}

	//cannot extrapolate with Nearest Neighbor selection
	if (mImpl->interpMethod == InterpMethod::INTERP_NEAREST) {
		extrapolate = false;
	}

	if (!extrapolate)
	{
		if (tableVal <= mImpl->data[1][1])
		{
			mImpl->lastRowIdx = 2;
			return mImpl->tables[0].interp(rowVal, colVal, false);
		}
		else if (tableVal >= mImpl->data[mImpl->numRows][1])
		{
			mImpl->lastRowIdx = mImpl->numRows;
			return mImpl->tables[mImpl->numRows - 1].interp(rowVal, colVal, false);
		}
	}


	while (r > 2 && mImpl->data[r - 1][1] > tableVal) {
		r--;
	}
	while (r < mImpl->numRows && mImpl->data[r][1] < tableVal) {
		r++;
	}

	mImpl->lastRowIdx = r;

	T rng = mImpl->data[r][1] - mImpl->data[r - 1][1];
	T fac = 0;
	if (rng != 0.0)
	{
		fac = (tableVal - mImpl->data[r - 1][1]) / rng;
		if (!extrapolate) {
			fac = fac > 1.0 ? fac = (T)1.0 : fac < 0.0 ? (T)0.0 : fac;
		}
	}
	else {
		fac = (T)1.0;
	}

	T table_value_low = mImpl->tables[r - 2].interp(rowVal, colVal, extrapolate);
	T table_value_high = mImpl->tables[r - 1].interp(rowVal, colVal, extrapolate);

	T interpVal;
	//3D interpolation between 2D tables only allows Nearest Neighbor or Linear
	if (mImpl->interpMethod == InterpMethod::INTERP_NEAREST)
	{
		if (fac < 0.5) {
			interpVal = table_value_low;
		}
		else {
			interpVal = table_value_high;
		}
	}
	else {
		//Linear
		interpVal = fac*(table_value_high - table_value_low) + table_value_low;
	}

	return interpVal;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename T>
Table<T>& Table<T>::operator<<(const T n)
{
	//Prevent access violation, was a common cause of runtime crashes, so check was needed
	if (mImpl->rowInsertCtr <= mImpl->numRows && mImpl->columnInsertCtr <= mImpl->numColumns)
	{
		mImpl->data[mImpl->rowInsertCtr][mImpl->columnInsertCtr] = n;
		if (mImpl->columnInsertCtr == (int)mImpl->numColumns) {
			mImpl->columnInsertCtr = 0;
			mImpl->rowInsertCtr++;
		}
		else {
			mImpl->columnInsertCtr++;
		}
	}
	else {
		//TODO: ERROR message informing user of attempting to access invalid array cell
	}
	return *this;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
Table<T>& Table<T>::operator<<(const int n)
{
	*this << (T)n;
	mImpl->splinesBuilt = false;
	return *this;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
T Table<T>::get(unsigned int row, unsigned int col) const
{
	return mImpl->data[row][col];
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <typename T>
T Table<T>::operator()(unsigned int row, unsigned int col) const
{
	return get(row, col);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// Declare float and double types so the compiler can build these templates
template class MATH_API Table<float>;
template class MATH_API Table<double>;

} //namespace otMath

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%