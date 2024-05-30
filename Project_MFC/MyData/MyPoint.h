#pragma once

#include "MyCoord.h"

#ifdef MYDATA_EXPORTS
#define MYDATA_API __declspec(dllexport)
#else
#define MYDATA_API __declspec(dllimport)
#endif

class MYDATA_API MY_POINT : public MY_COORD
{
public:
	double x;
	double y;
	char* name;
	int numb;
	COLORREF color;
public:
	MY_POINT() : x(0), y(0), name(nullptr) {}
	MY_POINT(double xx, double yy, COLORREF color) : x(xx), y(yy), name(nullptr), color(color){}
	~MY_POINT();
	void set(double xx, double yy) { x = xx; y = yy; }
	MY_POINT get() { return *this; }
};
