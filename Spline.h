//----------------------------------------------------------------------------
// Filename: Spline.h
//----------------------------------------------------------------------------
// Functionality: Performs interpolation using cubic spline curves
//----------------------------------------------------------------------------
// Description: 1. Provides two methods for setting boundary conditions:
//                  1) Given first derivatives at the endpoints
//                  2) Given second derivatives at the endpoints
//              2. Provides three methods for interpolation calculations:
//                  1) Calculate the interpolation for a single given point, 
//                     which may throw an extrapolation exception
//                  2) Calculate interpolations for multiple given points,
//                     which may throw an extrapolation exception
//                  3) Specify the number of interpolation outputs (at least 2),
//                     and automatically generate interpolated values with equal 
//                     spacing between Max(x) and Min(x). x is the sequence of 
//                     independent variable values, and this may throw an 
//                     insufficient points exception
//----------------------------------------------------------------------------
// Author: Dodge Ho (https://github.com/DodgeHo)
// First-author£º ÁõÑÇ±ò (https://www.cnblogs.com/yabin/p/6426849.html)
// Date£º2024.08.11
//-----------------------------------------------------------------------------

#ifndef _SPLINE_H
#define _SPLINE_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
using std::vector;

#pragma warning (disable:4290)       
// In Visual Studio 2010, the C++ compiler does not check for exceptions thrown by functions. This warning is disabled.
// Since the class names in this file are relatively short, a namespace is used to avoid name conflicts.

namespace SplineSpace {
	class SplineFailure {
	private:
		const char* Message;
	public:
		SplineFailure(const char* msg);
		const char* GetMessage();
	};

	class SplineInterface {
	public:
		// Single-point interpolation:
		// x -> input independent variable (input)
		// y -> returns the interpolation result (output)
		// If x is not within the interpolation range, the interpolation fails
		virtual bool SinglePointInterp(const double x, double& y) = 0;

		// Multi-point interpolation:
		// x -> pointer to the array of input independent variables, num -> reference to the number of independent variables (input)
		// y -> pointer to the array returning the interpolation results (output array)
		virtual bool MultiPointInterp(vector<double>& x, const int num, vector<double>& y) = 0;

		// Automatic interpolation:
		// num -> reference to the number of interpolation points to generate (input)
		// x -> pointer to the array storing the independent variables (output array)
		// y -> pointer to the array of corresponding interpolation results (output array)
		virtual bool AutoInterp(const int num, vector<double>& x, vector<double>& y) = 0;

		virtual ~SplineInterface() {};
	};

	enum BoundaryCondition {		// Boundary condition enumeration
		GivenFirstOrder = 1,		// Given first derivative
		GivenSecondOrder = 2,		// Given second derivative
	};

	class Spline : public SplineInterface {		// Implementation class
	public:
		// Constructor:
		// x0 -> pointer to the array of known independent variables, y0 -> pointer to the array of known dependent variables, num -> number of known data points
		// bc -> boundary condition, default is given second derivative
		// leftBoundary, rightBoundary -> references to the values of derivatives at the left and right boundaries, default is 0
		Spline(vector<double>& x0, vector<double>& y0, const int num,
			BoundaryCondition bc = GivenSecondOrder, const double leftBoundary = 0, const double rightBoundary = 0);

		bool SinglePointInterp(const double x, double& y) throw(SplineFailure);		// Implementation of virtual function
		bool MultiPointInterp(vector<double>& x, const int num, vector<double>& y) throw(SplineFailure);
		bool AutoInterp(const int num, vector<double>& x, vector<double>& y) throw(SplineFailure);

		~Spline();

	private:
		// Methods
		void PartialDerivative1(void);		// Calculate first derivative
		void PartialDerivative2(void);		// Calculate second derivative

		// Attributes
		vector<double> GivenX;	// Known independent variables
		vector<double> GivenY;	// Known dependent variables
		const int GivenNum;		// Number of known data points
		const BoundaryCondition Bc;	// Boundary condition type
		const double LeftB;
		const double RightB;

		vector<double> PartialDerivative;	// Stores the corresponding first or second partial derivatives
		double MaxX;				// Maximum value of the independent variable
		double MinX;				// Minimum value of the independent variable

		vector<double> SplineX;	// Interpolated independent variables
		vector<double> SplineY;		// Interpolated dependent variables
	};
}
#endif
